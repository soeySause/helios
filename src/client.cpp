#include "client.hpp"

namespace helios {
    client::client(const std::string& token) {
        this->cache_ = std::make_shared<cache>();
        cache_->put("token", token);
        this->guilds.token = cache_->get("token");

        const json getGateway = json::parse(request::getRequest("discord.com", "/api/gateway/bot", "", token));
        cache_->put("shards", getGateway["shards"].dump());
        cache_->put("url", getGateway["url"]);
    }

    [[maybe_unused]] [[noreturn]] void client::run() {
        if(!cache_->exist("url")) {
            throw(heliosException(60, "Failed to connect to websocket. No url found"));
        }

        load_root_certificates(this->sslContext);
        const std::string host = this->cache_->get("url").substr(6, cache_->get("url").length() - 6);
        this->cache_->put("host", host);

        if(!enableSharding) {
            std::shared_ptr<shard> newShard = std::make_shared<shard>();
            newShard->shardStructPtr->shardId = 0;

            this->shardClass.emplace_back(newShard);
        }

        // set running as true so new shards know to call
        // createWsShard() themselves after this point
        for(auto& shard : this->shardClass) {
            shard->shardStructPtr->running = true;
        }

        // create and start threads
        for(auto& shard : this->shardClass) {
            this->createWsShard(shard);
        }

        while(true) {
            std::unique_lock<std::mutex> lock(mutex);
            updateCondition.wait(lock, [&] {
                for (auto& shard: this->shardClass) {
                    if (shard->shardStructPtr->exitFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        if(shard->shardStructPtr->deleteShard) {
                            this->deleteShard(shard);
                            break;
                        }

                        if(shard->shardStructPtr->reconnect) {
                            this->reconnectShard(shard);
                            break;
                        }

                        if(shard->shardStructPtr->fullReconnect) {
                            this->fullReconnectShard(shard);
                            break;
                        }

                        switch(shard->shardStructPtr->closeCode) {
                            case 4000:
                            case 4001:
                            case 4002:
                            case 4003:
                            case 4005:
                            case 4007:
                            case 4008:
                            case 4009: {
                                this->reconnectShard(shard);
                                break;
                            }
                            case 4004:
                            case 4010:
                            case 4011:
                            case 4012:
                            case 4013:
                            case 4014: {
                                throw(heliosException(shard->shardStructPtr->closeCode, shard->shardStructPtr->closeReason.c_str()));
                            }
                            default:
                                this->reconnectShard(shard);
                                break;
                        }
                        break;
                    }
                }
                return false;
            });
        }
    }

    void client::reconnectShard(const std::shared_ptr<shard>& shardR){
        if(shardR->shardStructPtr->shardThread->joinable()) {
            shardR->shardStructPtr->shardThread->join();
        }

        std::unique_ptr<shardStruct> newShardStructPtr = std::make_unique<shardStruct>();
        newShardStructPtr->shardId = shardR->shardStructPtr->shardId;
        newShardStructPtr->resumeUrl = shardR->shardStructPtr->resumeUrl;
        newShardStructPtr->reconnect = true;
        newShardStructPtr->running = shardR->shardStructPtr->running;
        newShardStructPtr->sessionId = shardR->shardStructPtr->sessionId;
        newShardStructPtr->seq = shardR->shardStructPtr->seq;

        shardR->shardStructPtr.swap(newShardStructPtr);
        this->createWsShard(shardR);
    }

    void client::fullReconnectShard(const std::shared_ptr<shard>& shardR) {
        if(shardR->shardStructPtr->shardThread->joinable()) {
            shardR->shardStructPtr->shardThread->join();
        }

        std::unique_ptr<shardStruct> newShardStructPtr = std::make_unique<shardStruct>();
        newShardStructPtr->shardId = shardR->shardStructPtr->shardId;

        shardR->shardStructPtr.swap(newShardStructPtr);
        this->createWsShard(shardR);
    }

    void client::deleteShard(const std::shared_ptr<shard>& shardR) {
        if(shardR->shardStructPtr->shardThread->joinable()) {
            shardR->shardStructPtr->shardThread->join();
        }

        auto shardPositionInVector = std::find_if(this->shardClass.begin(), this->shardClass.end(), [&shardR](const std::shared_ptr<shard>& targetShard)
        { return targetShard->shardStructPtr == shardR->shardStructPtr; });

        if (shardPositionInVector != this->shardClass.end()) {
            this->shardClass.erase(shardPositionInVector);
        } else {
            std::stringstream targetShardAddress;
            targetShardAddress << shardR.get();

            const std::string error = "Cannot find shard " + targetShardAddress.str();
            throw(heliosException(81, error));
        }
    }



    void client::createWsShard(const std::shared_ptr<shard>& shard) {
        std::string host;

        if(shard->shardStructPtr->reconnect) {
            host = shard->shardStructPtr->resumeUrl;
        } else {
            host = cache_->get("host");
        }

        std::unique_ptr<std::thread> newWsShard;
        newWsShard = std::make_unique<std::thread>(&client::wsShard, this, host, shard);
        shard->shardStructPtr->shardThread.swap(newWsShard);
        shard->shardStructPtr->shardThreadId = shard->shardStructPtr->shardThread->get_id();
    }

    void client::wsShard(const std::string &host, const std::shared_ptr<shard>& shard) {
        net::io_context ioContext;
        std::shared_ptr<session> sessionShard = std::make_shared<session>(ioContext, this->sslContext, this->cache_);

        shard->shardStructPtr->sessionShard = sessionShard->weak_from_this();

        sessionShard->run(host, "443");
        ioContext.run();

        if(shard->shardStructPtr->reconnect) {
            json reconnectingPayload;
            reconnectingPayload["op"] = 6;
            reconnectingPayload["d"]["token"] = this->cache_->get("token");
            reconnectingPayload["d"]["session_id"] = shard->shardStructPtr->sessionId;
            reconnectingPayload["d"]["seq"] = shard->shardStructPtr->seq;
            sessionShard->asyncQueue(reconnectingPayload.dump());
        }

        while (sessionShard->is_socket_open()) {
            ioContext.restart();
            sessionShard->enable_async_read();

            const std::string wsResponse = sessionShard->getResponse();
            std::cout << wsResponse << std::endl;
            this->parseResponse(shard, wsResponse);

            ioContext.run();
        }

        // After this point, the connection is closed and cannot be reopened without creating a new session
        if(!ioContext.stopped()) ioContext.stop();

        shard->shardStructPtr->closeCode = sessionShard->getCloseCode();
        shard->shardStructPtr->closeReason = sessionShard->getCloseReason();
        shard->shardStructPtr->exit.set_value(true);
        stopHeartbeatCycle(shard);

        this->updateCondition.notify_one();
    }

    void client::parseResponse(const std::shared_ptr<shard>& shard, const std::string& response)
    {
        std::shared_ptr<session> sessionShard = shard->shardStructPtr->sessionShard.lock();
        const json wsResponseJson = json::parse(response);
        const int opCode = wsResponseJson["op"];
        std::string event;

        switch(opCode) {
            case 0: {
                // Case 0 is for sending and receiving events
                event = wsResponseJson["t"];
                shard->shardStructPtr->seq++;
                break;
            }
            case 1: {
                sendHeartbeat(shard);
                break;
            }
            case 7: {
                // Case 7 is sent when discord wants you to reconnect
                event = "RECONNECT";
                break;
            }
            case 9: {
                // Case 9 is sent when ws session is invalid
                if(wsResponseJson["d"] == "true")
                    event = "RECONNECT";
                else
                    event = "DISCONNECT";
                break;
            }
            case 10: {
                // Case 0 is sent on initial connection with discord
                shard->shardStructPtr->heartbeatInterval = std::stoi(wsResponseJson["d"]["heartbeat_interval"].dump());
                event = "ON_ENABLE";
                break;
            }
            case 11: {
                // Case 11 is sent on receiving heartbeats
                shard->shardStructPtr->receivedHeartbeat = true;
                break;
            }
            default: {
                std::cerr << "Unknown op code " << std::to_string(opCode) << std::endl;
                break;
            }
        }

        if(!event.empty()) {
            const std::string& executeEvent = event;

            if(executeEvent == "ON_ENABLE"){
                startHeartbeatCycle(shard);
                if(!shard->shardStructPtr->reconnect) {
                    json identifyPayload = getIdentifyPayload(shard->shardStructPtr->shardId);
                    sessionShard->asyncQueue(identifyPayload.dump());
                }
                shard->shardStructPtr->reconnect = false;
                return;
            }

            if(executeEvent == "READY") {
                const std::string resumeGatewayUrl = wsResponseJson["d"]["resume_gateway_url"];
                shard->shardStructPtr->resumeUrl = resumeGatewayUrl.substr(6, resumeGatewayUrl.length() - 6);
                shard->shardStructPtr->sessionId = wsResponseJson["d"]["session_id"];

                if(this->onEvent.readyFunction) {
                    readyEvent readyEventData = shard->shardStructPtr->eventData.getReadyEventData(wsResponseJson["d"]);
                    this->onEvent.readyFunction(readyEventData);
                }
            }

            if(executeEvent == "RESUMED") {
                if(this->onEvent.resumedFunction) {
                    resumedEvent resumedEventData = shard->shardStructPtr->eventData.getResumedEventData(wsResponseJson["d"]);
                    this->onEvent.resumedFunction(resumedEventData);
                }
            }

            if(executeEvent == "RECONNECT") {
                shard->shardStructPtr->reconnect = true;
                sessionShard->asyncCloseSession();
                return;
            }

            if(executeEvent == "DISCONNECT") {
                shard->shardStructPtr->fullReconnect = true;
                sessionShard->asyncCloseSession();
                return;
            }

            if(executeEvent == "CHANNEL_CREATE"){
                if(this->onEvent.channelCreateFunction) {
                    channelCreateEvent channelCreateData = shard->shardStructPtr->eventData.getChannelCreateEventData(wsResponseJson["d"]);
                    this->onEvent.channelCreateFunction(channelCreateData);
                }
            }

            if(executeEvent == "CHANNEL_UPDATE"){
                if(this->onEvent.channelUpdateFunction) {
                    channelUpdateEvent channelUpdateData = shard->shardStructPtr->eventData.getChannelUpdateEventData(wsResponseJson["d"]);
                    this->onEvent.channelUpdateFunction(channelUpdateData);
                }
            }

            if(executeEvent == "CHANNEL_DELETE"){
                if(this->onEvent.channelDeleteFunction) {
                    channelDeleteEvent channelDeleteData = shard->shardStructPtr->eventData.getChannelDeleteEventData(wsResponseJson["d"]);
                    this->onEvent.channelDeleteFunction(channelDeleteData);
                }
            }

            if(executeEvent == "GUILD_CREATE"){
                if(this->onEvent.guildCreateFunction) {
                    guildCreateEvent guildCreateData = shard->shardStructPtr->eventData.getGuildCreateEventData(wsResponseJson["d"]);
                    this->onEvent.guildCreateFunction(guildCreateData);
                }
            }

            if(executeEvent == "GUILD_UPDATE"){
                if(this->onEvent.guildUpdateFunction) {
                    guildUpdateEvent guildUpdateData = shard->shardStructPtr->eventData.getGuildUpdateEventData(wsResponseJson["d"]);
                    this->onEvent.guildUpdateFunction(guildUpdateData);
                }
            }

            if(executeEvent == "GUILD_DELETE"){
                if(this->onEvent.guildDeleteFunction) {
                    guildDeleteEvent guildDeleteData = shard->shardStructPtr->eventData.getGuildDeleteEventData(wsResponseJson["d"]);
                    this->onEvent.guildDeleteFunction(guildDeleteData);
                }
            }

        }
    }

    void client::startHeartbeatCycle(const std::shared_ptr<shard>& shard) {
        std::unique_ptr<std::thread> heartbeatThread;

        heartbeatThread = std::make_unique<std::thread>(&client::heartbeatCycle, this, shard);
        shard->shardStructPtr->heartbeatThread.swap(heartbeatThread);

        shard->shardStructPtr->heartbeatThreadId = shard->shardStructPtr->heartbeatThread->get_id();
    }

    void client::stopHeartbeatCycle(const std::shared_ptr<shard>& shard) {
            if(shard->shardStructPtr->heartbeatThreadId == std::thread::id()) return;
            if(shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
            shard->shardStructPtr->exitHeartbeat.set_value(true);
            if(shard->shardStructPtr->heartbeatThread->joinable()) {
                shard->shardStructPtr->heartbeatThread->join();
            }
        }
    }

    void client::sendHeartbeat(const std::shared_ptr<shard>& shard)
    {
        json heartbeatPayload;
        heartbeatPayload["op"] = 1;
        heartbeatPayload["d"] = shard->shardStructPtr->seq;

        std::shared_ptr<session> sessionShard = shard->shardStructPtr->sessionShard.lock();
        sessionShard->asyncQueue(heartbeatPayload.dump());
    }

    void client::heartbeatCycle(const std::shared_ptr<shard>& shard)
    {
        if(!shard->shardStructPtr->heartbeatInterval.has_value()) {
            throw(heliosException(60, "Failed to start heartbeat. No heartbeat interval found"));
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        double randomNumber = dis(gen);
        double jitter = randomNumber * shard->shardStructPtr->heartbeatInterval.value();
        if (shard->shardStructPtr->exitHeartbeatFuture.wait_for(std::chrono::milliseconds((int)jitter)) == std::future_status::ready) {
            return;
        }
        std::shared_ptr<session> sessionShard = shard->shardStructPtr->sessionShard.lock();

        while (true) {
            this->sendHeartbeat(shard);

            // If program receivedHeartbeat is false, program never received ack and must restart
            if(!shard->shardStructPtr->receivedHeartbeat) {
                sessionShard->asyncCloseSession(websocket::close_code::abnormal);
                break;
            }

            shard->shardStructPtr->receivedHeartbeat = false;
            if (shard->shardStructPtr->exitHeartbeatFuture.wait_for(std::chrono::milliseconds(shard->shardStructPtr->heartbeatInterval.value())) == std::future_status::ready) {
                break;
            }
        }
    }


    json client::getIdentifyPayload(const int& shard) {
        json identifyPayload;
        identifyPayload["op"] = 2;
        identifyPayload["d"]["token"] = this->cache_->get("token");

        identifyPayload["d"]["properties"]["os"] = this->properties.os;
        identifyPayload["d"]["properties"]["browser"] = this->properties.browser;
        identifyPayload["d"]["properties"]["device"] = this->properties.device;

        identifyPayload["d"]["compress"] = this->compress;
        identifyPayload["d"]["large_threshold"] = this->large_threshold;

        if(enableSharding) {
            json jsonShards = json::array();
            jsonShards = {shard, std::stoi(this->cache_->get("shards"))};
            identifyPayload["d"]["shard"] = jsonShards;
        }

        json jsonActivitiesArray = json::array();
        json jsonActivitiesObject;
        if(!this->presence.activities.name.empty()) {
            jsonActivitiesObject["name"] = this->presence.activities.name;
            jsonActivitiesObject["type"] = this->presence.activities.type;
        }
        if(!this->presence.activities.url.empty()) {
            jsonActivitiesObject["url"] = this->presence.activities.url;
        }

        jsonActivitiesArray.push_back(jsonActivitiesObject);
        identifyPayload["d"]["presence"]["activities"] = jsonActivitiesArray;

        identifyPayload["d"]["presence"]["status"] = this->presence.status;
        identifyPayload["d"]["presence"]["since"] = this->presence.since;
        identifyPayload["d"]["presence"]["afk"] = this->presence.afk;

        identifyPayload["d"]["intents"] = this->intents;

        return identifyPayload;
    }

    [[maybe_unused]] void properties::setOs(const std::string &os) {
        this->os = os;
    }

    [[maybe_unused]] void properties::setBrowser(const std::string &browser) {
        this->browser = browser;
    }

    [[maybe_unused]] void properties::setDevice(const std::string &device) {
        this->device = device;
    }

    [[maybe_unused]] void activities::setName(const std::string &name) {
        this->name = name;
    }

    [[maybe_unused]] void activities::setType(const int& type) {
        this->type = type;
    }

    [[maybe_unused]] void activities::setUrl(const std::string& url){
        this->url = url;
    }

    [[maybe_unused]] void presence::setSince(const int& since) {
        this->since = since;
    }

    [[maybe_unused]] void presence::setStatus(const std::string& status) {
        this->status = status;
    }

    [[maybe_unused]] void presence::setAfk(const bool& afk) {
        this->afk = afk;
    }

    [[maybe_unused]] void client::setLargeThreshold(const int threshold) {
        this->large_threshold = threshold;
    }

    void client::reconnect() {

    }

} // helios

