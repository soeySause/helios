#ifndef HELIOS_CLIENT_HPP
#define HELIOS_CLIENT_HPP

#include <string>
#include <iostream>
#include <random>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include "cache.hpp"
#include "ssl/root_certification.hpp"
#include "session.hpp"
#include "discordApiRequest.hpp"

namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using json = nlohmann::json;

namespace helios {
    class client;
    class shardedClient;

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

class client {
    private:
        bool compress = false;
        int large_threshold = 50;
        int intents = 7;

        void startHeartbeatCycle(const std::shared_ptr<session>& sessionShard);
        void stopHeartbeatCycle();
        void heartbeatCycle(const std::shared_ptr<session>& sessionShard);
        void sendHeartbeat(const std::shared_ptr<session>& sessionShard);
        void parseResponse(const std::shared_ptr<session>& sessionShard, const std::string& response, const int& shard);
        void wsShard(const int& shard, const std::string& host);
        void reconnect();

        json getIdentifyPayload(const int& shard);
protected:
        bool enableSharding = false;
        bool running = false;
        bool reconnecting = false;

        ssl::context sslContext{ssl::context::tlsv12_client};
        std::shared_ptr<cache> cache_;
        std::vector<int> shardIdVector;
        std::vector<std::thread> heartbeatThreads;
        std::vector<std::thread> shardedConnections;
        std::vector<std::shared_ptr<session>> shardedSessions;
        void createWsShardPool(const std::string& host, const int& delay = 250);
        void createWsShard(const int& shardId, const std::string& host);
public:
        explicit client(const std::string& token);

        [[noreturn]] [[maybe_unused]] void run();

        // Identify payload information
        [[maybe_unused]] void setLargeThreshold(int threshold);
        properties properties;
        presence presence;
    };
} // helios

#endif //HELIOS_CLIENT_HPP
