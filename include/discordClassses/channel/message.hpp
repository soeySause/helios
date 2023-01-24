#ifndef HELIOS_MESSAGE_HPP
#define HELIOS_MESSAGE_HPP
#include <optional>
#include <string>
#include <unordered_map>
#include "channel.hpp"
#include "discordClassses/user.hpp"
#include "discordClassses/application.hpp"
#include "role.hpp"
#include "sticker.hpp"
#include "emoji.hpp"
#include "embed.hpp"
#include "attachment.hpp"

namespace helios {
    class reaction {
        std::optional<int> count;
        std::optional<bool> me;
        emoji emoji;
    };

    class messageActivity {
    public:
        std::optional<int> type;
        std::optional<std::string> partyId;
    };

    class messageReference {
        std::optional<long> messageId;
        std::optional<long> channelId;
        std::optional<long> guildId;
        std::optional<bool> failIfNotExists;
    };

    class messageInteraction {

    };

    class messageComponents {

    };

    class messageStickerItem {

    };

    class roleSubscriptionData {

    };

    class message {
    public:
        std::optional<long> id;
        std::optional<long> channelId;
        user author;
        std::optional<std::string> content;
        std::optional<std::string> timestamp;
        std::optional<std::string> editedTimestamp;
        std::optional<bool> tts;
        std::optional<bool> mentionEveryone;
        std::unordered_map<long, user> mentions;
        std::unordered_map<long, role> mentionRoles;
        std::unordered_map<long, channelMention> mentionChannels;
        std::unordered_map <long, attachment> attachments;
        std::unordered_map <long, embed> embeds;
        std::unordered_map <long, reaction> reactions;
        std::optional<std::string> nonce;
        std::optional<bool> pinned;
        std::optional<long> webhookId;
        std::optional<int> type;
        messageActivity activity;
        application application;
        std::optional<long> applicationId;
        messageReference messageReference;
        std::optional<int> flags;
        message referencedMessage;
        messageInteraction interaction;
        channel thread;
        std::optional<messageComponents> components;
        messageStickerItem stickerItems;
        sticker stickers;
        std::optional<int> position;
        roleSubscriptionData
    };
} // helios

#endif //HELIOS_MESSAGE_HPP
