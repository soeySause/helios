#ifndef HELIOS_TEAM_HPP
#define HELIOS_TEAM_HPP
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "user.hpp"
#include "heliosException.hpp"

namespace helios {
    class teamMember {
    private:
        friend class team;
        static teamMember getTeamMemberData(const nlohmann::json& jsonData);
    public:
        std::optional<long> membershipState;
        std::vector<std::string> permissions;
        std::optional<long> teamId;
        user user;
    };
    class team {
    private:
        friend class application;
        static team getTeamData(const nlohmann::json& jsonData);
    public:
        std::optional<std::string> icon;
        std::optional<long>id;
        std::unordered_map<long, teamMember> members;
        std::optional<std::string> name;
        std::optional<long> ownerUserId;
    };
}

#endif //HELIOS_TEAM_HPP
