#include "discordApiRequest.hpp"

int apiRequest::getGateway(const std::shared_ptr<cache>& cache_) {
    std::vector<std::string> whatToCache = {"url", "shards"};
    return request::getRequest("discord.com", "/api/gateway/bot", cache_, cache_->get("token"), whatToCache);
}

