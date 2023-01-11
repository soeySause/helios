#ifndef HELIOS_EVENT_HPP
#define HELIOS_EVENT_HPP

#include <iostream>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace helios
{
    class eventData{
    public:
        std::string session_type;
        int shardId;
        int numShards;
        static eventData readyEventData(const json& jsonData);
    };
}


#endif //HELIOS_EVENT_HPP
