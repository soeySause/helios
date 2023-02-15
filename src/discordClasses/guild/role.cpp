#include "discordClasses/guild/role.hpp"

namespace helios {
    roleTags roleTags::getRoleTagsData(const nlohmann::json &jsonData) {
        roleTags roleTagsData;
        if(jsonData.contains("bot_id")) {roleTagsData.botId = std::stol(jsonData["bot_id"].get<std::string>()); }
        if(jsonData.contains("integration_id")) {roleTagsData.integrationId = std::stol(jsonData["integration_id"].get<std::string>()); }
        if(jsonData.contains("premium_subscriber")) {
            if (jsonData["premium_subscriber"].is_null()) {
                roleTagsData.premiumSubscriber = false;
            } else {roleTagsData.premiumSubscriber = true; }
        }
        if(jsonData.contains("subscription_listing_id")) {roleTagsData.subscriptionListing_id = std::stol(jsonData["subscription_listing_id"].get<std::string>()); }

        if(jsonData.contains("guild_connections")) {
            if (jsonData["guild_connections"].is_null()) {
                roleTagsData.guildConnections = false;
            } else {roleTagsData.guildConnections = true; }
        }

        if(jsonData.contains("available_for_purchase")) {
            if (jsonData["available_for_purchase"].is_null()) {
                roleTagsData.availableForPurchase = false;
            } else {roleTagsData.availableForPurchase = true; }
        }
        return roleTagsData;
    }

    role role::getRoleData(const nlohmann::json &jsonData) {
        role roleData;
        roleData.id = std::stol(jsonData["id"].get<std::string>());
        roleData.name = jsonData["name"];
        roleData.color = jsonData["color"];
        roleData.hoist = jsonData["hoist"];
        if(jsonData.contains("icon") && !jsonData["icon"].is_null()) roleData.icon = jsonData["icon"];
        if(jsonData.contains("unicode_emoji") && !jsonData["unicode_emoji"].is_null()) roleData.unicodeEmoji = jsonData["unicode_emoji"];
        roleData.position = jsonData["position"];
        roleData.permissions = jsonData["permissions"].dump();
        roleData.managed = jsonData["managed"];
        roleData.mentionable = jsonData["mentionable"];
        if(jsonData.contains("tags")) roleData.tags = roleTags::getRoleTagsData(jsonData["tags"]);
        return roleData;
    }
} // helios