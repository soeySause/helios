#include "discordClassses/channel/channel.hpp"

namespace helios {
    channelMention channelMention::getChannelMentionData(const json &jsonData) {
        channelMention channelMentionData;
        channelMentionData.id = std::stol(jsonData["id"].get<std::string>());
        channelMentionData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
        channelMentionData.type = jsonData["type"];
        channelMentionData.name = jsonData["name"];
        return channelMentionData;
    }

    overwrite overwrite::getOverwriteData(const json &jsonData) {
        overwrite overwriteData;
        overwriteData.id = std::stol(jsonData["id"].get<std::string>());
        overwriteData.type = jsonData["type"];
        overwriteData.allow = jsonData["allow"].dump();
        overwriteData.deny = jsonData["deny"].dump();
        return overwriteData;
    }

    threadMetadata threadMetadata::getThreadMetadataData(const json &jsonData) {
        threadMetadata threadMetadataData;
        threadMetadataData.archived = jsonData["archived"];
        threadMetadataData.autoArchiveDuration = jsonData["auto_archive_duration"];
        threadMetadataData.archiveTimestamp = jsonData["archive_timestamp"];
        threadMetadataData.locked = jsonData["locked"];
        if(jsonData.contains("invitable")) threadMetadataData.invitable = jsonData["invitable"];
        if(jsonData.contains("create_timestamp") && !jsonData["create_timestamp"].is_null()) threadMetadataData.createTimestamp = jsonData["create_timestamp"];
        return threadMetadataData;
    }

    threadMember threadMember::getThreadMemberData(const json &jsonData) {
        threadMember threadMemberData;
        if(jsonData.contains("id")) threadMemberData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("user_id")) threadMemberData.userId = std::stol(jsonData["user_id"].get<std::string>());
        threadMemberData.joinTimestamp = jsonData["join_timestamp"];
        threadMemberData.flags = jsonData["flags"];
        //if(jsonData.contains("member")) threadMemberData.member = guildMember::getGuildMemberData(jsonData["member"]);
        return threadMemberData;
    }

    forumTag forumTag::getForumTagData(const json &jsonData) {
        forumTag tagData;
        tagData.id = std::stol(jsonData["id"].get<std::string>());
        tagData.name = jsonData["name"];
        tagData.moderated = jsonData["moderated"];
        if(!jsonData["emoji_id"].is_null()) tagData.emojiId = std::stol(jsonData["id"].get<std::string>());
        if(!jsonData["emoji_name"].is_null()) tagData.emojiName = jsonData["emoji_name"];
        return tagData;
    }
    defaultReaction defaultReaction::getDefaultReactionData(const json &jsonData) {
        defaultReaction defaultReactionData;
        if(!jsonData["emoji_id"].is_null()) defaultReactionData.emojiId = std::stol(jsonData["emoji_id"].get<std::string>());
        if(!jsonData["emoji_name"].is_null()) defaultReactionData.emojiName = jsonData["emoji_name"];
        return defaultReactionData;
    }

    channel channel::getChannelData(const json &jsonData) {
        channel channelData;
        channelData.id = std::stol(jsonData["id"].get<std::string>());
        channelData.type = jsonData["type"];
        if(jsonData.contains("guild_id")) channelData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
        if(jsonData.contains("position")) channelData.position = jsonData["position"];

        if(jsonData.contains("permission_overwrites")) {
            for (const nlohmann::basic_json<>& overwrite: jsonData["permission_overwrites"]) {
                channelData.permissionOverwrites[std::stol(overwrite["id"].get<std::string>())] = overwrite::getOverwriteData(overwrite);
            }
        }

        if(jsonData.contains("name") && !jsonData["name"].is_null()) channelData.name = jsonData["name"];
        if(jsonData.contains("topic") && !jsonData["topic"].is_null()) channelData.topic = jsonData["topic"];
        if(jsonData.contains("nsfw")) channelData.nsfw = jsonData["nsfw"];
        if(jsonData.contains("last_message_id") && !jsonData["last_message_id"].is_null()) channelData.lastMessageId = std::stol(jsonData["last_message_id"].get<std::string>());
        if(jsonData.contains("bitrate")) channelData.bitrate = jsonData["bitrate"];
        if(jsonData.contains("user_limit")) channelData.userLimit = jsonData["user_limit"];
        if(jsonData.contains("rate_limit_per_user")) channelData.rateLimitPerUser = jsonData["rate_limit_per_user"];
        if(jsonData.contains("recipients")) {
            for (const nlohmann::basic_json<>& recipient: jsonData["recipient"]) {
                if(recipient["id"].empty()) continue;
                channelData.recipients[std::stol(recipient["id"].get<std::string>())] = user::getUserData(recipient);
            }
        }

        if(jsonData.contains("icon") && !jsonData["icon"].is_null()) channelData.icon = jsonData["icon"];
        if(jsonData.contains("owner_id")) channelData.ownerId = std::stol(jsonData["owner_id"].get<std::string>());
        if(jsonData.contains("application_id")) channelData.applicationId = std::stol(jsonData["application_id"].get<std::string>());
        if(jsonData.contains("parent_id") && !jsonData["parent_id"].is_null()) channelData.parentId = std::stol(jsonData["parent_id"].get<std::string>());
        if(jsonData.contains("last_pin_timestamp") && !jsonData["last_pin_timestamp"].is_null()) channelData.lastPinTimestamp = jsonData["last_pin_timestamp"];
        if(jsonData.contains("rtc_region") && !jsonData["rtc_region"].is_null()) channelData.rtcRegion = jsonData["rtc_region"];
        if(jsonData.contains("video_quality_mode")) channelData.videoQualityMode = jsonData["video_quality_mode"];
        if(jsonData.contains("message_count")) channelData.messageCount = jsonData["message_count"];
        if(jsonData.contains("member_count")) channelData.memberCount = jsonData["member_count"];
        if(jsonData.contains("thread_metadata")) channelData.thread_metadata = threadMetadata::getThreadMetadataData(jsonData["thread_metadata"]);
        if(jsonData.contains("member")) channelData.member = threadMember::getThreadMemberData(jsonData["member"]);
        if(jsonData.contains("default_auto_archive_duration")) channelData.defaultAutoArchiveDuration = jsonData["default_auto_archive_duration"];
        if(jsonData.contains("permissions")) channelData.permissions = jsonData["permissions"];
        if(jsonData.contains("flags")) channelData.flags = jsonData["flags"];
        if(jsonData.contains("total_message_sent")) channelData.totalMessageSent = jsonData["total_message_sent"];
        if(jsonData.contains("available_tags")) {
            for (const nlohmann::basic_json<>& tag: jsonData["available_tags"]) {
                if(tag["id"].empty()) continue;
                channelData.availableTags[std::stol(tag["id"].get<std::string>())] = forumTag::getForumTagData(tag);
            }
        }

        if(jsonData.contains("applied_tags")) {
            for (const nlohmann::basic_json<>& tag: jsonData["applied_tags"]) {
                channelData.appliedTags.emplace_back(tag);
            }
        }

        if(jsonData.contains("default_reaction_emoji") && !jsonData["default_reaction_emoji"].is_null()) channelData.defaultReactionEmoji = defaultReaction::getDefaultReactionData(jsonData["default_reaction_emoji"]);
        if(jsonData.contains("default_thread_rate_limit_per_use")) channelData.defaultThreadRateLimitPerUser = jsonData["default_thread_rate_limit_per_user"];
        if(jsonData.contains("default_sort_order") && !jsonData["default_sort_order"].is_null()) channelData.defaultSortOrder = jsonData["default_sort_order"];
        if(jsonData.contains("default_forum_layout")) channelData.defaultForumLayout = jsonData["default_forum_layout"];

        return channelData;
    }


    [[maybe_unused]] channel channel::modify(const channel& newChannel, const std::string& reason) {
        assert(this->id.has_value());
        assert(this->type.has_value());
        assert(this->botToken.has_value());

        const int channelType = this->type.value();
        json modifyChannelJson;

        switch (channelType) {
            // group Dm
            case 3: {
                if(newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if(newChannel.icon.has_value()) modifyChannelJson["icon"] = newChannel.icon.value();
                break;
            }
            // thread
            case 11:
            case 12: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.thread_metadata.archived.has_value()) modifyChannelJson["archived"] = newChannel.thread_metadata.archived.value();
                if (newChannel.thread_metadata.autoArchiveDuration.has_value()) modifyChannelJson["auto_archive_duration"] = newChannel.thread_metadata.autoArchiveDuration.value();
                if (newChannel.thread_metadata.locked.has_value()) modifyChannelJson["locked"] = newChannel.thread_metadata.locked.value();
                if (newChannel.thread_metadata.invitable.has_value()) modifyChannelJson["invitable"] = newChannel.thread_metadata.invitable.value();
                if (newChannel.rateLimitPerUser.has_value()) modifyChannelJson["rate_limit_per_user"] = newChannel.rateLimitPerUser.value();
                if (newChannel.flags.has_value()) modifyChannelJson["flags"] = newChannel.flags.value();
                json appliedTagJsonArray = json::array();
                for (const auto &tag: newChannel.appliedTags) {
                    appliedTagJsonArray.emplace_back(tag);
                }
                if (!appliedTagJsonArray.empty()) modifyChannelJson["applied_tags"] = appliedTagJsonArray;
                break;
            }
            // voice
            case 2: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
                if (newChannel.nsfw.has_value()) modifyChannelJson["nsfw"] = newChannel.nsfw.value();
                if (newChannel.bitrate.has_value()) modifyChannelJson["bitrate"] = newChannel.bitrate.value();
                if (newChannel.userLimit.has_value()) modifyChannelJson["user_limit"] = newChannel.userLimit.value();
                json permissionOverwritesJsonArray = json::array();
                for (const auto &[key, value]: newChannel.permissionOverwrites) {
                    json permissionOverwritesJson;
                    if (value.id.has_value()) permissionOverwritesJson["id"] = value.id.value();
                    if (value.type.has_value()) permissionOverwritesJson["type"] = value.type.value();
                    if (value.allow.has_value()) permissionOverwritesJson["allow"] = value.allow.value();
                    if (value.deny.has_value()) permissionOverwritesJson["deny"] = value.deny.value();
                    permissionOverwritesJsonArray.emplace_back(permissionOverwritesJson);
                }
                if (!permissionOverwritesJsonArray.empty()) modifyChannelJson["permission_overwrites"] = permissionOverwritesJsonArray;
                if (newChannel.parentId.has_value()) modifyChannelJson["parent_id"] = newChannel.parentId.value();
                if (newChannel.rtcRegion.has_value()) modifyChannelJson["rtc_region"] = newChannel.rtcRegion.value();
                if (newChannel.videoQualityMode.has_value()) modifyChannelJson["video_quality_mode"] = newChannel.videoQualityMode.value();
                break;
            }

            //stage voice
            case 13: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
                if (newChannel.bitrate.has_value()) modifyChannelJson["bitrate"] = newChannel.bitrate.value();
                json permissionOverwritesJsonArray = json::array();
                for (const auto &[key, value]: newChannel.permissionOverwrites) {
                    json permissionOverwritesJson;
                    if (value.id.has_value()) permissionOverwritesJson["id"] = value.id.value();
                    if (value.type.has_value()) permissionOverwritesJson["type"] = value.type.value();
                    if (value.allow.has_value()) permissionOverwritesJson["allow"] = value.allow.value();
                    if (value.deny.has_value()) permissionOverwritesJson["deny"] = value.deny.value();
                    permissionOverwritesJsonArray.emplace_back(permissionOverwritesJson);
                }
                if (!permissionOverwritesJsonArray.empty()) modifyChannelJson["permission_overwrites"] = permissionOverwritesJsonArray;
                if (newChannel.rtcRegion.has_value()) modifyChannelJson["rtc_region"] = newChannel.rtcRegion.value();
                break;
            }
            // announcement
            case 5: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.type.has_value()) modifyChannelJson["type"] = newChannel.type.value();
                if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
                if (newChannel.topic.has_value()) modifyChannelJson["topic"] = newChannel.topic.value();
                if (newChannel.nsfw.has_value()) modifyChannelJson["nsfw"] = newChannel.nsfw.value();
                json permissionOverwritesJsonArray = json::array();
                for (const auto &[key, value]: newChannel.permissionOverwrites) {
                    json permissionOverwritesJson;
                    if (value.id.has_value()) permissionOverwritesJson["id"] = value.id.value();
                    if (value.type.has_value()) permissionOverwritesJson["type"] = value.type.value();
                    if (value.allow.has_value()) permissionOverwritesJson["allow"] = value.allow.value();
                    if (value.deny.has_value()) permissionOverwritesJson["deny"] = value.deny.value();
                    permissionOverwritesJsonArray.emplace_back(permissionOverwritesJson);
                }
                if (!permissionOverwritesJsonArray.empty()) modifyChannelJson["permission_overwrites"] = permissionOverwritesJsonArray;
                if (newChannel.parentId.has_value()) modifyChannelJson["parent_id"] = newChannel.parentId.value();
                if (newChannel.defaultAutoArchiveDuration.has_value()) modifyChannelJson["default_auto_archive_duration"] = newChannel.defaultAutoArchiveDuration.value();
                break;
            }
            // text channel
            case 0: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.type.has_value()) modifyChannelJson["type"] = newChannel.type.value();
                if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
                if (newChannel.topic.has_value()) modifyChannelJson["topic"] = newChannel.topic.value();
                if (newChannel.nsfw.has_value()) modifyChannelJson["nsfw"] = newChannel.nsfw.value();
                if (newChannel.rateLimitPerUser.has_value()) modifyChannelJson["rate_limit_per_user"] = newChannel.rateLimitPerUser.value();

                json permissionOverwritesJsonArray = json::array();
                for (const auto &[key, value]: newChannel.permissionOverwrites) {
                    json permissionOverwritesJson;
                    if (value.id.has_value()) permissionOverwritesJson["id"] = value.id.value();
                    if (value.type.has_value()) permissionOverwritesJson["type"] = value.type.value();
                    if (value.allow.has_value()) permissionOverwritesJson["allow"] = value.allow.value();
                    if (value.deny.has_value()) permissionOverwritesJson["deny"] = value.deny.value();
                    permissionOverwritesJsonArray.emplace_back(permissionOverwritesJson);
                }
                if (!permissionOverwritesJsonArray.empty()) modifyChannelJson["permission_overwrites"] = permissionOverwritesJsonArray;
                if (newChannel.parentId.has_value()) modifyChannelJson["parent_id"] = newChannel.parentId.value();
                if (newChannel.defaultAutoArchiveDuration.has_value()) modifyChannelJson["default_auto_archive_duration"] = newChannel.defaultAutoArchiveDuration.value();
                if (newChannel.defaultThreadRateLimitPerUser.has_value()) modifyChannelJson["default_thread_rate_limit_per_user"] = newChannel.defaultSortOrder.value();
            }
            // forum
            case 15: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
                json permissionOverwritesJsonArray = json::array();
                for (const auto &[key, value]: newChannel.permissionOverwrites) {
                    json permissionOverwritesJson;
                    if (value.id.has_value()) permissionOverwritesJson["id"] = value.id.value();
                    if (value.type.has_value()) permissionOverwritesJson["type"] = value.type.value();
                    if (value.allow.has_value()) permissionOverwritesJson["allow"] = value.allow.value();
                    if (value.deny.has_value()) permissionOverwritesJson["deny"] = value.deny.value();
                    permissionOverwritesJsonArray.emplace_back(permissionOverwritesJson);
                }
                if (!permissionOverwritesJsonArray.empty()) modifyChannelJson["permission_overwrites"] = permissionOverwritesJsonArray;
                if (newChannel.defaultAutoArchiveDuration.has_value()) modifyChannelJson["default_auto_archive_duration"] = newChannel.defaultAutoArchiveDuration.value();
                if (newChannel.flags.has_value()) modifyChannelJson["flags"] = newChannel.flags.value();

                json availableTagJsonArray = json::array();
                for (const auto &[key, value]: newChannel.availableTags) {
                    json tagJson;
                    if (value.id.has_value()) tagJson["id"] = value.id.value();
                    if (value.name.has_value()) tagJson["name"] = value.name.value();
                    if (value.moderated.has_value()) tagJson["moderated"] = value.moderated.value();
                    if (value.emojiId.has_value()) tagJson["emoji_id"] = value.emojiId.value();
                    if (value.emojiName.has_value()) tagJson["emoji_name"] = value.emojiName.value();
                    availableTagJsonArray.emplace_back(tagJson);
                }
                if (!availableTagJsonArray.empty()) modifyChannelJson["available_tags"] = availableTagJsonArray;
                if (newChannel.defaultReactionEmoji.emojiId.has_value()) modifyChannelJson["default_reaction_emoji"]["emoji_id"] = newChannel.defaultReactionEmoji.emojiId.value();
                if (newChannel.defaultReactionEmoji.emojiName.has_value()) modifyChannelJson["default_reaction_emoji"]["emoji_name"] = newChannel.defaultReactionEmoji.emojiName.value();

                if (newChannel.defaultThreadRateLimitPerUser.has_value()) modifyChannelJson["default_thread_rate_limit_per_user"] = newChannel.defaultSortOrder.value();
                if (newChannel.defaultSortOrder.has_value()) modifyChannelJson["default_sort_order"] = newChannel.defaultSortOrder.value();
                if (newChannel.defaultForumLayout.has_value()) modifyChannelJson["default_forum_layout"] = newChannel.defaultForumLayout.value();

            }
            // default fields
            default: {
                if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
                if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
                if (newChannel.topic.has_value()) modifyChannelJson["topic"] = newChannel.topic.value();
                if (newChannel.nsfw.has_value()) modifyChannelJson["nsfw"] = newChannel.nsfw.value();
                if (newChannel.rateLimitPerUser.has_value()) modifyChannelJson["rate_limit_per_user"] = newChannel.rateLimitPerUser.value();
                json permissionOverwritesJsonArray = json::array();
                for (const auto &[key, value]: newChannel.permissionOverwrites) {
                    json permissionOverwritesJson;
                    if (value.id.has_value()) permissionOverwritesJson["id"] = value.id.value();
                    if (value.type.has_value()) permissionOverwritesJson["type"] = value.type.value();
                    if (value.allow.has_value()) permissionOverwritesJson["allow"] = value.allow.value();
                    if (value.deny.has_value()) permissionOverwritesJson["deny"] = value.deny.value();
                    permissionOverwritesJsonArray.emplace_back(permissionOverwritesJson);
                }
                if (!permissionOverwritesJsonArray.empty()) modifyChannelJson["permission_overwrites"] = permissionOverwritesJsonArray;
                if (newChannel.parentId.has_value()) modifyChannelJson["parent_id"] = newChannel.parentId.value();

                break;
            }
        }
        return channel::getChannelData(json::parse(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()), modifyChannelJson, boost::beast::http::verb::patch, this->botToken.value(), reason)));
    }
    [[maybe_unused]] channel channel::delete_(const std::string &reason) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        return channel::getChannelData(json::parse(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()), {}, boost::beast::http::verb::delete_, this->botToken.value(), reason)));
    }
    [[maybe_unused]] [[nodiscard]] std::unordered_map<long, message> channel::getMessages(const std::vector<std::vector<std::string>>& options) const {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        json payload;
        for(auto& option : options) {
            if(option.at(0) == "limit") {
                payload["limit"] = std::stoi(option.at(1));
            }
            if(option.at(0) == "around") {
                payload["around"] = std::stol(option.at(1));
                break;
            }
            if(option.at(0) == "before") {
                payload["before"] = std::stol(option.at(1));
                break;
            }
            if(option.at(0) == "after") {
                payload["after"] = std::stol(option.at(1));
                break;
            }
        }
        std::unordered_map<long, message> messages;
        for (const nlohmann::basic_json<>& message: json::parse(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages", payload, boost::beast::http::verb::get, this->botToken.value()))) {
            messages[std::stol(message["id"].get<std::string>())] = message::getMessageData(message);
        }
        return messages;
    }
    [[maybe_unused]] [[nodiscard]] message channel::getMessage(const long &messageId) const {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        return message::getMessageData(json::parse(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId), {}, boost::beast::http::verb::get, this->botToken.value())));

    }
    [[maybe_unused]] message channel::createMessage(const message &newMessage) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        json payload;
        if(newMessage.content.has_value()) payload["content"] = newMessage.content.value();
        if(newMessage.nonce.has_value()) payload["nonce"] = newMessage.nonce.value();
        if(newMessage.tts.has_value()) payload["tts"] = newMessage.tts.value();
        for(auto& embed: newMessage.embeds) {
            if (embed.title.has_value()) payload["embed"]["title"] = embed.title.value();
            if (embed.type.has_value()) payload["embed"]["type"] = embed.type.value();
            if (embed.description.has_value()) payload["embed"]["description"] = embed.description.value();
            if (embed.url.has_value()) payload["embed"]["url"] = embed.url.value();
            if (embed.timestamp.has_value()) payload["embed"]["timestamp"] = embed.timestamp.value();
            if (embed.color.has_value()) payload["embed"]["color"] = embed.color.value();

            if (embed.footer.proxyIconUrl.has_value()) payload["embed"]["footer"]["proxy_icon_url"] = embed.footer.proxyIconUrl.value();
            if (embed.footer.iconUrl.has_value()) payload["embed"]["footer"]["icon_url"] = embed.footer.iconUrl.value();
            if (embed.footer.text.has_value()) payload["embed"]["footer"]["text"] = embed.footer.text.value();

            if (embed.image.url.has_value()) payload["embed"]["image"]["url"] = embed.image.url.value();
            if (embed.image.proxyUrl.has_value()) payload["embed"]["image"]["proxy_url"] = embed.image.proxyUrl.value();
            if (embed.image.height.has_value()) payload["embed"]["image"]["height"] = embed.image.height.value();
            if (embed.image.width.has_value()) payload["embed"]["image"]["width"] = embed.image.width.value();

            if (embed.thumbnail.url.has_value()) payload["embed"]["thumbnail"]["url"] = embed.thumbnail.url.value();
            if (embed.thumbnail.proxyUrl.has_value()) payload["embed"]["thumbnail"]["proxy_url"] = embed.thumbnail.proxyUrl.value();
            if (embed.thumbnail.height.has_value()) payload["embed"]["thumbnail"]["height"] = embed.thumbnail.height.value();
            if (embed.thumbnail.width.has_value()) payload["embed"]["thumbnail"]["width"] = embed.thumbnail.width.value();

            if (embed.video.url.has_value()) payload["embed"]["video"]["url"] = embed.video.url.value();
            if (embed.video.proxyUrl.has_value()) payload["embed"]["video"]["proxy_url"] = embed.video.proxyUrl.value();
            if (embed.video.height.has_value()) payload["embed"]["video"]["height"] = embed.video.height.value();
            if (embed.video.width.has_value()) payload["embed"]["video"]["width"] = embed.video.width.value();

            if (embed.provider.name.has_value()) payload["embed"]["provider"]["name"] = embed.provider.name.value();
            if (embed.provider.url.has_value()) payload["embed"]["provider"]["url"] = embed.provider.url.value();

            if (embed.author.url.has_value()) payload["embed"]["author"]["url"] = embed.author.url.value();
            if (embed.author.name.has_value()) payload["embed"]["author"]["name"] = embed.author.name.value();
            if (embed.author.iconUrl.has_value()) payload["embed"]["author"]["icon_url"] = embed.author.iconUrl.value();
            if (embed.author.proxyIconUrl.has_value()) payload["embed"]["author"]["proxy_icon_url"] = embed.author.proxyIconUrl.value();

            json fields = json::array();
            for (auto &field: embed.fields) {
                json jsonField;
                if (field.name.has_value()) jsonField["name"] = field.name.value();
                if (field.value.has_value()) jsonField["value"] = field.value.value();
                if (field.inline_.has_value()) jsonField["inline"] = field.inline_.value();
                fields.emplace_back(jsonField);
            }
            if (!fields.empty()) payload["embed"]["fields"] = fields;
        }
        if(newMessage.messageReference.guildId.has_value()) payload["message_reference"]["guild_id"] = newMessage.messageReference.guildId.value();
        if(newMessage.messageReference.messageId.has_value()) payload["message_reference"]["message_id"] = newMessage.messageReference.messageId.value();
        if(newMessage.messageReference.channelId.has_value()) payload["message_reference"]["channel_id"] = newMessage.messageReference.channelId.value();
        if(newMessage.messageReference.failIfNotExists.has_value()) payload["message_reference"]["fail_if_not_exists"] = newMessage.messageReference.failIfNotExists.value();

        //TODO finish this


        return message::getMessageData(json::parse(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages", payload, boost::beast::http::verb::post, this->botToken.value())));
    }
    [[maybe_unused]] message channel::crosspostMessage(const long &messageId) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        return message::getMessageData(json::parse(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId) + "/crosspost", {}, boost::beast::http::verb::post, this->botToken.value())));
    }
    [[maybe_unused]] void channel::createReaction(const long &messageId, const std::string& emoji) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId) + "/reactions/" + request::url_encode(emoji) + "/@me", {}, boost::beast::http::verb::put, this->botToken.value());
    }
    [[maybe_unused]] void channel::deleteReaction(const long &messageId, const std::string &emoji) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId) + "/reactions/" + request::url_encode(emoji) + "/@me", {}, boost::beast::http::verb::delete_, this->botToken.value());
    }
    [[maybe_unused]] void channel::deleteUserReaction(const long &messageId, const std::string &emoji, const long& userId) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId) + "/reactions/" + request::url_encode(emoji) + "/" + std::to_string(userId), {}, boost::beast::http::verb::delete_, this->botToken.value());
    }
    [[maybe_unused]] [[nodiscard]] std::unordered_map<long, user> channel::getReactions(const long &messageId, const std::string &emoji, std::vector<std::vector<std::string>> &options) const {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

    }
    [[maybe_unused]] void channel::deleteAllReactions(const long &messageId) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId) + "/reactions", {}, boost::beast::http::verb::delete_, this->botToken.value());
    }
    [[maybe_unused]] void channel::deleteAllReactionsForEmoji(const long &messageId, const std::string &emoji) {
        assert(this->id.has_value());
        assert(this->botToken.has_value());

        request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()) + "/messages/" + std::to_string(messageId) + "/reactions/" + request::url_encode(emoji), {}, boost::beast::http::verb::delete_, this->botToken.value());
    }
} // helios