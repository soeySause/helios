#ifndef HELIOS_CLIENT_HPP
#define HELIOS_CLIENT_HPP

#include <string>
#include <iostream>
#include <random>
#include <iostream>
#include <future>
#include <functional>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>

#include "ssl/root_certification.hpp"
#include "session.hpp"
#include "event.hpp"
#include "cache.hpp"
#include "request.hpp"
#include "discordClasses/discordClasses.hpp"
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

    struct shardStruct {
        int shardId{};
        int seq = 0;
        int ioContextId = 0;
        std::vector<std::time_t> sentMessageTimestamp;

        bool running = false;
        bool reconnect = false;
        bool fullReconnect = false;
        bool deleteShard = false;

        eventData eventData;
        std::weak_ptr<session> sessionShard;
        std::unique_ptr<std::thread> shardThread;
        std::thread::id shardThreadId;

        bool receivedHeartbeat = true;
        std::unique_ptr<boost::asio::strand<boost::asio::io_context::executor_type>> heartbeatIoContextStrand;
        std::unique_ptr<boost::asio::steady_timer> heartbeatIntervalTimer;
        std::optional<int> heartbeatInterval;

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
        std::unique_ptr<shardStruct> shardStructPtr = std::make_unique<shardStruct>();
    public:
        [[maybe_unused]] void deleteShard();
        [[maybe_unused]] void reconnect();
    };

    class client {
    private:
        std::string host;
        std::string botToken;
        std::shared_ptr<rateLimitStruct> rateLimit;
        int maxConcurrency;

        bool compress = false;
        int large_threshold = 50;
        int intents = 2097151;

        std::vector<std::chrono::time_point<std::chrono::system_clock>> shardCreationTime;
        boost::asio::io_context ioContext;
        std::condition_variable updateCondition;
        std::mutex mutex;

        void startHeartbeatCycle(const std::shared_ptr<shard>& shard);
        static void stopHeartbeatCycle(const std::shared_ptr<shard>& shard);
        static void sendHeartbeat(const std::shared_ptr<shard>& shard);
        void parseResponse(const std::shared_ptr<shard>& shard, const std::string& response);
        void wsShard(const std::string& connectedHost, const std::shared_ptr<shard>& shard);
        static bool sendGatewayMessage(const std::shared_ptr<shard>& shard, const std::string& payload);
        void onReceiveData(const boost::system::error_code& ec, std::size_t bytes_transferred, const std::shared_ptr<shard>& shard, const std::string& data);

        json getIdentifyPayload(const int& shard);
    protected:
        int shards;
        int numberOfIoContext;
        bool enableSharding = false;

        ssl::context sslContext{ssl::context::tlsv12_client};
        std::vector<std::shared_ptr<shard>> shardClass;
        std::unordered_map<std::thread::id, int> threadMap;
        void createWsShard(const std::shared_ptr<shard>& shard);
        void reconnectShard(const std::shared_ptr<shard>& shard);
        void fullReconnectShard(const std::shared_ptr<shard>& shard);
        void deleteShard(const std::shared_ptr<shard>& shard);

    public:
        explicit client(const std::string& token);
        explicit client(const std::string& token, const std::map<std::string, int>& options);

        class applicationRoleConnectionMetadataOptions {
        private:
            friend class client;
            client *client;
        public:
            [[maybe_unused]] applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecords(const long& applicationId);
            [[maybe_unused]] applicationRoleConnectionMetadata updateApplicationRoleConnectionMetadataRecords(const long& applicationId);
        };
        class channelOptions {
        private:
            friend class client;
            client *client;
        public:
            [[maybe_unused]] channel create(const channel& channelOptions);
            [[maybe_unused]] [[nodiscard]] std::future<channel> get(const long& channelId, const bool& cacheObject = true) const;
            [[maybe_unused]] [[nodiscard]] channel getFromCache(const long& channelId) const;
            [[maybe_unused]] bool existsInCache(const long& guildId) const;
        };
        class guildOptions {
        private:
            friend class client;
            client *client;
        public:
            [[maybe_unused]] guild createGuild(const guild& guildOptions, const std::vector<role>& roles = {}, const std::vector<channel>& channels = {});
            [[maybe_unused]] [[nodiscard]] guild getGuild(const long& guildId, const bool& withCounts = false, const bool& cacheObject = true) const;
            guild getGuildFromCache(const long& guildId) const;
            bool guildExistsInCache(const long& guildId) const;
            guildPreview getGuildPreview(const long& guildId) const;
        };

        applicationRoleConnectionMetadataOptions applicationRoleConnectionMetadata{};
        channelOptions channels{};
        guildOptions guilds{};

        [[maybe_unused]] void reconnect();
        [[noreturn]] [[maybe_unused]] void run();
        onEvent onEvent;

        // Identify payload information
        [[maybe_unused]] void setLargeThreshold(int threshold);
        properties properties;
        presence presence;

    };
} // helios

#endif //HELIOS_CLIENT_HPP
