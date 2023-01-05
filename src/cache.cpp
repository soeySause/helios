#include "cache.hpp"
#include <iostream>
cache::cache()= default;;

void cache::put(const std::string &key, const std::string &val) {
    auto it = item_map.find(key);
    if(it != item_map.end()){
        item_list.erase(it->second);
        item_map.erase(it);
    }
    item_list.push_front(make_pair(key,val));
    item_map.insert(make_pair(key, item_list.begin()));
}

bool cache::exist(const std::string &key) {
    return (item_map.count(key)>0);
}

std::string cache::get(const std::string &key) {
    assert(exist(key));
    auto it = item_map.find(key);
    item_list.splice(item_list.begin(), item_list, it->second);
    return it->second->second;
}

void cache::remove(const std::string &key)
{
    assert(exist(key));
    auto it = item_map.find(key);
    item_list.erase(it->second);
    item_map.erase(it);
}
