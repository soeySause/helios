#include "event.hpp"

namespace helios {
    user eventData::getUserData(const json &jsonData) {
        user userData;
        if(jsonData.contains("id")) {userData.id = jsonData["id"]; }
        if(jsonData.contains("username")) {userData.username = jsonData["username"]; }
        if(jsonData.contains("discriminator")) {userData.discriminator = jsonData["discriminator"]; }
        if(jsonData.contains("avatar")) {userData.avatar = jsonData["avatar"]; }
        if(jsonData.contains("bot")) {userData.bot = jsonData["bot"]; }
        if(jsonData.contains("system")) {userData.system = jsonData["system"]; }
        if(jsonData.contains("mfa_enabled")) {userData.mfa_enabled = jsonData["mfa_enabled"]; }
        if(jsonData.contains("banner")) {userData.banner = jsonData["banner"]; }
        if(jsonData.contains("accent_color")) {userData.accent_color = jsonData["accent_color"]; }
        if(jsonData.contains("locale")) {userData.locale = jsonData["locale"]; }
        if(jsonData.contains("verified")) {userData.verified = jsonData["verified"]; }
        if(jsonData.contains("email") && !jsonData["email"].is_null()) {userData.email = jsonData["email"]; }
        if(jsonData.contains("flags")) {userData.flags = jsonData["flags"]; }
        if(jsonData.contains("premium_type")) {userData.premium_type = jsonData["premium_type"]; }
        if(jsonData.contains("avatar")) {userData.avatar = jsonData["avatar"]; }
        if(jsonData.contains("public_flags")) {userData.public_flags = jsonData["public_flags"]; }
        return userData;
    }

    partialApplication eventData::getPartialApplicationData(const json &jsonData) {
        partialApplication partialApplicationData;
        if(jsonData.contains("id")) {partialApplicationData.id = jsonData["id"]; }
        if(jsonData.contains("flags")) {partialApplicationData.flags = jsonData["flags"]; }
        return partialApplicationData;
    }

    unavailableGuilds eventData::getUnavailableGuildsData(const json &jsonData) {
        unavailableGuilds unavailableGuildsData;
        if(jsonData.contains("id")) {unavailableGuildsData.id = jsonData["id"]; }
        if(jsonData.contains("unavailable")) {unavailableGuildsData.unavailable = jsonData["unavailable"]; }
        return unavailableGuildsData;
    }

    readyEvent eventData::getReadyEventData(const json &jsonData) {
        this->readyEventData.user = getUserData(jsonData["user"]);

        for(const nlohmann::basic_json<>& guild : jsonData["guilds"]) {
            this->readyEventData.guilds.emplace_back(this->getUnavailableGuildsData(guild));
        }

        if(jsonData.contains("shard")) {
            this->readyEventData.shard[0] = jsonData["shard"].front();
            this->readyEventData.shard[1] = jsonData["shard"].back();
        }

        this->readyEventData.application = getPartialApplicationData(jsonData["application"]);

        return this->readyEventData;
    }
}