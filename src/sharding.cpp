#include "sharding.hpp"

namespace helios {
    shardedClient::shardedClient(const std::string &token, const int &shards) : client(token) {
        this->cache_->put("shards", std::to_string(shards));
        this->enableSharding = true;
    }

    shardedClient::shardedClient(const std::string &token, const std::string& type) : client(token) {
        if(type != "auto") {
            std::cerr << "Unknown shard type " << type << std::endl;
            exit(EXIT_FAILURE);
        }
        this->enableSharding = true;
    }

    void shardedClient::createShard(const int &shardId) {
        //if(this->running) {
            this->createWsShard(shardId, this->cache_->get("host"));
        //} else {
            this->startupShards.emplace_back(shardId);
        //}
    }

    void shardedClient::pool(const int &delay) {
        //if(running) {
            for(int shard = 0; shard < std::stoi(this->cache_->get("shards")); shard++) {
                createWsShard(shard, this->cache_->get("host"));
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        //} else {
            for(int shardId = 0; shardId < std::stoi(this->cache_->get("shards")); shardId++) {
                startupShards.emplace_back(shardId);
            }
        //}
    }

    void shardedClient::deleteShard(const int &shardId) {
        const auto positionInVector = std::find(startupShards.begin(), startupShards.end(), shardId);
        if(positionInVector == startupShards.end()) {
            std::cerr << "Shard id " + std::to_string(shardId) + "not found in vector!" << std::endl;
            exit(EXIT_FAILURE);
        }

        //if(running) {

        //} else {
            startupShards.erase(positionInVector);
        //}
    }

    void shardedClient::restartShard(const int &shardId) {

    }

    void shardedClient::restartAllShards() {

    }

    void shardedClient::reconnectShard(const int &shardId) {

    }

    void shardedClient::reconnectAllShards() {

    }
}

