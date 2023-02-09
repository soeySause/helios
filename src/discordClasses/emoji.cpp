#include "discordClasses/emoji.hpp"

namespace helios {
    emoji emoji::getEmojiData(const nlohmann::json& jsonData) {
        emoji emojiData;
        if(!jsonData["id"].is_null()) {emojiData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(!jsonData["name"].is_null()) {emojiData.name = jsonData["name"]; }

        if(jsonData.contains("roles")) {
            for (const nlohmann::basic_json<> &roleSnowflake: jsonData["roles"]) {
                emojiData.roles.emplace_back(std::stol(roleSnowflake.get<std::string>()));
            }
        }


        if(jsonData.contains("user")) { emojiData.user = user::getUserData(jsonData["user"]); }
        if(jsonData.contains("require_colons")) {emojiData.require_colons = jsonData["require_colons"]; }
        if(jsonData.contains("managed")) {emojiData.managed = jsonData["managed"]; }
        if(jsonData.contains("animated")) {emojiData.animated = jsonData["animated"]; }
        if(jsonData.contains("available")) {emojiData.available = jsonData["available"]; }
        return emojiData;
    }
}//helios