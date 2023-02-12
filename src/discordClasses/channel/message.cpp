#include "discordClasses/channel/message.hpp"
#include "discordClasses/channel/channel.hpp"
namespace helios {
    reaction reaction::getReactionData(const nlohmann::json &jsonData) {
        reaction reactionData;
        reactionData.count = jsonData["count"];
        reactionData.me = jsonData["me"];
        //reactionData.emoji = emoji::getEmojiData(jsonData["emoji"]);
        return reactionData;
    }

    messageActivity messageActivity::getMessageActivityData(const nlohmann::json &jsonData) {
        messageActivity messageActivityData;
        messageActivityData.type = jsonData["type"];
        if(jsonData.contains("party_it")) messageActivityData.partyId = jsonData["party_id"];
        return messageActivityData;
    }

    messageReference messageReference::getMessageReferenceData(const nlohmann::json &jsonData) {
        messageReference messageReferenceData;
        if(jsonData.contains("message_id")) messageReferenceData.messageId = std::stol(jsonData["message_id"].get<std::string>());
        if(jsonData.contains("channel_id")) messageReferenceData.channelId = std::stol(jsonData["channel_id"].get<std::string>());
        if(jsonData.contains("guild_id")) messageReferenceData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
        if(jsonData.contains("fail_if_not_exists")) messageReferenceData.failIfNotExists = jsonData["fail_if_not_exists"];
        return messageReferenceData;
    }

    messageReference::messageReference(const long &messageId, const long &guildId, const long &channelId, const bool &failIfNotExists) {
        this->messageId = messageId;
        this->guildId = guildId;
        if(channelId != -1) this->channelId = channelId;
        this->failIfNotExists = failIfNotExists;
    }

    roleSubscriptionData roleSubscriptionData::getRoleSubscriptionDataData(const nlohmann::json &jsonData) {
        roleSubscriptionData roleSubscriptionDataData;
        roleSubscriptionDataData.roleSubscriptionListingId = std::stol(jsonData["role_subscription_data_id"].get<std::string>());
        roleSubscriptionDataData.tierName = jsonData["tier_name"];
        roleSubscriptionDataData.totalMonthsSubscribed = jsonData["total_months_subscribed"];
        roleSubscriptionDataData.isRenewal = jsonData["is_renewal"];
        return roleSubscriptionDataData;
    }

    message message::getMessageData(const nlohmann::json &jsonData) {
        message messageData;
        messageData.id = std::stol(jsonData["id"].get<std::string>());
        messageData.channelId = std::stol(jsonData["channel_id"].get<std::string>());
        messageData.author = user::getUserData(jsonData["author"]);
        messageData.content = jsonData["content"];
        messageData.timestamp = jsonData["timestamp"];
        if(!jsonData["edited_timestamp"].is_null()) messageData.editedTimestamp = jsonData["edited_timestamp"];
        messageData.tts = jsonData["tts"];
        messageData.mentionEveryone = jsonData["mention_everyone"];


        for (const nlohmann::basic_json<>& mention: jsonData["mentions"]) {
            messageData.mentions[std::stol(mention["id"].get<std::string>())] = user::getUserData(mention);
        }

        for (const nlohmann::basic_json<> &mentionRole: jsonData["mention_roles"]) {
            messageData.mentionRoles[std::stol(mentionRole["id"].get<std::string>())] = role::getRoleData(mentionRole);
        }

        if(jsonData.contains("mention_channels")) {
            for (const nlohmann::basic_json<> &mentionChannel: jsonData["mention_channels"]) {
                messageData.mentionChannels[std::stol(
                        mentionChannel["id"].get<std::string>())] = channelMention::getChannelMentionData(
                        mentionChannel);
            }
        }

        if(jsonData.contains("attachments")) {
            for (const nlohmann::basic_json<> &attachment: jsonData["attachments"]) {
                messageData.attachments.emplace_back(attachment::getAttachmentData(attachment));
            }
        }

        if(jsonData.contains("embeds")) {
            for (const nlohmann::basic_json<> &embed: jsonData["embeds"]) {
                messageData.embeds.emplace_back(embed::getEmbedData(embed));
            }
        }

        if(jsonData.contains("reactions")) {
            for (const nlohmann::basic_json<> &reaction: jsonData["reactions"]) {
                messageData.reactions.emplace_back(reaction::getReactionData(reaction));
            }
        }

        if(jsonData.contains("nonce")) messageData.nonce = jsonData["nonce"];
        messageData.pinned = jsonData["pinned"];
        if(jsonData.contains("webhook_id")) messageData.webhookId = std::stol(jsonData["webhook_id"].get<std::string>());
        messageData.type = jsonData["type"];
        if(jsonData.contains("activity")) messageData.activity = messageActivity::getMessageActivityData(jsonData["activity"]);
        if(jsonData.contains("application")) messageData.application = application::getApplicationData(jsonData["application"]);
        if(jsonData.contains("application_id")) messageData.applicationId = std::stol(jsonData["application_id"].get<std::string>());
        if(jsonData.contains("message_reference")) messageData.messageReference = messageReference::getMessageReferenceData(jsonData["message_reference"]);
        if(jsonData.contains("flags")) messageData.flags = jsonData["flags"];
        if(jsonData.contains("referenced_message") && !jsonData["referenced_message"].is_null()) messageData.referencedMessage = std::make_unique<message>(getMessageData(jsonData["referenced_message"]));
        if(jsonData.contains("interaction")) messageData.interaction = messageInteraction::getMessageInteractionData(jsonData["interaction"]);
        if(jsonData.contains("thread")) messageData.thread = std::make_unique<channel>(channel::getChannelData(jsonData["thread"]));

        for (const nlohmann::basic_json<>& component: jsonData["components"]) {
            //messageData.components.emplace_back(messageComponent::getMessageComponentData(component));
        }

        if(jsonData.contains("sticker_items")) {
            for (const nlohmann::basic_json<>& stickerItem: jsonData["sticker_items"]) {
                messageData.stickerItems[std::stol(stickerItem["id"].get<std::string>())] = messageStickerItem::getMessageStickerItemData(stickerItem);
            }
        }

        if(jsonData.contains("position")) messageData.position = jsonData["position"];
        if(jsonData.contains("role_subscription_data")) messageData.roleSubscriptionData = roleSubscriptionData::getRoleSubscriptionDataData(jsonData["role_subscription_data"]);
        return messageData;
    }

    message::message(const std::string &messageContent) {
        this->content = messageContent;
    }
} // helios