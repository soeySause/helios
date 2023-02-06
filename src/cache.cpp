#include "cache.hpp"

namespace helios {
    void cache::createCacheDirectory(const std::string& path) {
        boost::filesystem::path dir_path(path);
        try {
            if(boost::filesystem::exists(path)) {
                boost::filesystem::remove_all(path);
            }

            boost::filesystem::create_directory(dir_path);
        } catch (const boost::filesystem::filesystem_error& ex) {
            throw(heliosException(16, ex.what()));
        }
    }
} // helios
