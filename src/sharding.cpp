#include "sharding.hpp"

namespace helios {
    [[maybe_unused]] shardedClient::shardedClient(const std::string &token, const int &shards) : client(token) {
        this->shards = shards;
        this->enableSharding = true;
    }

    [[maybe_unused]] shardedClient::shardedClient(const std::string &token, const std::string& type) : client(token) {
        if(type != "auto") {
            throw(heliosException(70, "Unknown shard type"));
        }

        this->enableSharding = true;
    }

    std::shared_ptr<shard> shardedClient::createShard(const int &shardId) {
        std::shared_ptr<shard> newShard = std::make_shared<shard>();
        newShard->shardStructPtr->shardId = shardId;

        this->shardClass.emplace_back(newShard);

        if(this->shardClass.at(0)->shardStructPtr->running) {
            this->createWsShard(newShard);
        }

        return newShard;
    }

    [[maybe_unused]] void shardedClient::removeShard(const std::shared_ptr<shard>& targetShard) {
        if(targetShard->shardStructPtr->running) {
            throw(heliosException(80, "Cannot remove shard that is not running"));
        }

        auto shardPositionInVector = std::find_if(this->shardClass.begin(), this->shardClass.end(), [&targetShard](const std::shared_ptr<shard>& ts)
        { return ts->shardStructPtr == targetShard->shardStructPtr; });

        if (shardPositionInVector != this->shardClass.end()) {
            this->shardClass.erase(shardPositionInVector);
        } else {
            std::stringstream targetShardAddress;
            targetShardAddress << targetShard.get();

            const std::string error = "Cannot find shard " + targetShardAddress.str();
            throw(heliosException(81, error));
        }
    }


    std::vector<std::shared_ptr<shard>> shardedClient::pool(const int &delay) {
        std::vector<std::shared_ptr<shard>> shardVector;
        for(int shardId = 0; shardId < this->shards; shardId++) {
            shardVector.emplace_back(createShard(shardId));
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        return shardVector;
    }

    [[maybe_unused]] void shard::deleteShard() {
        if(this->shardStructPtr->running) {
            std::shared_ptr<session> sessionShard = this->shardStructPtr->sessionShard.lock();
            if (sessionShard) {
                this->shardStructPtr->deleteShard = true;
                sessionShard->asyncCloseSession();
            } else {
                throw(heliosException(83, "Error deleting shard. Missing websocket connection."));
            }
        } else {
            throw(heliosException(80, "Cannot delete shard that is not running"));
        }
    }

    void shard::reconnect() {
        if(this->shardStructPtr->running) {
            std::shared_ptr<session> sessionShard = this->shardStructPtr->sessionShard.lock();
            if (sessionShard) {
                this->shardStructPtr->fullReconnect = true;
                sessionShard->asyncCloseSession();
            } else {
                throw(heliosException(83, "Error reconnecting shard. Missing websocket connection."));
            }
        } else {
            throw(heliosException(80, "Cannot reconnect shard that is not running"));
        }
    }

    [[maybe_unused]] void shardedClient::reconnectAllShards() {
        if(!this->shardClass.back()->shardStructPtr->running) {
            throw(heliosException(80, "Cannot reconnect shard that is not running"));
        }

        for(auto& shard : this->shardClass ) {
            shard->reconnect();
        }
    }
}
