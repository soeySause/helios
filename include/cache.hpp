#ifndef HELIOS_CACHE_HPP
#define HELIOS_CACHE_HPP
#include <string>
#include <fstream>
#include <iostream>
#include <optional>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/filesystem.hpp>

#include "discordClassses/discordClasses.hpp"
#include "heliosException.hpp"

namespace helios {
    class guildCacheObject {
        guild guild;
        std::unordered_map<long, voiceState> voiceStates;
        std::unordered_map<long, guildMember> members;
        std::unordered_map<long, channel> channels;
        std::unordered_map<long, channel> threads;
        std::unordered_map<long, presenceUpdate> presences;
        std::unordered_map<long, stageInstance> stageInstances;
        std::unordered_map<long, guildScheduledEvent> guildScheduledEvents;
    };
    class cache {
    private:
        friend class client;
        std::string pathToTokenDirectory;
        static void createCacheDirectory(const std::string& path = "cache");

        static void cacheGuild(const guildCacheObject& guildToBeCached, const int& shard);
        static guildCacheObject getGuildFromCache(const long& guildIdToGetFromCache, const int& shard);
        //static bool removeGuildFromCache(const long& guildIdToBeDeletedFromCache);
        //static bool removeAllGuildsFromCache();

    public:
    };
} // helios

#endif //HELIOS_CACHE_HPP
