#include "discordApiRequest.hpp"

int apiRequest::getGateway(const std::shared_ptr<cache>& cache_) {
    std::vector<std::string> whatToCache = {"url"};

    // If user has defined how many shards they want, don't cache discords recommended shards
    if(!cache_->exist("shards")) {
        whatToCache.emplace_back("shards");
    }

    return request::getRequest("discord.com", "/api/gateway/bot", cache_, cache_->get("token"), whatToCache);
}

