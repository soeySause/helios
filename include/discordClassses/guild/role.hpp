#ifndef HELIOS_ROLE_HPP
#define HELIOS_ROLE_HPP
#include <optional>
#include <nlohmann/json.hpp>
#include <string>

namespace helios {
    class roleTags {
    private:
        friend class role;
        static roleTags getRoleTagsData(const nlohmann::json& jsonData);
    public:
        std::optional<long> botId;
        std::optional<long> integrationId;
        std::optional<bool> premiumSubscriber;
        std::optional<long> subscriptionListing_id;
        std::optional<bool> availableForPurchase;
        std::optional<bool> guildConnections;
    };

    class role {
    private:
        friend class guild;
        friend class message;
        static role getRoleData(const nlohmann::json& jsonData);
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<int> color;
        std::optional<bool> hoist;
        std::optional<std::string> icon;
        std::optional<std::string> unicodeEmoji;
        std::optional<int> position;
        std::optional<std::string> permissions;
        std::optional<bool> managed;
        std::optional<bool> mentionable;
        roleTags tags;
    };

} // helios

#endif //HELIOS_ROLE_HPP
