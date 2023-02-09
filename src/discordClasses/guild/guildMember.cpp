#include "discordClasses/guild/guildMember.hpp"

helios::guildMember helios::guildMember::getGuildMemberData(const nlohmann::json &jsonData) {
    guildMember guildMemberData;
    if(jsonData.contains("user")) guildMemberData.user = user::getUserData(jsonData["user"]);
    if(jsonData.contains("nick") && !jsonData["nick"].is_null()) guildMemberData.nick = jsonData["nick"];
    if(jsonData.contains("avatar") && !jsonData["avatar"].is_null()) guildMemberData.avatar = jsonData["avatar"];
    for (const nlohmann::basic_json<> &roleSnowflake: jsonData["roles"]) {
        guildMemberData.roles.emplace_back(std::stol(roleSnowflake.get<std::string>()));
    }

    guildMemberData.joinedAt = jsonData["joined_at"];
    if(jsonData.contains("premium_since") && !jsonData["premium_since"].is_null()) guildMemberData.premiumSince = jsonData["premium_since"];
    guildMemberData.deaf = jsonData["deaf"];
    guildMemberData.mute = jsonData["mute"];
    if(jsonData.contains("pending")) guildMemberData.pending = jsonData["pending"];
    if(jsonData.contains("permissions")) guildMemberData.permissions = jsonData["permissions"];
    if(jsonData.contains("communication_disabled_until") && !jsonData["communication_disabled_until"].is_null()) guildMemberData.communicationDisabledUntil = jsonData["communication_disabled_until"];
    return guildMemberData;
}
