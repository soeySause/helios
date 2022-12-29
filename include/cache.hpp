#ifndef HELIOS_CACHE_HPP
#define HELIOS_CACHE_HPP

#include <list>
#include <unordered_map>
#include <cassert>
#include <string>

class cache {
private:
    std::list< std::pair<std::string,std::string> > item_list;
    std::unordered_map<std::string, decltype(item_list.begin()) > item_map;
public:
    explicit cache();
    void put(const std::string &key, const std::string &val);
    bool exist(const std::string &key);
    std::string get(const std::string &key);
    void remove(const std::string &key);
};

#endif //HELIOS_CACHE_HPP
