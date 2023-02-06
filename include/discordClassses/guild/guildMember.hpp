#ifndef HELIOS_GUILDMEMBER_HPP
#define HELIOS_GUILDMEMBER_HPP
#include <string>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

#include "../user.hpp"
#include "role.hpp"

namespace helios {
    class guildMember {
    private:
        friend class eventData;
        friend class guild;
        friend class voiceState;
        static guildMember getGuildMemberData(const nlohmann::json& jsonData);
    public:
        user user;
        std::optional<std::string> nick;
        std::optional<std::string> avatar;
        std::vector<long> roles;
        std::optional<std::string> joinedAt;
        std::optional<std::string> premiumSince;
        std::optional<bool> deaf;
        std::optional<bool> mute;
        std::optional<bool> pending;
        std::optional<std::string> permissions;
        std::optional<std::string> communicationDisabledUntil;
    };
}

#endif //HELIOS_GUILDMEMBER_HPP
