#ifndef HELIOS_CHANNEL_HPP
#define HELIOS_CHANNEL_HPP
#include <optional>
#include <unordered_map>
#include <string>
#include <vector>
#include "guildMember.hpp"
#include "discordClassses/user.hpp"

namespace helios {

    class channelMention {
        std::optional<long> id;
        std::optional<long> guildId;
        std::optional<int> type;
        std::optional<std::string> name;
    };

    class forumTag {
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<bool> moderated;
        std::optional<long> emojiId;
        std::optional<std::string> emojiName;
    };

    class defaultReaction {
    public:
        std::optional<long> emojiId;
        std::optional<std::string> emojiName;
    };

    class threadMetadata {
    public:
        std::optional<bool> archived;
        std::optional<int> autoArchiveDuration;
        std::optional<std::string> archiveTimestamp;
        std::optional<bool> locked;
        std::optional<bool> invitable;
        std::optional<std::string> createTimestamp;
    };

    class threadMember {
    public:
        std::optional<long> id;
        std::optional<long> userId;
        std::optional<std::string> joinTimestamp;
        std::optional<int> flags;
        guildMember member;
    };

    class overwrite {
        std::optional<long> id;
        std::optional<int> type;
        std::optional<std::string> allow;
        std::optional<std::string> deny;
    };

    class followedChannel {
        std::optional<long> channelId;
        std::optional<long> webhookId;
    };

    class channel {
    public:
        std::optional<long> id;
        std::optional<int> type;
        std::optional<long> guildId;
        std::optional<int> position;
        std::unordered_map<long, overwrite> permissionOverwrites;
        std::optional<std::string> name;
        std::optional<std::string> topic;
        std::optional<bool> nsfw;
        std::optional<long> lastMessageId;
        std::optional<int> bitrate;
        std::optional<int> userLimit;
        std::optional<int> rateLimitPerUser;
        std::unordered_map<long, user> recipients;
        std::optional<std::string> icon;
        std::optional<long> ownerId;
        std::optional<long> applicationId;
        std::optional<long> parentId;
        std::optional<std::string> lastPinTimestamp;
        std::optional<std::string> rtcRegion;
        std::optional<int> videoQualityMode;
        std::optional<int> messageCount;
        std::optional<int> memberCount;
        threadMetadata thread_metadata;
        threadMember member;
        std::optional<int> defaultAutoArchiveDuration;
        std::optional<std::string> permissions;
        std::optional<int> flags;
        std::optional<int> totalMessageSent;
        std::unordered_map<long, tag> availableTags;
        std::vector<long> appliedTags;
        defaultReaction defaultReactionEmoji;
        std::optional<int> defaultThreadRateLimitPerUse;
        std::optional<int> defaultSortOrder;
        std::optional<int> defaultForumLayout;
    };
} // helios

#endif //HELIOS_CHANNEL_HPP
