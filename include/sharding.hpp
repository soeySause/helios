#ifndef HELIOS_SHARDING_HPP
#define HELIOS_SHARDING_HPP

#include "cache.hpp"
#include "client.hpp"
#include "heliosException.hpp"

#include <thread>
#include <vector>
#include <string>
#include <iostream>

namespace helios{
    class shardedClient : public client {
    public:
        [[maybe_unused]] explicit shardedClient(const std::string &token, const int &shards);
        [[maybe_unused]] explicit shardedClient(const std::string &token, const std::string& type);

        [[maybe_unused]] std::vector<std::shared_ptr<shard>> pool(const int& delay = 500);
        [[maybe_unused]] std::shared_ptr<shard> createShard(const int& shardId);
        [[maybe_unused]] void removeShard(const std::shared_ptr<shard>& shard);
        [[maybe_unused]] void reconnectAllShards();
    };
}


#endif //HELIOS_SHARDING_HPP
