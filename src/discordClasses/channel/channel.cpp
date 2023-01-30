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
        if(jsonData.contains("default_thread_rate_limit_per_use")) channelData.defaultThreadRateLimitPerUse = jsonData["default_thread_rate_limit_per_use"];
        if(jsonData.contains("default_sort_order") && !jsonData["default_sort_order"].is_null()) channelData.defaultSortOrder = jsonData["default_sort_order"];
        if(jsonData.contains("default_forum_layout")) channelData.defaultForumLayout = jsonData["default_forum_layout"];

        return channelData;
    }

    channel channelOptions::get(const long &channelId, const bool &cacheObject) {
        return channel::getChannelData(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(channelId), "", "get", token::botToken));
    }

    channel channelOptions::create(const channel &channelOptions) {
        return channel();
    }

    channel channelOptions::getFromCache(const long &channelId) const {
        return channel();
    }

    [[maybe_unused]] channel channel::modify(const channel& newChannel, const int& channelType, const std::string& reason) {
        json modifyChannelJson;

        if(channelType == 1) {
            if(newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
            if(newChannel.icon.has_value()) modifyChannelJson["icon"] = newChannel.icon.value();
        }

        if(channelType == 0) {
            if (newChannel.name.has_value()) modifyChannelJson["name"] = newChannel.name.value();
            if (newChannel.type.has_value()) modifyChannelJson["type"] = newChannel.type.value();
            if (newChannel.position.has_value()) modifyChannelJson["position"] = newChannel.position.value();
            if (newChannel.topic.has_value()) modifyChannelJson["topic"] = newChannel.topic.value();
            if (newChannel.nsfw.has_value()) modifyChannelJson["nsfw"] = newChannel.nsfw.value();
            if (newChannel.rateLimitPerUser.has_value()) modifyChannelJson["rate_limit_per_user"] = newChannel.rateLimitPerUser.value();
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

            if (newChannel.defaultThreadRateLimitPerUse.has_value()) modifyChannelJson["defaultThreadRateLimitPerUse"] = newChannel.defaultSortOrder.value();
            if (newChannel.defaultSortOrder.has_value()) modifyChannelJson["default_sort_order"] = newChannel.defaultSortOrder.value();
            if (newChannel.defaultForumLayout.has_value()) modifyChannelJson["default_forum_layout"] = newChannel.defaultForumLayout.value();
        }

        if(channelType == 11 || channelType == 12) {
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
        }

        return channel::getChannelData(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()), modifyChannelJson.dump(), "patch", token::botToken, reason));
    }
    [[maybe_unused]] channel channel::close(const std::string &reason) {
        return channel::getChannelData(request::httpsRequest("discord.com", "/api/channels/" + std::to_string(this->id.value()), "", "delete", token::botToken, reason));
    }

} // helios