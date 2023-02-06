#ifndef HELIOS_CHANNEL_HPP
#define HELIOS_CHANNEL_HPP
#include <optional>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/beast/core/string.hpp>
#include <nlohmann/json.hpp>
#include "../guild/guildMember.hpp"
#include "../user.hpp"
#include "../invite.hpp"
#include "request.hpp"
#include "message.hpp"

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
        friend class client;
        friend class message;
        friend class eventData;

        std::optional<std::string> botToken;
        std::optional<int> shard;
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
        std::optional<int> defaultThreadRateLimitPerUser;
        std::optional<int> defaultSortOrder;
        std::optional<int> defaultForumLayout;

        [[maybe_unused]] channel modify(const channel& newChannel = helios::channel(), const std::string& reason = "");
        [[maybe_unused]] channel delete_(const std::string& reason = "");
        [[maybe_unused]] [[nodiscard]] std::unordered_map<long, message> getMessages(const std::map<std::string, std::string>& options = {}) const;
        [[maybe_unused]] [[nodiscard]] message getMessage(const long& messageId) const;
        [[maybe_unused]] message createMessage(const message& newMessage = helios::message());
        [[maybe_unused]] message createMessage(const std::string& message);
        [[maybe_unused]] message createMessage(const embed& embed);
        [[maybe_unused]] message createMessage(const std::vector<embed>& embeds);
        [[maybe_unused]] message createMessage(const attachment& attachment);
        [[maybe_unused]] message createMessage(const std::vector<attachment>& attachments);
        [[maybe_unused]] message createMessage(const long& stickerId);
        [[maybe_unused]] message createMessage(const std::vector<long>& stickerIds);
        [[maybe_unused]] message createMessage(const messageComponent& messageComponent);

        [[maybe_unused]] message crosspostMessage(const long& messageId);
        [[maybe_unused]] void createReaction(const long& messageId, const std::string& emoji);
        [[maybe_unused]] void deleteReaction(const long& messageId, const std::string& emoji);
        [[maybe_unused]] void deleteUserReaction(const long& messageId, const std::string& emoji, const long& userId);
        [[maybe_unused]] std::unordered_map<long, user> getReactions(const long& messageId, const std::string& emoji, std::vector<std::vector<std::string>>& options) const;
        [[maybe_unused]] void deleteAllReactions(const long& messageId);
        [[maybe_unused]] void deleteAllReactionsForEmoji(const long& messageId, const std::string& emoji);

        [[maybe_unused]] message editMessage(const long& messageId, const message& newMessage = helios::message());
        [[maybe_unused]] message editMessage(const long& messageId, const std::string& message);
        [[maybe_unused]] message editMessage(const long& messageId, const embed& embed);
        [[maybe_unused]] message editMessage(const long& messageId, const std::vector<embed>& embeds);
        [[maybe_unused]] message editMessage(const long& messageId, const attachment& attachment);
        [[maybe_unused]] message editMessage(const long& messageId, const std::vector<attachment>& attachments);
        [[maybe_unused]] message editMessage(const long& messageId, const messageComponent& messageComponent);

        [[maybe_unused]] void deleteMessage(const long& messageId, const std::string& reason = "");
        [[maybe_unused]] void bulkDeleteMessage(const std::vector<long>& messageIdsVector, const std::string& reason = "");
        [[maybe_unused]] void editPermissions(const overwrite& overwrite, const std::string& reason = "");
        [[maybe_unused]] [[nodiscard]] invite getInvites();


    };
} // helios

#endif //HELIOS_CHANNEL_HPP
