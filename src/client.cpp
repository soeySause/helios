#include "client.hpp"

namespace helios {
    client::client() {
        std::cout << "Client initialized" << std::endl;
        this->cache_ = std::make_shared<cache>();
    }

    [[noreturn]] void client::run(const std::string& token)
    {
        if(!cache_->exist("token")){
            if(token.empty()) {
                std::cerr << "No token provided" << std::endl;
                exit(EXIT_FAILURE);
            } else {
                cache_->put("token", token);
            }
        }

        const int getGatewayResponseCode = apiRequest::getGateway(cache_);
        if(getGatewayResponseCode != 200) {
            std::cerr << "Error getting gateway url" << std::endl;
            std::cerr << "Response code " << std::to_string(getGatewayResponseCode) << std::endl;
            exit(EXIT_FAILURE);
        }

        if(!cache_->exist("url")) {
            std::cerr << "Could not find a url in cache to connect to!" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::cout << "Attempting to start bot" << std::endl;
        std::cout << "Creating " << this->cache_->get("shards") << " shards" << std::endl;
        load_root_certificates(this->sslContext);

        // Splice url to make usable
        std::string host = cache_->get("url").substr(6, cache_->get("url").length() - 6);

        while(true) {
            if(reconnecting) {
                reconnecting = false;
                std::string host = cache_->get("url").substr(6, cache_->get("url").length() - 6);
            }
            for(int shard = 0; shard < std::stoi(this->cache_->get("shards")); shard++) {
                this->shardedConnections.emplace_back(&client::createWsShard, this, shard, host);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            for(auto& thread : this->shardedConnections) {
                if(thread.joinable()) {
                    thread.join();
                }
            }
        }
    }

    void client::createWsShard(const int& shard, const std::string& host) {
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
        this->heartbeatThread.emplace_back(&client::heartbeatCycle, this, sessionShard);
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
                    if(cache_->get("receivedHeartbeat") == "false")
                        std::cout << "No response received from heartbeat! Attempting to resend heartbeat";
                    else
                        std::this_thread::sleep_for(std::chrono::milliseconds(std::stoi(this->cache_->get("heartbeat_interval")) - waitForMsDelay));
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

        identifyPayload["d"]["properties"]["os"] = this->properties.getOs();
        identifyPayload["d"]["properties"]["browser"] = this->properties.getBrowser();
        identifyPayload["d"]["properties"]["device"] = this->properties.getDevice();

        identifyPayload["d"]["compress"] = this->compress;
        identifyPayload["d"]["large_threshold"] = this->getLargeThreshold();

        json jsonShards = json::array();
        jsonShards = {shard, std::stoi(this->cache_->get("shards"))};
        identifyPayload["d"]["shard"] = jsonShards;

        json jsonActivitiesArray = json::array();
        json jsonActivitiesObject;
        if(!this->presence.activities.getName().empty()) {
            jsonActivitiesObject["name"] = this->presence.activities.getName();
            jsonActivitiesObject["type"] = this->presence.activities.getType();
        }
        if(!this->presence.activities.getUrl().empty()) {
            jsonActivitiesObject["url"] = this->presence.activities.getUrl();
        }

        jsonActivitiesArray.push_back(jsonActivitiesObject);
        identifyPayload["d"]["presence"]["activities"] = jsonActivitiesArray;

        identifyPayload["d"]["presence"]["status"] = this->presence.getStatus();
        identifyPayload["d"]["presence"]["since"] = this->presence.getSince();
        identifyPayload["d"]["presence"]["afk"] = this->presence.getAfk();

        identifyPayload["d"]["intents"] = this->getIntents();

        return identifyPayload;
    }


    void properties::setOs(const std::string &os) {
        this->os = os;
    }

    std::string properties::getOs() const {
        return this->os;
    }

    void properties::setBrowser(const std::string &browser) {
        this->browser = browser;
    }

    std::string properties::getBrowser() const {
        return this->browser;
    }

    void properties::setDevice(const std::string &device) {
        this->device = device;
    }

    std::string properties::getDevice() const {
        return this->device;
    }

    void activities::setName(const std::string &name) {
        this->name = name;
    }

    std::string activities::getName() const {
        return this->name;
    }

    void activities::setType(const int& type) {
        this->type = type;
    }

    int activities::getType() const {
        return this->type;
    }

    void activities::setUrl(const std::string& url){
        this->url = url;
    }

    std::string activities::getUrl() const {
        return this->url;
    }

    void presence::setSince(const int& since) {
        this->since = since;
    }

    int presence::getSince() const {
        return this->since;
    }

    void presence::setStatus(const std::string& status) {
        this->status = status;
    }

    std::string presence::getStatus() const {
        return this->status;
    }
    void presence::setAfk(const bool& afk) {
        this->afk = afk;
    }

    bool presence::getAfk() const {
        return this->afk;
    }

    void client::setToken(const std::string& token) {
        this->cache_->put("token", token);
    }

    void client::setLargeThreshold(const int threshold) {
        this->large_threshold = threshold;
    }

    int client::getLargeThreshold() const {
        return this->large_threshold;
    }
    void client::setShards(const int shards) {
        this->cache_->put("shards", std::to_string(shards));
    }

    //TODO calculate intents automatically depending on what events they have;
    void client::setIntents(const int intents) {
        this->intents = intents;
    }

    int client::getIntents() const {
        return this->intents;
    }


} // helios
