#ifndef HELIOS_RATELIMIT_HPP
#define HELIOS_RATELIMIT_HPP
#include <string>
#include <mutex>
#include <atomic>
#include <memory>
#include <chrono>
#include <algorithm>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include "discordClasses/channel/attachment.hpp"

class httpRequest {
public:
    std::string host;
    std::string target;
    nlohmann::json payload;
    boost::beast::http::verb method;;
    std::string authorization;
    std::string reason;
    std::vector<helios::attachment> attachments;

    explicit httpRequest(const std::string& host, const std::string& target, const nlohmann::json& payload, const boost::beast::http::verb& method, const std::string& authorization, const std::string& reason, const std::vector<helios::attachment>& attachments);
};

class rateLimitEndpoint {
private:
    std::string bucket;
public:
    rateLimitEndpoint();
    void putRateLimit(const std::string& endpoint, boost::beast::http::response<boost::beast::http::string_body> response);
    void getRateLimit(const std::string& endpoint);

};

class rateLimitGlobal {
private:
    int globalRateLimit;
    std::atomic<unsigned long long int> msTimeSinceLastReqCheck = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::atomic<int> totalReq = 0;
    std::mutex globalRateLimitMutex;
public:
    explicit rateLimitGlobal(const int& globalRateLimit);
    long long int countHttpsReq();
};

struct rateLimitStruct {
    std::unique_ptr<rateLimitGlobal> rateLimitGlobal;
    std::unique_ptr<rateLimitEndpoint> rateLimitEndpoint;
};
#endif //HELIOS_RATELIMIT_HPP
