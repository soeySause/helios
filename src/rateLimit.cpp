#include "rateLimit.hpp"
#include <thread>
#include <memory>
#include <iostream>

httpRequest::httpRequest(const std::string& host, const std::string& target, const json& payload, const boost::beast::http::verb& method,const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::string& reason, const std::function<void(http::response<http::string_body>)>& onResponse) {
    this->host = host;
    this->target = target;
    this->payload = payload;
    this->method = method;
    this->rateLimit = rateLimit;
    this->authorization = authorization;
    this->reason = reason;
    this->onResponse = onResponse;
}
httpRequest::httpRequest(const std::string& host, const std::string& target, const json& payload, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::function<void(http::response<http::string_body>)>& onResponse) {
    this->host = host;
    this->target = target;
    this->payload = payload;
    this->method = method;
    this->rateLimit = rateLimit;
    this->authorization = authorization;
    this->onResponse = onResponse;
}
httpRequest::httpRequest(const std::string& host, const std::string& target, const json& jsonPayload, const std::vector<helios::attachment>& attachments, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::string& reason, const std::function<void(http::response<http::string_body>)>& onResponse) {
    this->host = host;
    this->target = target;
    this->attachments = attachments;
    this->payload = jsonPayload;
    this->method = method;
    this->rateLimit = rateLimit;
    this->authorization = authorization;
    this->reason = reason;
    this->onResponse = onResponse;
}
httpRequest::httpRequest(const std::string& host, const std::string& target, const json& jsonPayload, const std::vector<helios::attachment>& attachments, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::function<void(http::response<http::string_body>)>& onResponse) {
    this->host = host;
    this->target = target;
    this->attachments = attachments;
    this->payload = jsonPayload;
    this->method = method;
    this->rateLimit = rateLimit;
    this->authorization = authorization;
    this->onResponse = onResponse;

}

rateLimitGlobal::rateLimitGlobal(const int& globalRateLimit) {
    this->globalRateLimit = globalRateLimit;
}

long long int rateLimitGlobal::countHttpsReq() {
    auto nowInMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::unique_lock<std::mutex> lock(this->globalRateLimitMutex);
    auto lastCheckedGlobalRateLimit = nowInMs - this->msTimeSinceLastReqCheck;

    for(int i = 0; i < lastCheckedGlobalRateLimit/1000; i++) {
        this->totalReq = std::max(0, this->totalReq - this->globalRateLimit);
    }

    this->totalReq.fetch_add(1, std::memory_order_relaxed);
    this->msTimeSinceLastReqCheck = nowInMs;

    long long int timeToWait = 0;
    if(this->totalReq > this->globalRateLimit) {
        timeToWait += 1000*((this->totalReq - this->globalRateLimit - 1)/this->globalRateLimit) + nowInMs%1000;
    }
    lock.unlock();
    return timeToWait;
}
