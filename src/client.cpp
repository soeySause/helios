#include "client.hpp"

namespace helios {
    client::client() {
        std::cout << "Client initialized" << std::endl;
        this->cache_ = std::make_shared<cache>();
    }

    void client::run(const std::string& token)
    {
        std::cout << "Starting bot " << std::endl;
        cache_->put("token", token);
        std::cerr << 0;

        const int getGatewayResponseCode = apiRequest::getGateway(cache_);
        if(getGatewayResponseCode != 200) {
            std::cerr << "Error getting gateway url" << std::endl;
            std::cerr << "Response code " << std::to_string(getGatewayResponseCode) << std::endl;
            exit(EXIT_FAILURE);
        }

        load_root_certificates(this->sslContext);
        this->p = std::make_shared<session>(this->ioContext, this->sslContext, cache_);
        if(cache_->exist("url"))
        {
            // Splice url to make usable
            std::string url = cache_->get("url");
            std::string host = url.substr(6, url.length() - 6);

            //if(connectionType == "resume_gateway_url")
            //{
            //    this->ioContext.reset();
            //    this->p->asyncQueue(bot::websocket::sendEvents::resumeSessionPayload(this->cache_));
            //}

            p->run(host, "443", "");
            //thread heartbeatThread(&client::heartbeatCycle, this);

            this->ioContext.run();


            std::string response;
            while (this->p->is_socket_open()) {
                this->ioContext.restart();
                this->p->enable_async_read();

                response = this->p->getResponse();
                std::cout << response << std::endl;
                //client::parseResponse(response);

                this->ioContext.run();
            }

            //heartbeatThread.join();
            //this->event_->reconnect();

        } else
        {
            std::cerr << "Could not find a url in cache to connect to!" << std::endl;
            exit(EXIT_FAILURE);
        }
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

    void client::parseResponse(const string& wsResponse)
    {
        json responseJson_ = json::parse(wsResponse);
        if (responseJson_.contains("op")) {
            string event = cacheOpCodes(cache_, responseJson_.at("op"), responseJson_);
            if (!event.empty()) {
                client::executeEvent(event, wsResponse);
            }
        }
    }

    void client::sendWsMessage(const string& message, bool synchronous)
    {
        this->p->asyncQueue(message, synchronous);
    }

    void client::endWsConnection()
    {
        this->p->closeSession();
    }
     }*/
} // helios