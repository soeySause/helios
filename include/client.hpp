#ifndef HELIOS_CLIENT_HPP
#define HELIOS_CLIENT_HPP

#include <string>
#include <iostream>
#include <random>
#include <future>
#include <functional>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include "cache.hpp"
#include "ssl/root_certification.hpp"
#include "session.hpp"
#include "discordApiRequest.hpp"
#include "event.hpp"
#include "heliosException.hpp"

namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using json = nlohmann::json;

namespace helios {
    class client;

    class properties {
    protected:
        friend class client;
        std::string browser;
        std::string device;
        std::string os;
    public:
        [[maybe_unused]] void setOs(const std::string& os);
        [[maybe_unused]] void setBrowser(const std::string& browser);
        [[maybe_unused]] void setDevice(const std::string& device);
    };

    class activities {
    private:
        friend class client;
        std::string name;
        int type = 0;
        std::string url;
    public:
        //void setActivity(const std::string& name, const int type, const std::string& url = "");
        [[maybe_unused]] void setName(const std::string& name);
        [[maybe_unused]] void setType(const int& type);
        [[maybe_unused]] void setUrl(const std::string& url);
    };

    class presence {
    private:
        friend class client;
        int since = 0;
        std::string status;
        bool afk = false;
    public:
        [[maybe_unused]] void setSince(const int& since);
        activities activities;
        [[maybe_unused]] void setStatus(const std::string& status);
        [[maybe_unused]] void setAfk(const bool& afk);
    };

    class onEvent {
    private:
        friend class client;
        std::function<void(readyEvent)> readyFunction;
    public:
        [[maybe_unused]] void ready(const std::function<void(readyEvent)>& userFunction);
        [[maybe_unused]] void resumed(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void reconnect(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void applicationCommandPermissionsUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void autoModerationRuleCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationRuleUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationRuleDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationActionExecution(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void channelCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void channelUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void channelDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void channelPinUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void threadCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadListSync(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadMemberUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadMembersUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildBanAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildBanRemove(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildEmojiUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildStickersUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildIntegrationsUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildMemberAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildMemberRemove(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildMemberUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildMembersChunk(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildRoleCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildRoleUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildRoleDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildScheduledEventCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUserAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUserRemove(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void integrationCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void integrationUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void integrationDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void inviteCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void inviteDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void messageCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageDeleteBulk(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageReactionAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageReactionRemoveAll(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageReactionRemoveEmoji(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void presenceUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void stageInstanceCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void stageInstanceUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void stageInstanceDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void typingStart(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void userUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void voiceStateUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void voiceServerUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void webhooksUpdate(const std::function<void(eventData)>& userFunction);

    };

    struct shardStruct {
        int shardId{};
        int seq = 0;
        bool running = false;
        bool reconnect = false;
        bool fullReconnect = false;
        bool deleteShard = false;

        eventData eventData;
        std::weak_ptr<session> sessionShard;
        std::unique_ptr<std::thread> shardThread;
        std::thread::id shardThreadId;

        bool receivedHeartbeat = true;
        std::promise<bool> exitHeartbeat;
        std::future<bool> exitHeartbeatFuture = exitHeartbeat.get_future();
        std::unique_ptr<std::thread> heartbeatThread;
        std::thread::id heartbeatThreadId;

        std::string sessionId;
        std::string resumeUrl;

        std::promise<bool> exit;
        std::future<bool> exitFuture = exit.get_future();
        beast::static_string<123, char> closeReason;
        std::uint16_t closeCode{};
    };

    class shard {
    private:
        friend class client;
        friend class shardedClient;
        std::unique_ptr<shardStruct> shardStructPtr = std::make_unique<shardStruct>();;
    public:
        [[maybe_unused]] void deleteShard();
        [[maybe_unused]] void reconnect();
    };

    class client {
    private:
        bool compress = false;
        int large_threshold = 50;
        int intents = 7;

        std::condition_variable updateCondition;
        std::mutex mutex;

        void startHeartbeatCycle(const std::shared_ptr<session>& sessionShard);
        void stopHeartbeatCycle();
        void heartbeatCycle(const std::shared_ptr<session>& sessionShard);
        void sendHeartbeat(const std::shared_ptr<session>& sessionShard);
        void parseResponse(const std::shared_ptr<session>& sessionShard, const std::string& response);
        void wsShard(const std::string& host);

        json getIdentifyPayload(const int& shard);
    protected:
        bool enableSharding = false;

        ssl::context sslContext{ssl::context::tlsv12_client};
        std::shared_ptr<cache> cache_;
        std::vector<std::shared_ptr<shard>> shardClass;
        void createWsShard(const std::shared_ptr<shard>& shard);
        void reconnectShard(const std::shared_ptr<shard>& shard);
        void fullReconnectShard(const std::shared_ptr<shard>& shard);
        void deleteShard(const std::shared_ptr<shard>& shardR);

    public:
        explicit client(const std::string& token);

        [[noreturn]] [[maybe_unused]] void run();
        onEvent onEvent;

        // Identify payload information
        [[maybe_unused]] void setLargeThreshold(int threshold);
        properties properties;
        presence presence;
    };
} // helios

#endif //HELIOS_CLIENT_HPP
