#ifndef HELIOS_DISCORDAPIREQUEST_HPP
#define HELIOS_DISCORDAPIREQUEST_HPP

#include <string>
#include <memory>
#include <vector>

#include "request.hpp"
#include "cache.hpp"

namespace apiRequest {
    int getGateway(const std::shared_ptr<cache>& cache_);
}

#endif //HELIOS_DISCORDAPIREQUEST_HPP
