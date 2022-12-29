#include "discordApiRequest.hpp"

int apiRequest::getGateway(const std::shared_ptr<cache>& cache_) {
    std::cerr<< 1;
    return request::getRequest("discord.com", "/api/gateway/bot", cache_, cache_->get("token"), {"url",  "shards"});
}

