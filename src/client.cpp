#include "client.hpp"

namespace helios {
    client::client(const std::string& token) {
        this->rateLimit = std::make_shared<rateLimitStruct>();
        this->rateLimit->rateLimitGlobal = std::make_unique<rateLimitGlobal>(50);

        this->threadPool = std::make_shared<helios::threadPool>(1);
        std::promise<json> getGatewayPromise;
        httpRequest getGatewayReq("discord.com", "/api/gateway/bot", {}, boost::beast::http::verb::get, this->rateLimit, token, [&getGatewayPromise](const http::response<http::string_body>& res_)  {
            getGatewayPromise.set_value(json::parse(res_.body()));
        });
        this->threadPool->queueHttpRequest(getGatewayReq);

        cache::createCacheDirectory();
        this->botToken = token;
        this->applicationRoleConnectionMetadata.client = this;
        this->channels.client = this;
        this->guilds.client = this;
        load_root_certificates(this->sslContext);

        json getGateway = getGatewayPromise.get_future().get();
        this->shards = getGateway["shards"];
        this->maxConcurrency = getGateway["session_start_limit"]["max_concurrency"];
        this->host = getGateway["url"].get<std::string>().substr(6, getGateway["url"].get<std::string>().length() - 6);
        this->threadPool->resize(this->shards);

    }
    client::client(const std::string& token, const std::map<std::string, int>& options) {
        load_root_certificates(this->sslContext);

        this->threadPool = std::make_shared<helios::threadPool>(1);
        std::promise<json> getGatewayPromise;
        httpRequest getGatewayReq("discord.com", "/api/gateway/bot", {}, boost::beast::http::verb::get, this->rateLimit, token, [&getGatewayPromise](const http::response<http::string_body>& res_)  {
            getGatewayPromise.set_value(json::parse(res_.body()));
        });
        this->threadPool->queueHttpRequest(getGatewayReq);


        cache::createCacheDirectory();
        this->botToken = token;
        this->applicationRoleConnectionMetadata.client = this;
        this->channels.client = this;
        this->guilds.client = this;

        int globalRateLimit = 50;
        for(auto& option : options) {
            if(option.first == "globalRateLimit") globalRateLimit = option.second;
        }

        this->rateLimit = std::make_shared<rateLimitStruct>();
        this->rateLimit->rateLimitGlobal = std::make_unique<rateLimitGlobal>(globalRateLimit);

        json getGateway = getGatewayPromise.get_future().get();

        this->shards = getGateway["shards"];
        this->maxConcurrency = getGateway["session_start_limit"]["max_concurrency"];
        this->host = getGateway["url"].get<std::string>().substr(6, getGateway["url"].get<std::string>().length() - 6);
        this->threadPool->resize(this->shards);
    }
    [[maybe_unused]] [[noreturn]] void client::run() {
        if(!enableSharding) {
            std::shared_ptr<shard> newShard = std::make_shared<shard>();
            newShard->shardStructPtr->shardId = 0;

            this->shardClass.emplace_back(newShard);
        }

        if(this->shardClass.empty()) {
            throw(helios::heliosException(10, "No shards defined"));
        }

        std::unique_lock<std::mutex> lock(mutex);
        for(auto& shard : this->shardClass) {
            shard->shardStructPtr->running = true;
            this->createWsShard(shard);
        }

        while(true) {
            updateCondition.wait(lock, [&] {
                if(endSession) exit(0);
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
        std::unique_ptr<std::thread> newWsShard;
        if(shard->shardStructPtr->reconnect) {
            newWsShard = std::make_unique<std::thread>(&client::wsShard, this, shard->shardStructPtr->resumeUrl, shard);
            shard->shardStructPtr->shardThread.swap(newWsShard);
            shard->shardStructPtr->shardThreadId = shard->shardStructPtr->shardThread->get_id();
            return;
        }

        if(this->shardCreationTime.size() >= this->maxConcurrency) {
            const auto timeSinceLastShard = std::chrono::system_clock::now() - shardCreationTime.back();
            if (timeSinceLastShard < std::chrono::seconds(5)) {
                std::this_thread::sleep_for(std::chrono::seconds(5) - timeSinceLastShard);
            }
            this->shardCreationTime.pop_back();
        }

        this->shardCreationTime.insert(this->shardCreationTime.begin(), std::chrono::system_clock::now());
        newWsShard = std::make_unique<std::thread>(&client::wsShard, this, this->host, shard);
        shard->shardStructPtr->shardThread.swap(newWsShard);

        shard->shardStructPtr->shardThreadId = shard->shardStructPtr->shardThread->get_id();
        this->threadMap[shard->shardStructPtr->shardThreadId] = shard->shardStructPtr->shardId;
    }

    void client::wsShard(const std::string &connectedHost, const std::shared_ptr<shard>& shard) {
        shard->shardStructPtr->sessionShard = std::make_shared<session>(shard->shardStructPtr->ioContext, this->sslContext, shard,
            [this](auto && PH1, auto && PH2, auto && PH3, auto && PH4) { onReceiveData(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), std::forward<decltype(PH3)>(PH3), std::forward<decltype(PH4)>(PH4)); },
            [this](auto && PH1, auto && PH2, auto && PH3) { onSessionClose(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), std::forward<decltype(PH3)>(PH3)); });

        shard->shardStructPtr->sessionShard->run(connectedHost, "443");

        if(shard->shardStructPtr->reconnect) {
            json reconnectingPayload;
            reconnectingPayload["op"] = 6;
            reconnectingPayload["d"]["token"] = this->botToken;
            reconnectingPayload["d"]["session_id"] = shard->shardStructPtr->sessionId;
            reconnectingPayload["d"]["seq"] = shard->shardStructPtr->seq;
            client::sendGatewayMessage(shard, reconnectingPayload.dump());
        }

        shard->shardStructPtr->ioContext.run();
    }
    void client::onReceiveData(const boost::system::error_code &ec, std::size_t bytes_transferred, const std::shared_ptr<shard>& shard, const std::string &data) {
        boost::ignore_unused(bytes_transferred);
        if (ec) return fail(ec, "response");
        std::cout << data << std::endl;
        this->parseResponse(shard, data);
    }
    void client::onSessionClose(const boost::system::error_code &ec, std::size_t bytes_transferred, const std::shared_ptr<shard> &shard) {
        boost::ignore_unused(bytes_transferred);
        shard->shardStructPtr->closeCode = shard->shardStructPtr->sessionShard->getCloseCode();
        shard->shardStructPtr->closeReason = shard->shardStructPtr->sessionShard->getCloseReason();
        shard->shardStructPtr->exit.set_value(true);
        shard->shardStructPtr->sessionShard.reset();
        shard->shardStructPtr->ioContext.stop();
        stopHeartbeatCycle(shard);

        this->updateCondition.notify_one();
    }

    void client::parseResponse(const std::shared_ptr<shard>& shard, const std::string& response)
    {
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
                json heartbeatPayload;
                heartbeatPayload["op"] = 1;
                heartbeatPayload["d"] = shard->shardStructPtr->seq;
                client::sendGatewayMessage(shard, heartbeatPayload.dump());
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
                    client::sendGatewayMessage(shard, identifyPayload.dump());
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
                shard->shardStructPtr->sessionShard->asyncCloseSession();
                return;
            }

            if(executeEvent == "DISCONNECT") {
                shard->shardStructPtr->fullReconnect = true;
                shard->shardStructPtr->sessionShard->asyncCloseSession();
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
                return;
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

    bool client::sendGatewayMessage(const std::shared_ptr<shard>& shard, const std::string& payload) {
        if(!shard->shardStructPtr->sentMessageTimestamp.empty()) {
            while (static_cast<long int>(shard->shardStructPtr->sentMessageTimestamp.back()) >
                   static_cast<long int>(shard->shardStructPtr->sentMessageTimestamp.front()) + 60) {
                    shard->shardStructPtr->sentMessageTimestamp.pop_back();
            }
        }
        if(shard->shardStructPtr->sentMessageTimestamp.size() >= 120) {
            std::cerr << "Failed to send gateway message to connection due to rate limit" << std::endl;
            return false;
        }

        shard->shardStructPtr->sentMessageTimestamp.emplace_back(std::time(nullptr));
        shard->shardStructPtr->sessionShard->asyncQueue(payload);
        return true;
    }

    void client::startHeartbeatCycle(const std::shared_ptr<shard>& shard) {
        if(!shard->shardStructPtr->heartbeatInterval.has_value()) {
            throw(heliosException(60, "Failed to start heartbeat. No heartbeat interval found"));
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0, 1);

        double randomNumber = dis(gen);
        double jitter = randomNumber * shard->shardStructPtr->heartbeatInterval.value();
        shard->shardStructPtr->heartbeatIntervalTimer = std::make_unique<boost::asio::steady_timer>(shard->shardStructPtr->ioContext.get_executor());
        shard->shardStructPtr->strand = std::make_unique<boost::asio::strand<boost::asio::io_context::executor_type>>(shard->shardStructPtr->ioContext.get_executor());

        shard->shardStructPtr->heartbeatIntervalTimer->expires_from_now(std::chrono::milliseconds((int)jitter));
        shard->shardStructPtr->heartbeatCycle = std::make_unique<std::function<void(const boost::system::error_code& ec)>>([shard](const boost::system::error_code& ec) {
            if (ec == boost::asio::error::operation_aborted) {
                return;
            }
            if (ec) throw(heliosException(ec.value(), ec.what()));

            json heartbeatPayload;
            heartbeatPayload["op"] = 1;
            heartbeatPayload["d"] = shard->shardStructPtr->seq;

            client::sendGatewayMessage(shard, heartbeatPayload.dump());

            if (!shard->shardStructPtr->receivedHeartbeat) {
                shard->shardStructPtr->sessionShard->asyncCloseSession(websocket::close_code::abnormal);
                return;
            }

            shard->shardStructPtr->receivedHeartbeat = false;
            shard->shardStructPtr->heartbeatIntervalTimer->expires_at(shard->shardStructPtr->heartbeatIntervalTimer->expiry() + std::chrono::milliseconds(shard->shardStructPtr->heartbeatInterval.value()));
            shard->shardStructPtr->heartbeatIntervalTimer->async_wait(boost::asio::bind_executor(*shard->shardStructPtr->strand, *shard->shardStructPtr->heartbeatCycle));
        });
        shard->shardStructPtr->heartbeatIntervalTimer->async_wait(boost::asio::bind_executor(*shard->shardStructPtr->strand, *shard->shardStructPtr->heartbeatCycle));
    }

    void client::stopHeartbeatCycle(const std::shared_ptr<shard>& shard) {
        shard->shardStructPtr->heartbeatIntervalTimer->cancel();
    }

    json client::getIdentifyPayload(const int& shard) {
        json identifyPayload;
        identifyPayload["op"] = 2;
        identifyPayload["d"]["token"] = this->botToken;

        identifyPayload["d"]["properties"]["os"] = this->properties.os;
        identifyPayload["d"]["properties"]["browser"] = this->properties.browser;
        identifyPayload["d"]["properties"]["device"] = this->properties.device;

        identifyPayload["d"]["compress"] = this->compress;
        identifyPayload["d"]["large_threshold"] = this->large_threshold;

        if(enableSharding) {
            json jsonShards = json::array();
            jsonShards = {shard, this->shards};
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

    void client::reconnect() {
        this->shardClass.at(0)->shardStructPtr->reconnect = true;
        this->shardClass.at(0)->shardStructPtr->sessionShard->asyncCloseSession();
        return;
    }

    void client::disconnect() {
        auto closeCode = static_cast<websocket::close_code>(1000);
        this->endSession = true;
        this->shardClass.at(0)->shardStructPtr->fullReconnect = true;
        this->shardClass.at(0)->shardStructPtr->sessionShard->asyncCloseSession(closeCode);
        return;
    }

    [[maybe_unused]] void properties::setOs(const std::string &userInputOs) {
        this->os = userInputOs;
    }

    [[maybe_unused]] void properties::setBrowser(const std::string &userInputBrowser) {
        this->browser = userInputBrowser;
    }

    [[maybe_unused]] void properties::setDevice(const std::string &userInputDevice) {
        this->device = userInputDevice;
    }

    [[maybe_unused]] void activities::setName(const std::string &userInputName) {
        this->name = userInputName;
    }

    [[maybe_unused]] void activities::setType(const int& userInputType) {
        this->type = userInputType;
    }

    [[maybe_unused]] void activities::setUrl(const std::string& userInputUrl){
        this->url = userInputUrl;
    }

    [[maybe_unused]] void presence::setSince(const int& userInputSince) {
        this->since = userInputSince;
    }

    [[maybe_unused]] void presence::setStatus(const std::string& userInputStatus) {
        this->status = userInputStatus;
    }

    [[maybe_unused]] void presence::setAfk(const bool& userInputAfk) {
        this->afk = userInputAfk;
    }

    [[maybe_unused]] void client::setLargeThreshold(const int userInputThreshold) {
        this->large_threshold = userInputThreshold;
    }

/*
    applicationRoleConnectionMetadata
    client::applicationRoleConnectionMetadataOptions::getApplicationRoleConnectionMetadataRecords(
            const long &applicationId) {
        return helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData
                (nlohmann::json::parse(request::httpsRequest("discord.com", "/api/applications/" + std::to_string(applicationId) + "/role-connections/metadata", "", boost::beast::http::verb::get, client->rateLimit, client->botToken)));

    }

    applicationRoleConnectionMetadata
    client::applicationRoleConnectionMetadataOptions::updateApplicationRoleConnectionMetadataRecords(
            const long &applicationId) {
        return helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData
                (nlohmann::json::parse(request::httpsRequest("discord.com", "/api/applications/" + std::to_string(applicationId) + "/role-connections/metadata", "", boost::beast::http::verb::put, client->rateLimit, client->botToken)));
    }
*/
    std::future<channel> client::channelOptions::get(const long &channelId, const bool &cacheObject) const {
        std::promise<channel> channelPromise;
        httpRequest getChannelReq("discord.com", "/api/channels/" + std::to_string(channelId), {}, boost::beast::http::verb::get, client->rateLimit, client->botToken, [&channelPromise, this](const http::response<http::string_body>& res_)  {
            channel reqChannel = channel::getChannelData(json::parse(res_.body()));
            reqChannel.botToken = client->botToken;
            reqChannel.shard = client->threadMap[std::this_thread::get_id()];
            reqChannel.rateLimit = client->rateLimit;
            channelPromise.set_value(reqChannel);
        });
        //request::sendHttpRequest(getChannelReq);
        return channelPromise.get_future();
    }
/*
    guild client::guildOptions::createGuild(const helios::guild &guildOptions, const std::vector<role> &roles, const std::vector<channel> &channels) {
        json createGuildPayload;
        if(guildOptions.name.has_value()) createGuildPayload["name"] = guildOptions.name.value();
        if(guildOptions.icon.has_value()) createGuildPayload["icon"] = guildOptions.icon.value();
        if(guildOptions.verification_level.has_value()) createGuildPayload["verification_level"] = guildOptions.verification_level.value();
        if(guildOptions.defaultMessageNotifications.has_value()) createGuildPayload["default_message_notifications"] = guildOptions.defaultMessageNotifications.value();
        if(guildOptions.explicitContentFilter.has_value()) createGuildPayload["explicit_content_filter"] = guildOptions.explicitContentFilter.value();

        json createGuildRoles = json::array();
        for(auto& role : roles) {
            json roleJson;
            if(role.name.has_value()) roleJson["name"] = role.name.value();
            if(role.permissions.has_value()) roleJson["permissions"] = role.permissions.value();
            if(role.color.has_value()) roleJson["color"] = role.color.value();
            if(role.hoist.has_value()) roleJson["hoist"] = role.hoist.value();
            if(role.icon.has_value()) roleJson["icon"] = role.icon.value();
            if(role.unicodeEmoji.has_value()) roleJson["unicode_emoji"] = role.unicodeEmoji.value();
            if(role.mentionable.has_value()) roleJson["mentionable"] = role.mentionable.value();
            createGuildRoles.emplace_back(roleJson);
        }
        if(!createGuildRoles.empty()) createGuildPayload["roles"] = createGuildRoles;

        json createGuildChannel = json::array();
        for(auto& channel : channels) {
            json channelJson;
            if(channel.name.has_value()) channelJson["name"] = channel.name.value();
            if(channel.type.has_value()) channelJson["type"] = channel.type.value();
            if(channel.topic.has_value()) channelJson["topic"] = channel.topic.value();
            if(channel.bitrate.has_value()) channelJson["bitrate"] = channel.bitrate.value();
            if(channel.userLimit.has_value()) channelJson["user_limit"] = channel.userLimit.value();
            if(channel.rateLimitPerUser.has_value()) channelJson["rate_limit_per_user"] = channel.rateLimitPerUser.value();
            if(channel.position.has_value()) channelJson["position"] = channel.position.value();
            if(!channel.permissionOverwrites.empty()) {

            }
            if(channel.parentId.has_value()) channelJson["parent_id"] = channel.parentId.value();
            if(channel.nsfw.has_value()) channelJson["nsfw"] = channel.nsfw.value();
            if(channel.rtcRegion.has_value()) channelJson["rtc_region"] = channel.rtcRegion.value();
            if(channel.videoQualityMode.has_value()) channelJson["video_quality_mode"] = channel.videoQualityMode.value();
            if(channel.defaultAutoArchiveDuration.has_value()) channelJson["default_auto_archive_duration"] = channel.defaultAutoArchiveDuration.value();
            if(channel.defaultReactionEmoji.emojiId.has_value()) {
                channelJson["default_reaction_emoji"]["emoji_id"] = channel.defaultReactionEmoji.emojiId.value();
            }
            if(channel.defaultReactionEmoji.emojiName.has_value()) {
                channelJson["default_reaction_emoji"]["emoji_name"] = channel.defaultReactionEmoji.emojiName.value();
            }

            if(!channel.availableTags.empty()) {

            }

            if(channel.defaultSortOrder.has_value()) channelJson["default_sort_order"] = channel.defaultSortOrder.value();
            createGuildChannel.emplace_back(channelJson);
        }

        if(guildOptions.afkChannelId.has_value()) createGuildPayload["afk_channel_id"] = guildOptions.afkChannelId.value();
        if(guildOptions.afkTimeout.has_value()) createGuildPayload["afk_timeout"] = guildOptions.afkTimeout.value();
        if(guildOptions.systemChannelId.has_value()) createGuildPayload["system_channel_id"] = guildOptions.systemChannelId.value();
        if(guildOptions.systemChannelFlags.has_value()) createGuildPayload["system_channel_flags"] = guildOptions.systemChannelFlags.value();

        const std::string newGuild = request::httpsRequest("discord.com", "/api/guilds", createGuildPayload.dump(), boost::beast::http::verb::post, client->rateLimit, client->botToken);
        return guild::getGuildData(json::parse(newGuild));
    }
    guild client::guildOptions::getGuild(const long& guildId, const bool& withCounts, const bool& cacheObject) const {
        json payload;
        payload["with_counts"] = withCounts;
        const std::string newGuild = request::httpsRequest("discord.com", "/api/guilds/" + std::to_string(guildId), payload.dump(), boost::beast::http::verb::get, client->rateLimit, client->botToken);
        if(cacheObject) {

        }
        return guild::getGuildData(json::parse(newGuild));
    }
    */
} // helios

