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
#include <boost/beast/http.hpp>
#include "discordClasses/channel/attachment.hpp"

using nlohmann::json;
namespace http = boost::beast::http;

struct rateLimitStruct;
class httpRequest {
public:
    explicit httpRequest(const std::string& host, const std::string& target, const json& payload, const boost::beast::http::verb& method,const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::string& reason, const std::function<void(http::response<http::string_body>)>& onResponse);
    explicit httpRequest(const std::string& host, const std::string& target, const json& payload, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::function<void(http::response<http::string_body>)>& onResponse);
    explicit httpRequest(const std::string& host, const std::string& target, const json& jsonPayload, const std::vector<helios::attachment>& attachments, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::string& reason, const std::function<void(http::response<http::string_body>)>& onResponse);
    explicit httpRequest(const std::string& host, const std::string& target, const json& jsonPayload, const std::vector<helios::attachment>& attachments, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::function<void(http::response<http::string_body>)>& onResponse);

    std::string host;
    std::string target;
    int version = 11;
    std::string port = "443";
    nlohmann::json payload;
    http::verb method;
    std::string authorization;
    std::string reason;
    std::vector<helios::attachment> attachments;
    std::shared_ptr<rateLimitStruct> rateLimit;
    std::function<void(http::response<http::string_body>)> onResponse;
};

class rateLimitHeaders {
    std::string bucket;
    int requestsRemaining;
    float resetAfter;
    bool global;
    std::string scope;
};

class rateLimitEndpoint {
private:
    std::unordered_map<std::string, rateLimitHeaders> rateLimitHeaderMap;
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
