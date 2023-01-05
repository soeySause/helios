#include "client.hpp"

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

        std::cout << "Client initialized" << std::endl;
    }

    [[noreturn]] void client::run()
    {
        running = true;
        if(!cache_->exist("url")) {
            std::cerr << "Could not find a url in cache to connect to!" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Attempting to start bot" << std::endl;
        load_root_certificates(this->sslContext);

        // Splice url to make usable
        std::string host = cache_->get("url").substr(6, cache_->get("url").length() - 6);
        cache_->put("host", host);

        if(enableSharding) {
            for(auto shardId : shardIdVector) {
                this->createWsShard(shardId, host);
            }

            for(auto& thread : this->shardedConnections) {
                if(thread.joinable()) {
                    thread.join();
                }
            }
        } else {
            wsShard(-1, host);
        }
    }

    void client::createWsShard(const int &shardId, const std::string& host) {
        this->shardedConnections.emplace_back(&client::wsShard, this, shardId, host);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void client::createWsShardPool(const std::string& host, const int& delay) {
        for(int shard = 0; shard < std::stoi(this->cache_->get("shards")); shard++) {
            createWsShard(shard, host);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }


    void client::wsShard(const int &shard, const std::string &host) {
        net::io_context ioContext;
        std::shared_ptr<session> shardSession;

        shardedSessions.emplace_back(shardSession);
        shardSession = std::make_shared<session>(ioContext, this->sslContext, cache_);
        shardSession->run(host, "443");

        ioContext.run();
        startHeartbeatCycle(shardSession);

        //if(this->reconnecting) {
        //    this->p->asyncQueue(/*resume session payload*/" ");
        //}

        while (shardSession->is_socket_open()) {
            ioContext.restart();
            shardSession->enable_async_read();

            const std::string wsResponse = shardSession->getResponse();
            std::cout << wsResponse << std::endl;
            this->parseResponse(shardSession, wsResponse, shard);

            ioContext.run();
        }

        ioContext.stop();
    }
/*
    void client::setShardHandler(const shardedClient &shardedClient) {
        this->shardHandler_ = shardedClient;
    }

    void client::updateShardHandler(const helios::shardedClient &shardedClient) {
        this->shardHandler_ = shardedClient;
    }
*/
    void client::parseResponse(const std::shared_ptr<session>& sessionShard, const std::string& response, const int& shard)
    {
        const json wsResponseJson = json::parse(response);
        const int opCode = wsResponseJson["op"];
        std::string event;

        switch(opCode) {
            case 0: {
                // Case 0 is for sending and receiving events
                event = wsResponseJson["t"];
                cache_->put("s", wsResponseJson["s"].dump());
                break;
            }
            case 1: {
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
                // Case 11 is sent on heartbeats
                if(cache_->exist("receivedHeartbeat")) {
                    cache_->put("receivedHeartbeat", "true");
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
                json identifyPayload = getIdentifyPayload(shard);
                sessionShard->asyncQueue(identifyPayload.dump());
                return;
            }

            if(executeEvent == "READY") {
                if(shard > stoi(this->cache_->get("shards")) - 1) {
                    std::cerr << "Created shard " << std::to_string(shard) << " but only " << std::to_string(stoi(this->cache_->get("shards")) - 1) << " should exist" << std::endl;
                    exit(EXIT_FAILURE);
                }

                //this->cache_->put("[" + std::to_string(shard) + ", " + this->cache_->get("shards") + "]|");

                if(stoi(this->cache_->get("shards")) > 1) {
                    std::cout << "Shard " << shard << " is online" << std::endl;
                } else {
                    std::cout << "Bot is online" << std::endl;
                }


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
            }
            if(executeEvent == "RECONNECT") {
                reconnecting = true;
                return;
            }

            if(executeEvent == "DISCONNECT") {
                return;
            }
        }
    }

    void client::startHeartbeatCycle(const std::shared_ptr<session>& sessionShard) {
        this->heartbeatThreads.emplace_back(&client::heartbeatCycle, this, sessionShard);
    }

    void client::stopHeartbeatCycle() {
        for(auto& thread : this->shardedConnections) {
            if(thread.joinable()) {
                thread.join();
            }
        }
    }

    void client::sendHeartbeat(const std::shared_ptr<session>& sessionShard)
    {
        if(cache_->exist("heartbeat_interval"))
        {
            json heartbeatPayload;
            heartbeatPayload["op"] = 1;
            heartbeatPayload["d"] = this->cache_->get("s");

            sessionShard->asyncQueue(heartbeatPayload.dump());
        }
    }

    void client::heartbeatCycle(const std::shared_ptr<session>& sessionShard)
    {
        cache_->put("receivedHeartbeat", "false");
        int connectionAttempts = 0;
        if(!cache_->exist("heartbeat_interval")) {
            connectionAttempts++;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if(connectionAttempts > 45) {
                std::cerr << "Could not find heartbeat_interval in cache" << std::endl;
                exit(EXIT_FAILURE);
            } else {
                heartbeatCycle(sessionShard);
            }
        } else {
            const int waitForMsDelay = 500;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(0, 1);

            double randomNumber = dis(gen);
            double jitter = randomNumber * std::stod(cache_->get("heartbeat_interval"));
            std::this_thread::sleep_for(std::chrono::milliseconds((int)jitter));

            while(sessionShard->is_socket_open()) {
                std::cout << "Sending heartbeat" << std::endl;
                try {
                    this->sendHeartbeat(sessionShard);
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitForMsDelay));
                    if(cache_->get("receivedHeartbeat") == "false") {
                        std::cout << "No response received from heartbeat! Attempting to resend heartbeat";
                    } else {
                        std::this_thread::sleep_for(std::chrono::milliseconds(
                                std::stoi(this->cache_->get("heartbeat_interval")) - waitForMsDelay));
                    }

                } catch (std::error_code &e) {
                    std::cerr << "Failed to send heartbeat due to: " << e;
                    std::cerr << "Attempting to re-send heartbeat in 5 seconds." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
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

    void properties::setOs(const std::string &os) {
        this->os = os;
    }

    void properties::setBrowser(const std::string &browser) {
        this->browser = browser;
    }

    void properties::setDevice(const std::string &device) {
        this->device = device;
    }

    void activities::setName(const std::string &name) {
        this->name = name;
    }

    void activities::setType(const int& type) {
        this->type = type;
    }

    void activities::setUrl(const std::string& url){
        this->url = url;
    }

    void presence::setSince(const int& since) {
        this->since = since;
    }

    void presence::setStatus(const std::string& status) {
        this->status = status;
    }

    void presence::setAfk(const bool& afk) {
        this->afk = afk;
    }

    void client::setLargeThreshold(const int threshold) {
        this->large_threshold = threshold;
    }

} // helios
