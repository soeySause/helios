#ifndef HELIOS_CHANNEL_HPP
#define HELIOS_CHANNEL_HPP
#include <optional>
#include <unordered_map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../guild/guildMember.hpp"
#include "../user.hpp"
#include "../../request.hpp"
#include "token.hpp"

using nlohmann::json;
namespace helios {
    class message;
    class channelMention {
    private:
        friend class message;
        static channelMention getChannelMentionData(const json& jsonData);
        std::optional<long> id;
        std::optional<long> guildId;
        std::optional<int> type;
        std::optional<std::string> name;
    };

    class forumTag {
    private:
        friend class channel;
        static forumTag getForumTagData(const json& jsonData);
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<bool> moderated;
        std::optional<long> emojiId;
        std::optional<std::string> emojiName;
    };

    class defaultReaction {
    private:
        friend class channel;
        static defaultReaction getDefaultReactionData(const json& jsonData);
    public:
        std::optional<long> emojiId;
        std::optional<std::string> emojiName;
    };

    class threadMetadata {
    private:
        friend class channel;
        static threadMetadata getThreadMetadataData(const json& jsonData);
    public:
        std::optional<bool> archived;
        std::optional<int> autoArchiveDuration;
        std::optional<std::string> archiveTimestamp;
        std::optional<bool> locked;
        std::optional<bool> invitable;
        std::optional<std::string> createTimestamp;
    };

    class threadMember {
    private:
        friend class channel;
        static threadMember getThreadMemberData(const json& jsonData);
    public:
        std::optional<long> id;
        std::optional<long> userId;
        std::optional<std::string> joinTimestamp;
        std::optional<int> flags;
        guildMember member;
    };

    class overwrite {
    private:
        friend class channel;
        static overwrite getOverwriteData(const json& jsonData);
    public:
        std::optional<long> id;
        std::optional<std::string> type;
        std::optional<std::string> allow;
        std::optional<std::string> deny;
    };

    class followedChannel {
        std::optional<long> channelId;
        std::optional<long> webhookId;
    };

    class channel {
    private:
        friend class message;
        friend class channelOptions;
        friend class eventData;
        static channel getChannelData(const json& jsonData);
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
        std::unordered_map<long, forumTag> availableTags;
        std::vector<long> appliedTags;
        defaultReaction defaultReactionEmoji;
        std::optional<int> defaultThreadRateLimitPerUse;
        std::optional<int> defaultSortOrder;
        std::optional<int> defaultForumLayout;

        [[maybe_unused]] channel modify(const channel& newChannel = helios::channel(), const int& channelType = 0, const std::string& reason = "");
        [[maybe_unused]] channel close(const std::string& reason = "");
        [[maybe_unused]] message getMessages(const std::vector<message>& messages);
        [[maybe_unused]] message getMessage(const message& message);

    };

    class channelOptions {
    public:
        [[maybe_unused]] channel create(const channel& channelOptions);
        [[maybe_unused]] [[nodiscard]] static channel get(const long& channelId, const bool& cacheObject = true) ;
        [[maybe_unused]] [[nodiscard]] channel getFromCache(const long& channelId) const;
        [[maybe_unused]] bool existsInCache(const long& guildId) const;
    };

} // helios

#endif //HELIOS_CHANNEL_HPP
