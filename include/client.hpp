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

    class properties {
    protected:
        std::string browser;
        std::string device;
        std::string os;
    public:
        void setOs(const std::string& os);
        [[nodiscard]] std::string getOs() const;
        void setBrowser(const std::string& browser);
        [[nodiscard]] std::string getBrowser() const;
        void setDevice(const std::string& device);
        [[nodiscard]] std::string getDevice() const;
    };

    class activities {
    private:
        std::string name;
        int type;
        std::string url;
    public:
        //void setActivity(const std::string& name, const int type, const std::string& url = "");
        void setName(const std::string& name);
        [[nodiscard]] std::string getName() const;
        void setType(const int& type);
        [[nodiscard]] int getType() const;
        void setUrl(const std::string& url);
        [[nodiscard]] std::string getUrl() const;
    };

    class presence {
    private:
        int since = 0;
        std::string status;
        bool afk = false;
    public:
        void setSince(const int& since);
        [[nodiscard]] int getSince() const;
        activities activities;
        void setStatus(const std::string& status);
        [[nodiscard]] std::string getStatus() const;
        void setAfk(const bool& afk);
        [[nodiscard]] bool getAfk() const;
    };

class client {
    private:
        ssl::context sslContext{ssl::context::tlsv12_client};
        std::shared_ptr<cache> cache_;
        std::vector<std::thread> heartbeatThread;
        std::vector<std::thread> shardedConnections;
        std::vector<std::shared_ptr<session>> shardedSessions;
        bool reconnecting = false;

        bool compress = false;
        int large_threshold = 50;
        int intents = 7;

        void startHeartbeatCycle(const std::shared_ptr<session>& sessionShard);
        void stopHeartbeatCycle();
        void heartbeatCycle(const std::shared_ptr<session>& sessionShard);
        void sendHeartbeat(const std::shared_ptr<session>& sessionShard);
        void parseResponse(const std::shared_ptr<session>& sessionShard, const std::string& response, const int& shard);
        void createWsShard(const int& shard, const std::string& host);
        void reconnect();

        json getIdentifyPayload(const int& shard);
public:
        explicit client();

        [[noreturn]] [[maybe_unused]] void run(const std::string& token = "");

        // Identify payload information
        [[maybe_unused]] void setToken(const std::string& token);
        properties properties;
        [[maybe_unused]] void setLargeThreshold(int threshold);
        [[nodiscard]] int getLargeThreshold() const;
        [[maybe_unused]] void setShards(int shards);
        presence presence;
        //TODO calculate intents automatically depending on what events they have;
        [[maybe_unused]] void setIntents(int intents);
        [[nodiscard]] int getIntents() const;
    };
} // helios

#endif //HELIOS_CLIENT_HPP
