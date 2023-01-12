#ifndef HELIOS_EVENT_HPP
#define HELIOS_EVENT_HPP

#include <iostream>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace helios {
    class user {
    public:
        std::optional<std::string> id;
        std::optional<std::string> username;
        std::optional<std::string> discriminator;
        std::optional<std::string> avatar;
        std::optional<bool> bot;
        std::optional<bool> system;
        std::optional<bool> mfa_enabled;
        std::optional<std::string> banner;
        std::optional<int> accent_color;
        std::optional<std::string> locale;
        std::optional<bool> verified;
        std::optional<std::string> email;
        std::optional<int> flags;
        std::optional<int> premium_type;
        std::optional<int> public_flags;
    };

    class unavailableGuilds {
    public:
        std::optional<std::string> id;
        std::optional<bool> unavailable;
    };

    class partialApplication {
    public:
        std::optional<std::string> id;
        std::optional<int> flags;
    };

    class readyEvent {
    public:
        user user;
        std::vector<unavailableGuilds> guilds;
        std::optional<int> shard[2];
        partialApplication application;
    };

    class eventData {
    private:
        friend class client;
        static user getUserData(const json& jsonData);
        static partialApplication getPartialApplicationData(const json& jsonData);
        static unavailableGuilds getUnavailableGuildsData(const json& jsonData);

        readyEvent getReadyEventData(const json& jsonData);
    public:
        readyEvent readyEventData;
    };
}


#endif //HELIOS_EVENT_HPP
