#include "client.hpp"

#include <memory>

namespace helios {
    client::client(const std::string& token) {
        this->cache_ = std::make_shared<cache>();
        cache_->put("token", token);

        const int getGatewayResponseCode = apiRequest::getGateway(this->cache_);
        if(getGatewayResponseCode != 200) {
            std::cerr << "Error getting gateway url" << std::endl;
            std::cerr << "Response code " << std::to_string(getGatewayResponseCode) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    [[maybe_unused]] [[noreturn]] void client::run()
    {
        if(!cache_->exist("url")) {
            std::cerr << "Could not find a url in cache to connect to!" << std::endl;
            exit(EXIT_FAILURE);
        }

        load_root_certificates(this->sslContext);
        const std::string host = cache_->get("url").substr(6, cache_->get("url").length() - 6);

        if(!enableSharding) {
            this->startupShards = {0};
        }

        // create and start threads
        for(auto shardId : this->startupShards) {
            this->createWsShard(shardId, host);
        }
        startupShards.clear();

        while(true) {
            std::unique_lock<std::mutex> lock(mutex);
            updateCondition.wait(lock, [&] {
                for (auto& shardReference: shardVector) {
                    if (shardReference->exitFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        switch(shardReference->closeCode) {
                            case 0:
                            case 4000:
                            case 4001:
                            case 4002:
                            case 4003:
                            case 4005:
                            case 4007:
                            case 4008:
                            case 4009:
                            {
                                if(shardReference->shardThread->joinable()) {
                                    shardReference->shardThread->join();
                                }

                                auto shardPositionInVector = std::find_if(this->shardVector.begin(), this->shardVector.end(), [&shardReference](const std::unique_ptr<shard>& p)
                                                       { return p->shardThreadId == shardReference->shardThreadId; });
                                auto oldShardPointer = shardReference.release();

                                if (shardPositionInVector != this->shardVector.end()) {
                                    this->shardVector.erase(shardPositionInVector);
                                    this->createWsShard(oldShardPointer->shardId, oldShardPointer->resumeUrl, true, oldShardPointer->sessionId, oldShardPointer->seq);
                                }
                                delete oldShardPointer;
                                break;
                            }
                            case 4014: {
                                break;
                            }

                            default:
                                std::cerr << "Unknown close code " << std::to_string(shardReference->closeCode) << std::endl;
                                exit(EXIT_FAILURE);
                        }
                        break;
                    }
                }
                return false;
            });
        }
    }

    void client::createWsShard(const int &shardId, const std::string& host, const bool& reconnecting, const std::string& sessionId, const int& seq) {

        std::unique_ptr<shard> shardStruct = std::make_unique<shard>();
        std::unique_ptr<std::thread> newWsShard;

        if(reconnecting) {
            shardStruct->reconnecting = true;
            shardStruct->sessionId = sessionId;
            shardStruct->seq = seq;
        }

        newWsShard = std::make_unique<std::thread>(&client::wsShard, this, host);
        shardStruct->shardThread.swap(newWsShard);

        shardStruct->shardId = shardId;
        shardStruct->shardThreadId = shardStruct->shardThread->get_id();

        this->shardVector.emplace_back(std::move(shardStruct));

    }

    void client::wsShard(const std::string &host) {
        net::io_context ioContext;
        std::shared_ptr<session> sessionShard = std::make_shared<session>(ioContext, this->sslContext, this->cache_);
        json reconnectingPayload;

        for (auto& shard : this->shardVector) {
            if(shard->shardThreadId == std::this_thread::get_id()) {
                shard->running = true;

                if(shard->reconnecting) {
                    reconnectingPayload["op"] = 6;
                    reconnectingPayload["d"]["token"] = this->cache_->get("token");
                    reconnectingPayload["d"]["session_id"] = shard->sessionId;
                    reconnectingPayload["d"]["seq"] = shard->seq;
                }
                break;
            }

            if(shard == this->shardVector.back()) {
                std::cerr << "Could not find thread id ";
                std::cerr << std::this_thread::get_id();
                std::cerr << " in shard list!" << std::endl;
                exit(EXIT_FAILURE);
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

        for (auto& shard : this->shardVector) {
            if (shard->shardThreadId == std::this_thread::get_id()) {
                shard->closeCode = sessionShard->getCloseCode();
                shard->exit.set_value(true);
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
                for (auto& shard : this->shardVector) {
                    if (shard->shardThreadId == std::this_thread::get_id()) {
                        shard->seq++;
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
                for (auto& shard : this->shardVector) {
                    if (shard->shardThreadId == std::this_thread::get_id()) {
                        shard->receivedHeartbeat = true;
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
                for (auto& shard : this->shardVector) {
                    if(shard->shardThreadId == std::this_thread::get_id()) {
                        if(!shard->reconnecting) {
                            json identifyPayload = getIdentifyPayload(shard->shardId);
                            sessionShard->asyncQueue(identifyPayload.dump());
                        }
                    }
                }
                return;
            }

            if(executeEvent == "READY") {
                eventData readyEventData = eventData::readyEventData(wsResponseJson);
                this->onEvent.readyFunction(readyEventData);

                for (auto& shard : this->shardVector) {
                    if(shard->shardThreadId == std::this_thread::get_id()) {
                        const std::string resumeGatewayUrl = wsResponseJson["d"]["resume_gateway_url"];
                        shard->resumeUrl = resumeGatewayUrl.substr(6, resumeGatewayUrl.length() - 6);
                        shard->sessionId = wsResponseJson["d"]["session_id"];
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
                for (auto& shard : this->shardVector) {
                    if (shard->shardThreadId == std::this_thread::get_id()) {
                        shard->reconnecting = true;
                        sessionShard->asyncCloseSession();
                    }
                }
                return;
            }

            if(executeEvent == "DISCONNECT") {
                for (auto& shard : this->shardVector) {
                    if (shard->shardThreadId == std::this_thread::get_id()) {
                        sessionShard->asyncCloseSession();
                    }
                }
                return;
            }
        }
    }

    void client::startHeartbeatCycle(const std::shared_ptr<session>& sessionShard) {
        for (auto& shard : this->shardVector) {
            // Find and update shardVector with heartbeat thread
            if(shard->shardThreadId == std::this_thread::get_id()) {

                std::unique_ptr<std::thread> heartbeatThread;
                heartbeatThread = std::make_unique<std::thread>(&client::heartbeatCycle, this, sessionShard);
                shard->heartbeatThread.swap(heartbeatThread);

                shard->heartbeatThreadId = shard->heartbeatThread->get_id();
                break;
            }

            if(shard == shardVector.back()) {
                std::cerr << "Failed to start heartbeat. Could not find thread id ";
                std::cerr << std::this_thread::get_id();
                std::cerr << "in shard list!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    void client::stopHeartbeatCycle() {
        for (auto& shard : this->shardVector) {
            if(shard->heartbeatThreadId == std::thread::id()) return;
            if(shard->shardThreadId == std::this_thread::get_id()) {
            shard->exitHeartbeat.set_value(true);
            if(shard->heartbeatThread->joinable()) {
                shard->heartbeatThread->join();
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
                std::cerr << "Could not find heartbeat_interval in cache" << std::endl;
                exit(EXIT_FAILURE);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        bool foundHeartbeatThread = false;
        double randomNumber = dis(gen);
        double jitter = randomNumber * std::stod(cache_->get("heartbeat_interval"));
        for (auto& shard : this->shardVector) {
            if (shard->heartbeatThreadId == std::this_thread::get_id()) {
                foundHeartbeatThread = true;
                if (shard->exitHeartbeatFuture.wait_for(std::chrono::milliseconds((int)jitter)) == std::future_status::ready) {
                    break;
                }
                while (true) {
                    this->sendHeartbeat(sessionShard);

                    // If program receivedHeartbeat is false, program never received ack and must restart
                    if(!shard->receivedHeartbeat) {
                        sessionShard->asyncCloseSession(websocket::close_code::abnormal);
                        break;
                    }

                    shard->receivedHeartbeat = false;
                    if (shard->exitHeartbeatFuture.wait_for(std::chrono::milliseconds(stoi(cache_->get("heartbeat_interval")))) == std::future_status::ready) {
                        break;
                    }
                }
            }

            if(foundHeartbeatThread) {
                break;
            }

            if(shard == this->shardVector.back()) {
                std::cerr << "Could not find heartbeat thread ";
                std::cerr << shard->heartbeatThreadId;
                std::cerr << " in shard vector" << std::endl;
                exit(EXIT_FAILURE);
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

