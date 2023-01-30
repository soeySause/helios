#include "cache.hpp"

namespace helios {
    void cache::createCacheDirectory() {
        boost::filesystem::path dir_path("cache");
        try {
            boost::filesystem::create_directory(dir_path);
        } catch (const boost::filesystem::filesystem_error& ex) {
            throw(heliosException(16, ex.what()));
        }
    }
} // helios
