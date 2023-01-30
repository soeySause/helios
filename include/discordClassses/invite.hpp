#ifndef HELIOS_INVITE_HPP
#define HELIOS_INVITE_HPP
#include <optional>
#include <string>
#include <unordered_map>
#include "guild/guild.hpp"
#include "guild/guildMember.hpp"
#include "guildScheduledEvent.hpp"
#include "channel/channel.hpp"
#include "user.hpp"
#include "application.hpp"
namespace helios {

    class inviteStageInstance {
        std::unordered_map<long, guildMember> members;
        std::optional<int> participantCount;
        std::optional<int> speakerCount;
        std::optional<std::string> topic;
    };

    class invite {
        std::optional<std::string> code;
        guild guild;
        channel channel;
        user inviter;
        std::optional<int> targetType;
        user targetUser;
        application targetApplication;
        std::optional<int> approximatePresenceCount;
        std::optional<int> approximateMemberCount;
        std::optional<std::string> expiresAt;
        inviteStageInstance stageInstance;
        guildScheduledEvent guildScheduledEvent;
    };

    class inviteMetadata : public invite {
        std::unordered_map<long, guildMember> members;
        std::optional<int> participantCount;
        std::optional<int> speakerCount;
        std::optional<std::string> topic;
    };

} // helios

#endif //HELIOS_INVITE_HPP
