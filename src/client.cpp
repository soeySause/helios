#include "client.hpp"

namespace helios {
    client::client() {
        std::cout << "Client initialized" << std::endl;
        this->cache_ = std::make_shared<cache>();
    }

    void client::run(const std::string& token)
    {
        if(!cache_->exist("token")){
            if(token.empty()) {
                std::cerr << "No token proviced" << std::endl;
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


        std::cout << "Attempting to connect to websocket" << std::endl;

        load_root_certificates(this->sslContext);
        this->p = std::make_shared<session>(this->ioContext, this->sslContext, cache_);
        if(!cache_->exist("url"))
        {
            std::cerr << "Could not find a url in cache to connect to!" << std::endl;
            exit(EXIT_FAILURE);

        }
            // Splice url to make usable
            std::string url = cache_->get("url");
            std::string host = url.substr(6, url.length() - 6);

            //if(connectionType == "resume_gateway_url")
            //{
            //    this->ioContext.reset();
            //    this->p->asyncQueue(bot::websocket::sendEvents::resumeSessionPayload(this->cache_));
            //}

            p->run(host, "443");
            //thread heartbeatThread(&client::heartbeatCycle, this);

            this->ioContext.run();


            while (this->p->is_socket_open()) {
                this->ioContext.restart();
                this->p->enable_async_read();

                const std::string wsResponse = this->p->getResponse();
                std::cout << wsResponse << std::endl;

                const json wsResponseJson = json::parse(wsResponse);
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
                    if(event == "ON_ENABLE"){
                        std::cout << "Attempting to start bot" << std::endl;

                        json identifyPayload;
                        identifyPayload["op"] = 2;
                        identifyPayload["d"]["token"] = token;

                        identifyPayload["d"]["properties"]["os"] = this->properties.getOs();
                        identifyPayload["d"]["properties"]["browser"] = this->properties.getBrowser();
                        identifyPayload["d"]["properties"]["device"] = this->properties.getDevice();

                        identifyPayload["d"]["compress"] = this->compress;
                        identifyPayload["d"]["large_threshold"] = this->getLargeThreshold();

                        json jsonShards = json::array();
                        jsonShards = {0, std::stoi(this->cache_->get("shards"))};
                        identifyPayload["d"]["shard"] = jsonShards;

                        json jsonActivitiesArray = json::array();
                        json jsonActivitiesObject;
                        jsonActivitiesObject["name"] = this->presence.activities.getName();
                        jsonActivitiesObject["type"] = this->presence.activities.getType();
                        jsonActivitiesObject["url"] = this->presence.activities.getUrl();
                        jsonActivitiesArray = jsonActivitiesObject;
                        identifyPayload["d"]["presence"]["activities"] = jsonActivitiesArray;

                        identifyPayload["d"]["presence"]["status"] = this->presence.getStatus();
                        identifyPayload["d"]["presence"]["since"] = this->presence.getSince();
                        identifyPayload["d"]["presence"]["afk"] = this->presence.getAfk();

                        identifyPayload["d"]["intents"] = this->getIntents();

                        std::cout << identifyPayload.dump() << std::endl;

                        this->p->asyncQueue(identifyPayload.dump());
                    }
                }


                this->ioContext.run();
            }

            //heartbeatThread.join();
            //this->event_->reconnect();
    }

    /*
    void client::sendHeartbeat()
    {
        if(cache_->exist("heartbeat_interval"))
        {
            this->p->asyncQueue(bot::websocket::sendEvents::getHeartbeatEvent(cache_));
        }
    }

    void client::heartbeatCycle()
    {
        cache_->put("receivedHeartbeat", "false");
        if(cache_->exist("heartbeat_interval"))
        {
            const int waitForMsDelay = 500;
            float randomNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float jitter = randomNumber * stof(cache_->get("heartbeat_interval"));
            this_thread::sleep_for(chrono::milliseconds((int)jitter));
            while(this->p->is_socket_open())
            {
                cout << "Sending heartbeat" << endl;
                try
                {
                    this->sendHeartbeat();
                    this_thread::sleep_for(chrono::milliseconds(waitForMsDelay));
                    if(cache_->get("receivedHeartbeat") == "false")
                        cout << "No response received from heartbeat! Attempting to resend heartbeat";
                    else
                        this_thread::sleep_for(chrono::milliseconds(stoi(cache_->get("heartbeat_interval")) - waitForMsDelay));
                } catch (error_code &e) {
                    cerr << "Failed to send heartbeat due to: " << e;
                    cerr << "Attempting to re-send heartbeat in 5 seconds." << endl;
                    this_thread::sleep_for(chrono::seconds(5));
                }
            }
        } else {
            cerr << "Could not find heartbeat_interval in cache! Failed to send heartbeat." << endl;
            cerr << "Attempting to re-send heartbeat in 5 seconds." << endl;
            this_thread::sleep_for(chrono::seconds(5));
            heartbeatCycle();
        }
    }

*/

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
        this->shards = shards;
    }
    int client::getShards() const {
        if(this->shards == -1) {
            return stoi(cache_->get("shards"));
        } else {
            return this->shards;
        }
    }
    //TODO calculate intents automatically depending on what events they have;
    void client::setIntents(const int intents) {
        this->intents = intents;
    }

    int client::getIntents() const {
        return this->intents;
    }


} // helios