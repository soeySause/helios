#ifndef HELIOS_MESSAGE_HPP
#define HELIOS_MESSAGE_HPP
#include <optional>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "../user.hpp"
#include "../application.hpp"
#include "../guild/role.hpp"
#include "../guild/guildMember.hpp"
#include "../sticker.hpp"
#include "../emoji.hpp"
#include "../interactions/messageComponent.hpp"
#include "../interactions/interaction.hpp"
#include "embed.hpp"
#include "attachment.hpp"
#include "channel.hpp"

namespace helios {
    class channel;
    class reaction {
    private:
        friend class message;
        static reaction getReactionData(const nlohmann::json& jsonData);
    public:
        std::optional<int> count;
        std::optional<bool> me;
        emoji emoji;
    };

    class messageActivity {
    private:
        friend class message;
        static messageActivity getMessageActivityData(const nlohmann::json& jsonData);
    public:
        std::optional<int> type;
        std::optional<std::string> partyId;
    };

    class messageReference {
    private:
        friend class message;
        static messageReference getMessageReferenceData(const nlohmann::json& jsonData);
    public:
        std::optional<long> messageId;
        std::optional<long> channelId;
        std::optional<long> guildId;
        std::optional<bool> failIfNotExists;
    };

    class roleSubscriptionData {
    private:
        friend class message;
        static roleSubscriptionData getRoleSubscriptionDataData(const nlohmann::json& jsonData);
    public:
        std::optional<long> roleSubscriptionListingId;
        std::optional<std::string> tierName;
        std::optional<int> totalMonthsSubscribed;
        std::optional<bool> isRenewal;
    };

    class message {
    private:
        static message getMessageData(const nlohmann::json& jsonData);
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
        std::vector <embed> embeds;
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
        std::unique_ptr<message> referencedMessage;
        messageInteraction interaction;
        channel thread;
        std::vector<messageComponent> components;
        messageStickerItem stickerItems;
        std::unordered_map<long, sticker> stickers;
        std::optional<int> position;
        roleSubscriptionData roleSubscriptionData;
    };
} // helios

#endif //HELIOS_MESSAGE_HPP
