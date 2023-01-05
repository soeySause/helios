#ifndef HELIOS_SHARDING_HPP
#define HELIOS_SHARDING_HPP

#include "cache.hpp"
#include "client.hpp"

#include <thread>
#include <vector>
#include <string>
#include <iostream>

namespace helios{
    class shardedClient : public client {
    public:
        [[maybe_unused]] explicit shardedClient(const std::string &token, const int &shards);
        [[maybe_unused]] explicit shardedClient(const std::string &token, const std::string& type);

        [[maybe_unused]] void pool(const int& delay = 500);
        [[maybe_unused]] void createShard(const int& shardId);
        [[maybe_unused]] void deleteShard(const int& shardId);
        [[maybe_unused]] void restartShard(const int& shardId);
        [[maybe_unused]] void restartAllShards();
        [[maybe_unused]] void reconnectShard(const int& shardId);
        [[maybe_unused]] void reconnectAllShards();
    };
}


#endif //HELIOS_SHARDING_HPP
