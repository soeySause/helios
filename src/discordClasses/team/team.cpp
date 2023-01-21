#include "discordClassses/team.hpp"

helios::teamMember helios::teamMember::getTeamMemberData(const nlohmann::json &jsonData) {
    teamMember teamMemberData;
    if(jsonData.contains("membership_state")) teamMemberData.membershipState = jsonData["membership_state"];
    if(jsonData.contains("permissions")) {
        for(const nlohmann::basic_json<>& permission : jsonData["permissions"]) {
            teamMemberData.permissions.emplace_back(permission);
        }
    }

    if(jsonData.contains("team_id")) teamMemberData.teamId = std::stol(jsonData["team_id"].get<std::string>());
    if(jsonData.contains("user")) teamMemberData.user = user::getUserData(jsonData["user"]);
    return teamMemberData;
}

helios::team helios::team::getTeamData(const nlohmann::json &jsonData) {
    team teamData;
    if(jsonData.contains("icon")) teamData.icon = jsonData["icon"];
    if(jsonData.contains("id")) teamData.id = std::stol(jsonData["id"].get<std::string>());
    if(jsonData.contains("members")) {
        for(const nlohmann::basic_json<>& member : jsonData["members"]) {
            if(jsonData["user"]["id"]) throw(heliosException(30, "Could not find [user][id] in " + jsonData["members"].dump()));
            teamData.members.at(std::stol(member["user"]["id"].get<std::string>())) = teamMember::getTeamMemberData(member);
        }
    }
    if(jsonData.contains("name")) teamData.name = jsonData["name"];
    if(jsonData.contains("owner_user_id")) teamData.ownerUserId = std::stol(jsonData["owner_user_id"].get<std::string>());
    return teamData;
}
