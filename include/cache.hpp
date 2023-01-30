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

#include "heliosException.hpp"

namespace helios {
    class cache {
    private:
        friend class client;
        std::string pathToTokenDirectory;
        static void createCacheDirectory();

        //static void cacheGuild(const guildCacheObject& guildToBeCached);
        //static guildCacheObject getGuildFromCache(const long& guildIdToGetFromCache, const int& shard);
        //static bool removeGuildFromCache(const long& guildIdToBeDeletedFromCache);
        //static bool removeAllGuildsFromCache();

    public:
    };
} // helios

#endif //HELIOS_CACHE_HPP
