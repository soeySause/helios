#include "client.hpp"

namespace helios {
    client::client(const std::string& token) {
        this->cache_ = std::make_shared<cache>();
        cache_->put("token", token);

        const int getGatewayResponseCode = apiRequest::getGateway(this->cache_);
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
        newShardStructPtr->reconnect = shardR->shardStructPtr->reconnect;
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



    void client::createWsShard(const std::shared_ptr<shard>& shardR) {
        std::string host;

        if(shardR->shardStructPtr->reconnect) {
            host = shardR->shardStructPtr->resumeUrl;
        } else {
            host = cache_->get("host");
        }

        std::unique_ptr<std::thread> newWsShard;
        newWsShard = std::make_unique<std::thread>(&client::wsShard, this, host);
        shardR->shardStructPtr->shardThread.swap(newWsShard);
        shardR->shardStructPtr->shardThreadId = shardR->shardStructPtr->shardThread->get_id();
    }

    void client::wsShard(const std::string &host) {
        net::io_context ioContext;
        std::shared_ptr<session> sessionShard = std::make_shared<session>(ioContext, this->sslContext, this->cache_);
        json reconnectingPayload;

        for (auto& shard : this->shardClass) {
            if(shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                std::weak_ptr<session> weakShard(sessionShard);
                shard->shardStructPtr->sessionShard.swap(weakShard);

                if(shard->shardStructPtr->reconnect) {
                    reconnectingPayload["op"] = 6;
                    reconnectingPayload["d"]["token"] = this->cache_->get("token");
                    reconnectingPayload["d"]["session_id"] = shard->shardStructPtr->sessionId;
                    reconnectingPayload["d"]["seq"] = shard->shardStructPtr->seq;
                }
                break;
            }

            if(shard == this->shardClass.back()) {
                std::stringstream targetShardAddress;
                targetShardAddress << shard->shardStructPtr->shardThreadId;

                const std::string error = "Cannot find thread " + targetShardAddress.str();
                throw(heliosException(82, error));
            }
        }

        sessionShard->run(host, "443");
        ioContext.run();

        if(!reconnectingPayload.empty()) {
            sessionShard->asyncQueue(reconnectingPayload.dump());
        }

        while (sessionShard->is_socket_open()) {
            ioContext.restart();
            sessionShard->enable_async_read();

            const std::string wsResponse = sessionShard->getResponse();
            std::cout << wsResponse << std::endl;
            this->parseResponse(sessionShard, wsResponse);

            ioContext.run();
        }

        // After this point, the connection is closed and cannot be reopened without creating a new session
        if(!ioContext.stopped()) ioContext.stop();

        for (auto& shard : this->shardClass) {
            if (shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                shard->shardStructPtr->closeCode = sessionShard->getCloseCode();
                shard->shardStructPtr->closeReason = sessionShard->getCloseReason();
                shard->shardStructPtr->exit.set_value(true);
                stopHeartbeatCycle();

                this->updateCondition.notify_one();
                break;
            }
        }
    }

    void client::parseResponse(const std::shared_ptr<session>& sessionShard, const std::string& response)
    {
        const json wsResponseJson = json::parse(response);
        const int opCode = wsResponseJson["op"];
        std::string event;

        switch(opCode) {
            case 0: {
                // Case 0 is for sending and receiving events
                event = wsResponseJson["t"];
                for (auto& shard : this->shardClass) {
                    if (shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                        shard->shardStructPtr->seq++;
                    }
                }
                break;
            }
            case 1: {
                sendHeartbeat(sessionShard);
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
                cache_->put("heartbeat_interval", wsResponseJson["d"]["heartbeat_interval"].dump());
                if(wsResponseJson["s"].is_null()) {
                    cache_->put("s", "0");
                } else {
                    cache_->put("s", wsResponseJson["s"].dump());
                }
                event = "ON_ENABLE";
                break;
            }
            case 11: {
                // Case 11 is sent on receiving heartbeats
                for (auto& shard : this->shardClass) {
                    if (shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                        shard->shardStructPtr->receivedHeartbeat = true;
                    }
                }
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
                startHeartbeatCycle(sessionShard);
                for (auto& shard : this->shardClass) {
                    if(shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                        if(!shard->shardStructPtr->reconnect) {
                            json identifyPayload = getIdentifyPayload(shard->shardStructPtr->shardId);
                            sessionShard->asyncQueue(identifyPayload.dump());
                        }
                    shard->shardStructPtr->reconnect = false;
                    break;
                    }
                }
                return;
            }

            if(executeEvent == "READY") {
                eventData readyEventData = eventData::readyEventData(wsResponseJson);
                this->onEvent.readyFunction(readyEventData);

                for (auto& shard : this->shardClass) {
                    if(shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                        const std::string resumeGatewayUrl = wsResponseJson["d"]["resume_gateway_url"];
                        shard->shardStructPtr->resumeUrl = resumeGatewayUrl.substr(6, resumeGatewayUrl.length() - 6);
                        shard->shardStructPtr->sessionId = wsResponseJson["d"]["session_id"];
                    }
                }

                /*
                        bool hasGuildMemberIntents = false;
                        std::string binaryIntents;
                        int intentsInt = 14;

                        while(intentsInt != 0) {
                            if (intentsInt % 2 == 0) {
                                binaryIntents += "0";
                            } else {
                                binaryIntents += "1";
                            }
                            intentsInt /= 2;
                        }
                        return;
                         */
            }

            if(executeEvent == "RESUMED") {

            }
            if(executeEvent == "RECONNECT") {
                for (auto& shard : this->shardClass) {
                    if (shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                        shard->shardStructPtr->reconnect = true;
                        sessionShard->asyncCloseSession();
                    }
                }
                return;
            }

            if(executeEvent == "DISCONNECT") {
                for (auto& shard : this->shardClass) {
                    if (shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
                        shard->shardStructPtr->fullReconnect = true;
                        sessionShard->asyncCloseSession();
                    }
                }
                return;
            }
        }
    }

    void client::startHeartbeatCycle(const std::shared_ptr<session>& sessionShard) {
        for (auto& shard : this->shardClass) {
            // Find and update shardVector with heartbeat thread
            if(shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {

                std::unique_ptr<std::thread> heartbeatThread;
                heartbeatThread = std::make_unique<std::thread>(&client::heartbeatCycle, this, sessionShard);
                shard->shardStructPtr->heartbeatThread.swap(heartbeatThread);

                shard->shardStructPtr->heartbeatThreadId = shard->shardStructPtr->heartbeatThread->get_id();
                break;
            }

            if(shard == this->shardClass.back()) {
                std::stringstream targetShardAddress;
                targetShardAddress << shard->shardStructPtr->shardThreadId;

                const std::string error = "Cannot find thread " + targetShardAddress.str();
                throw(heliosException(82, error));
            }
        }
    }

    void client::stopHeartbeatCycle() {
        for (auto& shard : this->shardClass) {
            if(shard->shardStructPtr->heartbeatThreadId == std::thread::id()) return;
            if(shard->shardStructPtr->shardThreadId == std::this_thread::get_id()) {
            shard->shardStructPtr->exitHeartbeat.set_value(true);
            if(shard->shardStructPtr->heartbeatThread->joinable()) {
                shard->shardStructPtr->heartbeatThread->join();
            }
                break;
            }
        }
    }

    void client::sendHeartbeat(const std::shared_ptr<session>& sessionShard)
    {
        json heartbeatPayload;
        heartbeatPayload["op"] = 1;
        heartbeatPayload["d"] = this->cache_->get("s");

        sessionShard->asyncQueue(heartbeatPayload.dump());
    }

    void client::heartbeatCycle(const std::shared_ptr<session>& sessionShard)
    {
        if(!cache_->exist("heartbeat_interval")) {
            throw(heliosException(60, "Failed to start heartbeat. No heartbeat_interval found"));
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        bool foundHeartbeatThread = false;
        double randomNumber = dis(gen);
        double jitter = randomNumber * std::stod(cache_->get("heartbeat_interval"));
        for (auto& shard : this->shardClass) {
            if (shard->shardStructPtr->heartbeatThreadId == std::this_thread::get_id()) {
                foundHeartbeatThread = true;
                if (shard->shardStructPtr->exitHeartbeatFuture.wait_for(std::chrono::milliseconds((int)jitter)) == std::future_status::ready) {
                    break;
                }
                while (true) {
                    this->sendHeartbeat(sessionShard);

                    // If program receivedHeartbeat is false, program never received ack and must restart
                    if(!shard->shardStructPtr->receivedHeartbeat) {
                        sessionShard->asyncCloseSession(websocket::close_code::abnormal);
                        break;
                    }

                    shard->shardStructPtr->receivedHeartbeat = false;
                    if (shard->shardStructPtr->exitHeartbeatFuture.wait_for(std::chrono::milliseconds(stoi(cache_->get("heartbeat_interval")))) == std::future_status::ready) {
                        break;
                    }
                }
            }

            if(foundHeartbeatThread) {
                break;
            }

            if(shard == this->shardClass.back()) {
                std::stringstream targetShardAddress;
                targetShardAddress << shard->shardStructPtr->shardThreadId;

                const std::string error = "Cannot find thread " + targetShardAddress.str();
                throw(heliosException(82, error));
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

    [[maybe_unused]] void onEvent::ready(const std::function<void(eventData)>& userFunction) {
        this->readyFunction = userFunction;
    }

} // helios

