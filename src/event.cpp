#include "event.hpp"

namespace helios {

    roleTags eventData::getRoleTagsData(const json &jsonData) {
        roleTags roleTagsData;
        if(jsonData.contains("bot_id")) {roleTagsData.botId = std::stol(jsonData["bot_id"].get<std::string>()); }
        if(jsonData.contains("integration_id")) {roleTagsData.integrationId = std::stol(jsonData["integration_id"].get<std::string>()); }
        if(jsonData.contains("premium_subscriber")) {if (jsonData["premium_subscriber"].is_null()) {
            roleTagsData.premiumSubscriber = false;
            } else {roleTagsData.premiumSubscriber = true; }
        }
        if(jsonData.contains("subscription_listing_id")) {roleTagsData.subscriptionListing_id = std::stol(jsonData["subscription_listing_id"].get<std::string>()); }

        if(jsonData.contains("guild_connections")) {if (jsonData["guild_connections"].is_null()) {
                roleTagsData.guildConnections = false;
            } else {roleTagsData.guildConnections = true; }
        }

        if(jsonData.contains("available_for_purchase")) {if (jsonData["available_for_purchase"].is_null()) {
                roleTagsData.availableForPurchase = false;
            } else {roleTagsData.availableForPurchase = true; }
        }
        return roleTagsData;
    }

    role eventData::getRoleData(const json &jsonData) {
        role roleData;
        if(jsonData.contains("id")) {roleData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("name")) {roleData.name = jsonData["name"]; }
        if(jsonData.contains("color")) {roleData.color = jsonData["color"]; }
        if(jsonData.contains("hoist")) {roleData.hoist = jsonData["hoist"]; }
        if(jsonData.contains("icon") && !jsonData["icon"].is_null()) {roleData.icon = jsonData["icon"]; }
        if(jsonData.contains("unicode_emoji") && !jsonData["unicode_emoji"].is_null()) {roleData.unicodeEmoji = jsonData["unicode_emoji"]; }
        if(jsonData.contains("position")) {roleData.position = jsonData["position"]; }
        if(jsonData.contains("permissions")) {roleData.permissions = jsonData["permissions"].dump(); }
        if(jsonData.contains("managed")) {roleData.managed = jsonData["managed"]; }
        if(jsonData.contains("mentionable")) {roleData.mentionable = jsonData["mentionable"]; }
        if(jsonData.contains("tags")) {roleData.tags = getRoleTagsData(jsonData["tags"]); }
        return roleData;
    }

    emoji eventData::getEmojiData(const json& jsonData) {
        emoji emojiData;
        if(jsonData.contains("id") && !jsonData["id"].is_null()) {emojiData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("name") && !jsonData["name"].is_null()) {emojiData.name = jsonData["name"]; }

        if(jsonData.contains("roles")) {
            for (const nlohmann::basic_json<> &roleSnowflake: jsonData["roles"]) {
                emojiData.roles.emplace_back(std::stol(roleSnowflake.get<std::string>()));
            }
        }


    if(jsonData.contains("user")) { emojiData.user = user::getUserData(jsonData["user"]); }
        if(jsonData.contains("require_colons")) {emojiData.require_colons = jsonData["require_colons"]; }
        if(jsonData.contains("managed")) {emojiData.managed = jsonData["managed"]; }
        if(jsonData.contains("animated")) {emojiData.animated = jsonData["animated"]; }
        if(jsonData.contains("available")) {emojiData.available = jsonData["available"]; }
        return emojiData;
    }

    welcomeScreenChannelStructure eventData::getWelcomeScreenChannelStructureData(const json& jsonData) {
        welcomeScreenChannelStructure welcomeScreenChannelStructureData;
        if(jsonData.contains("channel_id")) welcomeScreenChannelStructureData.channelId = std::stol(jsonData["channel_id"].get<std::string>());
        if(jsonData.contains("description")) welcomeScreenChannelStructureData.description = jsonData["description"];
        if(jsonData.contains("emoji_id") && !jsonData["emoji_id"].is_null()) welcomeScreenChannelStructureData.emojiId = std::stol(jsonData["integration_id"].get<std::string>());
        if(jsonData.contains("emoji_name") && !jsonData["emoji_name"].is_null()) welcomeScreenChannelStructureData.emojiName = jsonData["emoji_name"];
        return welcomeScreenChannelStructureData;
    }

    welcomeScreen eventData::getWelcomeScreenData(const json& jsonData) {
        welcomeScreen welcomeScreenData;
        if(jsonData.contains("description") && !jsonData["description"].is_null()) {welcomeScreenData.description = jsonData["description"]; }

        if(jsonData.contains("welcome_channels")) {
            for(const nlohmann::basic_json<>& channels : jsonData["welcome_channels"]) {
                welcomeScreenData.welcome_channels.emplace_back(getWelcomeScreenChannelStructureData(channels));
            }
        }

        return welcomeScreenData;
    }

    sticker eventData::getStickerData(const json &jsonData) {
        sticker stickerData;
        if(jsonData.contains("id")) { stickerData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("pack_id")) { stickerData.packId = std::stol(jsonData["pack_id"].get<std::string>()); }
        if(jsonData.contains("name")) { stickerData.name = jsonData["name"]; }
        if(jsonData.contains("description") && !jsonData["description"].is_null()) { stickerData.description = jsonData["description"]; }
        if(jsonData.contains("tags")) { stickerData.tags = jsonData["tags"]; }
        if(jsonData.contains("asset")) { stickerData.asset = jsonData["asset"]; }
        if(jsonData.contains("type")) { stickerData.type = jsonData["type"]; }
        if(jsonData.contains("format_type")) { stickerData.formatType = jsonData["format_type"]; }
        if(jsonData.contains("available")) { stickerData.available = jsonData["available"]; }
        if(jsonData.contains("guild_id")) { stickerData.guildId = std::stol(jsonData["guild_id"].get<std::string>()); }
        if(jsonData.contains("user")) { stickerData.user = user::getUserData("user"); }
        if(jsonData.contains("sort_value")) { stickerData.sortValue = jsonData["sort_value"]; }
        return stickerData;
    }

    guildMember eventData::getGuildMemberData(const json &jsonData, const long& guildId) {
        guildMember guildMemberData;
        guildMemberData.guildId = guildId;
        if(jsonData.contains("user")) guildMemberData.user = user::getUserData(jsonData["user"]);
        if(jsonData.contains("nick") && !jsonData["nick"].is_null()) guildMemberData.nick = jsonData["nick"];
        if(jsonData.contains("avatar") && !jsonData["avatar"].is_null()) guildMemberData.avatar = jsonData["avatar"];
        if(jsonData.contains("roles")) {
            for (const nlohmann::basic_json<> &roleSnowflake: jsonData["roles"]) {
                guildMemberData.roles.emplace_back(std::stol(roleSnowflake.get<std::string>()));
            }
        }

        if(jsonData.contains("joined_at")) guildMemberData.joinedAt = jsonData["joined_at"];
        if(jsonData.contains("premium_since") && !jsonData["premium_since"].is_null()) guildMemberData.premiumSince = jsonData["premium_since"];
        if(jsonData.contains("deaf")) guildMemberData.deaf = jsonData["deaf"];
        if(jsonData.contains("mute")) guildMemberData.mute = jsonData["mute"];
        if(jsonData.contains("pending")) guildMemberData.pending = jsonData["pending"];
        if(jsonData.contains("permissions")) guildMemberData.permissions = jsonData["permissions"];
        if(jsonData.contains("communication_disabled_until") && !jsonData["communication_disabled_until"].is_null()) guildMemberData.communicationDisabledUntil = jsonData["communication_disabled_until"];
        return guildMemberData;
    }

    voiceState eventData::getVoiceStateData(const json &jsonData, const long& guildId) {
        voiceState voiceStateData;
        if(jsonData.contains("guild_id")) voiceStateData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
        if(jsonData.contains("channel_id") && !jsonData["channel_id"].is_null()) voiceStateData.channelId = std::stol(jsonData["channel_id"].get<std::string>());
        if(jsonData.contains("user_id")) voiceStateData.userId = std::stol(jsonData["user_id"].get<std::string>());
        if(jsonData.contains("member")) voiceStateData.member = getGuildMemberData(jsonData["member"], guildId);
        if(jsonData.contains("session_id")) voiceStateData.session_id = jsonData["session_id"];
        if(jsonData.contains("deaf")) voiceStateData.deaf = jsonData["deaf"];
        if(jsonData.contains("mute")) voiceStateData.mute = jsonData["mute"];
        if(jsonData.contains("self_deaf")) voiceStateData.selfDeaf = jsonData["self_deaf"];
        if(jsonData.contains("self_mute")) voiceStateData.selfMute = jsonData["self_mute"];
        if(jsonData.contains("self_stream")) voiceStateData.selfStream = jsonData["self_stream"];
        if(jsonData.contains("self_video")) voiceStateData.selfVideo = jsonData["self_video"];
        if(jsonData.contains("suppress")) voiceStateData.suppress = jsonData["suppress"];
        if(jsonData.contains("request_to_speak_timestamp") && !jsonData["request_to_speak_timestamp"].is_null()) voiceStateData.requestToSpeakTimestamp = jsonData["request_to_speak_timestamp"];
        return voiceStateData;
    }

    guild eventData::getGuildData(const json &jsonData) {
        guild guildData;
        if(jsonData.contains("id")) guildData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("name")) guildData.name = jsonData["name"];
        if(jsonData.contains("icon") && !jsonData["icon"].is_null()) guildData.icon = jsonData["icon"];
        if(jsonData.contains("icon_hash") && !jsonData["icon_hash"].is_null()) {guildData.iconHash = jsonData["icon_hash"]; }
        if(jsonData.contains("discovery_splash") && !jsonData["discovery_splash"].is_null()) guildData.discoverySplash = jsonData["discovery_splash"];
        if(jsonData.contains("owner")) {guildData.owner = jsonData["owner"]; }
        if(jsonData.contains("owner_id")) {guildData.ownerId = std::stol(jsonData["owner_id"].get<std::string>()); }
        if(jsonData.contains("permissions")) {guildData.permissions = jsonData["permissions"]; }
        if(jsonData.contains("afk_channel_id") && !jsonData["afk_channel_id"].is_null()) {guildData.afkChannelId = std::stol(jsonData["afk_channel_id"].get<std::string>()); }
        if(jsonData.contains("afk_timeout")) {guildData.afkTimeout = jsonData["afk_timeout"]; }
        if(jsonData.contains("widget_enabled")) {guildData.widget_enabled = jsonData["widget_enabled"]; }
        if(jsonData.contains("widget_channel_id") && !jsonData["widget_channel_id"].is_null()) {guildData.widgetChannelId = std::stol(jsonData["widget_channel_id"].get<std::string>()); }
        if(jsonData.contains("verification_level")) {guildData.verification_level = jsonData["verification_level"]; }
        if(jsonData.contains("default_message_notifications")) {guildData.defaultMessageNotifications = jsonData["default_message_notifications"]; }
        if(jsonData.contains("explicit_content_filter")) {guildData.explicitContentFilter = jsonData["explicit_content_filter"]; }

        if(jsonData.contains("roles")) {
            for(const nlohmann::basic_json<>& role : jsonData["roles"]) {
                if(role["id"].empty()) continue;
                guildData.roles[std::stol(role["id"].get<std::string>())] = getRoleData(role);
            }
        }


        if(jsonData.contains("emojis")) {
            for (const nlohmann::basic_json<>& emoji: jsonData["emojis"]) {
                if(emoji["id"].empty()) continue;
                guildData.emojis[std::stol(emoji["id"].get<std::string>())] = getEmojiData(emoji);
            }
        }

        if(jsonData.contains("features")) {
            for (const nlohmann::basic_json<>& feature: jsonData["features"]) {
                guildData.features.emplace_back(feature);
            }
        }

        if(jsonData.contains("mfa_level")) {guildData.mfaLevel = jsonData["mfa_level"]; }
        if(jsonData.contains("application_id") && !jsonData["application_id"].is_null()) {guildData.applicationId = std::stol(jsonData["application_id"].get<std::string>()); }
        if(jsonData.contains("system_channel_id") && !jsonData["system_channel_id"].is_null()) {guildData.afkChannelId = std::stol(jsonData["system_channel_id"].get<std::string>()); }
        if(jsonData.contains("system_channel_flags")) {guildData.systemChannelFlags = jsonData["system_channel_flags"]; }
        if(jsonData.contains("rules_channel_id") && !jsonData["rules_channel_id"].is_null()) {guildData.rulesChannelId = std::stol(jsonData["rules_channel_id"].get<std::string>()); }
        if(jsonData.contains("max_presences") && !jsonData["max_presences"].is_null()) {guildData.maxPresences = jsonData["max_presences"]; }
        if(jsonData.contains("max_members")) {guildData.maxMembers = jsonData["max_members"]; }
        if(jsonData.contains("vanity_url_code") && !jsonData["vanity_url_code"].is_null()) {guildData.vanityUrlCode = jsonData["vanity_url_code"]; }
        if(jsonData.contains("description") && !jsonData["description"].is_null()) {guildData.description = jsonData["description"]; }
        if(jsonData.contains("banner") && !jsonData["banner"].is_null()) {guildData.banner = jsonData["banner"]; }
        if(jsonData.contains("premium_tier")) {guildData.premiumTier = jsonData["premium_tier"]; }
        if(jsonData.contains("premium_subscription_count")) {guildData.premiumSubscriptionCount = jsonData["premium_subscription_count"]; }
        if(jsonData.contains("preferred_locale")) {guildData.preferredLocale = jsonData["preferred_locale"]; }
        if(jsonData.contains("public_updates_channel_id") && !jsonData["public_updates_channel_id"].is_null()) {guildData.publicUpdatesChannelId = std::stol(jsonData["public_updates_channel_id"].get<std::string>()); }
        if(jsonData.contains("max_video_channel_users")) {guildData.maxVideoChannelUsers = jsonData["max_video_channel_users"]; }
        if(jsonData.contains("approximate_member_count")) {guildData.approximateMemberCount = jsonData["approximate_member_count"]; }
        if(jsonData.contains("approximate_presence_count")) {guildData.approximatePresenceCount = jsonData["approximate_presence_count"]; }
        if(jsonData.contains("welcome_screen")) {welcomeScreen welcome_screen = getWelcomeScreenData(jsonData["welcome_screen"]); }
        if(jsonData.contains("nsfw_level")) {guildData.nsfwLevel = jsonData["nsfw_level"]; }

        if(jsonData.contains("stickers")) {
            for (const nlohmann::basic_json<> &sticker: jsonData["stickers"]) {
                if(sticker["id"].empty()) continue;
                guildData.stickers[std::stol(sticker["id"].get<std::string>())] = getStickerData(sticker);
            }
        }


        if(jsonData.contains("premium_progress_bar_enabled")) {guildData.premiumProgressBarEnabled = jsonData["premium_progress_bar_enabled"]; }
        return guildData;
    }

    overwrite eventData::getOverwriteData(const json &jsonData) {
        overwrite overwriteData;
        if(jsonData.contains("id")) overwriteData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("type")) overwriteData.type = jsonData["type"];
        if(jsonData.contains("allow")) overwriteData.allow = jsonData["allow"].dump();
        if(jsonData.contains("deny")) overwriteData.deny = jsonData["deny"].dump();
        return overwriteData;
    }

    threadMetadata eventData::getThreadMetadataData(const json& jsonData) {
        threadMetadata threadMetadataData;
        if(jsonData.contains("archived")) threadMetadataData.archived = jsonData["archived"];
        if(jsonData.contains("auto_archive_duration")) threadMetadataData.autoArchiveDuration = jsonData["auto_archive_duration"];
        if(jsonData.contains("archive_timestamp")) threadMetadataData.archiveTimestamp = jsonData["archive_timestamp"];
        if(jsonData.contains("locked")) threadMetadataData.locked = jsonData["locked"];
        if(jsonData.contains("invitable")) threadMetadataData.invitable = jsonData["invitable"];
        if(jsonData.contains("create_timestamp") && !jsonData["create_timestamp"].is_null()) threadMetadataData.createTimestamp = jsonData["create_timestamp"];
        return threadMetadataData;
    }

    threadMember eventData::getThreadMemberData(const json& jsonData, const long& guildId) {
        threadMember threadMemberData;
        if(jsonData.contains("id")) threadMemberData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("user_id")) threadMemberData.userId = std::stol(jsonData["user_id"].get<std::string>());
        if(jsonData.contains("join_timestamp")) threadMemberData.joinTimestamp = jsonData["join_timestamp"];
        if(jsonData.contains("flags")) threadMemberData.flags = jsonData["flags"];
        if(jsonData.contains("member")) threadMemberData.member = getGuildMemberData(jsonData["member"], guildId);
        return threadMemberData;
    }

    tag eventData::getTagData(const json& jsonData) {
        tag tagData;
        if(jsonData.contains("id")) tagData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("name")) tagData.name = jsonData["name"];
        if(jsonData.contains("moderated")) tagData.moderated = jsonData["moderated"];
        if(jsonData.contains("emoji_id") && !jsonData["emoji_id"].is_null()) tagData.emojiId = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("emoji_name") && !jsonData["emoji_name"].is_null()) tagData.emojiName = jsonData["emoji_name"];
        return tagData;
    }
    defaultReaction eventData::getDefaultReactionData(const json& jsonData) {
        defaultReaction defaultReactionData;
        if(jsonData.contains("emoji_id") && !jsonData["emoji_id"].is_null()) defaultReactionData.emojiId = std::stol(jsonData["emoji_id"].get<std::string>());
        if(jsonData.contains("emoji_name") && !jsonData["emoji_name"].is_null()) defaultReactionData.emojiName = jsonData["emoji_name"];
        return defaultReactionData;
    }


    channel eventData::getChannelData(const json& jsonData, const long& guildId) {
        channel channelData;
        if(jsonData.contains("id")) channelData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("type")) channelData.type = jsonData["type"];
        if(jsonData.contains("guild_id")) channelData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
        if(jsonData.contains("position")) channelData.position = jsonData["position"];

        if(jsonData.contains("permission_overwrites")) {
            for (const nlohmann::basic_json<>& overwrite: jsonData["permission_overwrites"]) {
                if(overwrite["id"].empty()) continue;
                channelData.permissionOverwrites[std::stol(overwrite["id"].get<std::string>())] = getOverwriteData(overwrite);
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
        if(jsonData.contains("thread_metadata")) channelData.thread_metadata = getThreadMetadataData(jsonData["thread_metadata"]);
        if(jsonData.contains("member")) channelData.member = getThreadMemberData(jsonData["member"], guildId);
        if(jsonData.contains("default_auto_archive_duration")) channelData.defaultAutoArchiveDuration = jsonData["default_auto_archive_duration"];
        if(jsonData.contains("permissions")) channelData.permissions = jsonData["permissions"];
        if(jsonData.contains("flags")) channelData.flags = jsonData["flags"];
        if(jsonData.contains("total_message_sent")) channelData.totalMessageSent = jsonData["total_message_sent"];
        if(jsonData.contains("available_tags")) {
            for (const nlohmann::basic_json<>& tag: jsonData["available_tags"]) {
                if(tag["id"].empty()) continue;
                channelData.availableTags[std::stol(tag["id"].get<std::string>())] = getTagData(tag);
            }
        }

        if(jsonData.contains("applied_tags")) {
            for (const nlohmann::basic_json<>& tag: jsonData["applied_tags"]) {
                channelData.appliedTags.emplace_back(tag);
            }
        }

        if(jsonData.contains("default_reaction_emoji") && !jsonData["default_reaction_emoji"].is_null()) channelData.defaultReactionEmoji = getDefaultReactionData(jsonData["default_reaction_emoji"]);
        if(jsonData.contains("default_thread_rate_limit_per_use")) channelData.defaultThreadRateLimitPerUse = jsonData["default_thread_rate_limit_per_use"];
        if(jsonData.contains("default_sort_order") && !jsonData["default_sort_order"].is_null()) channelData.defaultSortOrder = jsonData["default_sort_order"];
        if(jsonData.contains("default_forum_layout")) channelData.defaultForumLayout = jsonData["default_forum_layout"];

        return channelData;
    }


    partialApplication eventData::getPartialApplicationData(const json &jsonData) {
        partialApplication partialApplicationData;
        if(jsonData.contains("id")) {partialApplicationData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("flags")) {partialApplicationData.flags = jsonData["flags"]; }
        return partialApplicationData;
    }

    unavailableGuild eventData::getUnavailableGuildData(const json &jsonData) {
        unavailableGuild unavailableGuildsData;
        if(jsonData.contains("id")) {unavailableGuildsData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("unavailable")) {unavailableGuildsData.unavailable = jsonData["unavailable"]; }
        return unavailableGuildsData;
    }

    readyEvent eventData::getReadyEventData(const json &jsonData) {
        this->readyEventData.user = user::getUserData(jsonData["user"]);

        for(const nlohmann::basic_json<>& guild : jsonData["guilds"]) {
            this->readyEventData.guilds.emplace_back(this->getUnavailableGuildData(guild));
        }

        if(jsonData.contains("shard")) {
            this->readyEventData.shard[0] = jsonData["shard"].front();
            this->readyEventData.shard[1] = jsonData["shard"].back();
        }

        this->readyEventData.application = getPartialApplicationData(jsonData["application"]);
        return this->readyEventData;
    }

    resumedEvent eventData::getResumedEventData(const json &jsonData) const {
        return this->resumedEventData;
    }

    channelCreateEvent eventData::getChannelCreateEventData(const json &jsonData) {
        this->channelCreateData.channel = getChannelData(jsonData, std::stol(jsonData["guild_id"].get<std::string>()));
        return this->channelCreateData;
    }

    channelUpdateEvent eventData::getChannelUpdateEventData(const json &jsonData) {
        this->channelUpdateData.channel = getChannelData(jsonData, std::stol(jsonData["guild_id"].get<std::string>()));
        return this->channelUpdateData;
    }

    channelDeleteEvent eventData::getChannelDeleteEventData(const json &jsonData) {
        this->channelDeleteData.channel = getChannelData(jsonData, std::stol(jsonData["guild_id"].get<std::string>()));
        return this->channelDeleteData;
    }


    guildCreateEvent eventData::getGuildCreateEventData(const json& jsonData) {
        this->guildCreateData.guild = getGuildData(jsonData);

        if(jsonData.contains("joined_at")) {this->guildCreateData.joinedAt = jsonData["joined_at"]; }
        if(jsonData.contains("large")) {this->guildCreateData.large = jsonData["large"]; }
        if(jsonData.contains("unavailable")) {this->guildCreateData.unavailable = jsonData["unavailable"]; }
        if(jsonData.contains("member_count")) {this->guildCreateData.memberCount = jsonData["member_count"]; }

        if(jsonData.contains("voice_states")) {
            for (const nlohmann::basic_json<> &voiceState: jsonData["voice_states"]) {
                if(voiceState["user_id"].empty()) continue;
                this->guildCreateData.voiceStates[std::stol(voiceState["user_id"].get<std::string>())] = getVoiceStateData(voiceState, this->guildCreateData.guild.id.value());
            }
        }

        if(jsonData.contains("members")) {
            for (const nlohmann::basic_json<> &member: jsonData["members"]) {
                if(member["user"]["id"].empty()) continue;
                this->guildCreateData.members[std::stol(member["user"]["id"].get<std::string>())] = getGuildMemberData(member, this->guildCreateData.guild.id.value());
            }
        }

        if(jsonData.contains("channels")) {
            for (const nlohmann::basic_json<> &channel: jsonData["channels"]) {
                if(channel["id"].empty()) continue;
                this->guildCreateData.channels[std::stol(channel["id"].get<std::string>())] = getChannelData(channel, this->guildCreateData.guild.id.value());
            }
        }

        if(jsonData.contains("threads")) {
            for (const nlohmann::basic_json<> &thread: jsonData["threads"]) {
                if(thread["id"].empty()) continue;
                this->guildCreateData.threads[std::stol(thread["id"].get<std::string>())] = getChannelData(thread, this->guildCreateData.guild.id.value());
            }
        }
        return this->guildCreateData;
    }

    guildUpdateEvent eventData::getGuildUpdateEventData(const json &jsonData) {
        this->guildUpdateData.guild = getGuildData(jsonData);
        return this->guildUpdateData;
    }

    guildDeleteEvent eventData::getGuildDeleteEventData(const json &jsonData) {
        this->guildDeleteData.unavailableGuild = getUnavailableGuildData(jsonData);
        return this->guildDeleteData;
    }


    [[maybe_unused]] void onEvent::ready(const std::function<void(readyEvent)>& userFunction) {
        this->readyFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::resumed(const std::function<void(resumedEvent)>& userFunction) {
        this->resumedFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildCreate(const std::function<void(guildCreateEvent)> &userFunction) {
        this->guildCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildUpdate(const std::function<void(guildUpdateEvent)> &userFunction) {
        this->guildUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildDelete(const std::function<void(guildDeleteEvent)> &userFunction) {
        this->guildDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelCreate(const std::function<void(channelCreateEvent)> &userFunction) {
        this->channelCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelUpdate(const std::function<void(channelUpdateEvent)> &userFunction) {
        this->channelUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelDelete(const std::function<void(channelDeleteEvent)> &userFunction) {
        this->channelDeleteFunction = userFunction;
    }

    void eventData::cacheGuild(const guildCacheObject& guildToBeCached) {
        //std::ofstream ofs(std::to_string(guildToBeCached.id.value()) + ".dat");
        //boost::archive::text_oarchive oa(ofs);
        //oa << guildToBeCached;
    }

    guildMember::operator bool() const {
        return this->guildId.has_value();
    }

    guild guildOptions::createGuild(const helios::guild &guildOptions, const std::vector<role> &roles, const std::vector<channel> &channels) {
        json createGuildPayload;
        if(guildOptions.name.has_value()) createGuildPayload["name"] = guildOptions.name.value();
        if(guildOptions.icon.has_value()) createGuildPayload["icon"] = guildOptions.icon.value();
        if(guildOptions.verification_level.has_value()) createGuildPayload["verification_level"] = guildOptions.verification_level.value();
        if(guildOptions.defaultMessageNotifications.has_value()) createGuildPayload["default_message_notifications"] = guildOptions.defaultMessageNotifications.value();
        if(guildOptions.explicitContentFilter.has_value()) createGuildPayload["explicit_content_filter"] = guildOptions.explicitContentFilter.value();

        json createGuildRoles = json::array();
        for(auto& role : roles) {
            json roleJson;
            if(role.name.has_value()) roleJson["name"] = role.name.value();
            if(role.permissions.has_value()) roleJson["permissions"] = role.permissions.value();
            if(role.color.has_value()) roleJson["color"] = role.color.value();
            if(role.hoist.has_value()) roleJson["hoist"] = role.hoist.value();
            if(role.icon.has_value()) roleJson["icon"] = role.icon.value();
            if(role.unicodeEmoji.has_value()) roleJson["unicode_emoji"] = role.unicodeEmoji.value();
            if(role.mentionable.has_value()) roleJson["mentionable"] = role.mentionable.value();
            createGuildRoles.emplace_back(roleJson);
        }
        if(!createGuildRoles.empty()) createGuildPayload["roles"] = createGuildRoles;

        json createGuildChannel = json::array();
        for(auto& channel : channels) {
            json channelJson;
            if(channel.name.has_value()) channelJson["name"] = channel.name.value();
            if(channel.type.has_value()) channelJson["type"] = channel.type.value();
            if(channel.topic.has_value()) channelJson["topic"] = channel.topic.value();
            if(channel.bitrate.has_value()) channelJson["bitrate"] = channel.bitrate.value();
            if(channel.userLimit.has_value()) channelJson["user_limit"] = channel.userLimit.value();
            if(channel.rateLimitPerUser.has_value()) channelJson["rate_limit_per_user"] = channel.rateLimitPerUser.value();
            if(channel.position.has_value()) channelJson["position"] = channel.position.value();
            if(!channel.permissionOverwrites.empty()) {

            }
            if(channel.parentId.has_value()) channelJson["parent_id"] = channel.parentId.value();
            if(channel.nsfw.has_value()) channelJson["nsfw"] = channel.nsfw.value();
            if(channel.rtcRegion.has_value()) channelJson["rtc_region"] = channel.rtcRegion.value();
            if(channel.videoQualityMode.has_value()) channelJson["video_quality_mode"] = channel.videoQualityMode.value();
            if(channel.defaultAutoArchiveDuration.has_value()) channelJson["default_auto_archive_duration"] = channel.defaultAutoArchiveDuration.value();
            if(channel.defaultReactionEmoji.emojiId.has_value()) {
                channelJson["default_reaction_emoji"]["emoji_id"] = channel.defaultReactionEmoji.emojiId.value();
            };
            if(channel.defaultReactionEmoji.emojiName.has_value()) {
                channelJson["default_reaction_emoji"]["emoji_name"] = channel.defaultReactionEmoji.emojiName.value();
            };

            if(!channel.availableTags.empty()) {

            }

            if(channel.defaultSortOrder.has_value()) channelJson["default_sort_order"] = channel.defaultSortOrder.value();
            createGuildChannel.emplace_back(channelJson);
        }

        if(guildOptions.afkChannelId.has_value()) createGuildPayload["afk_channel_id"] = guildOptions.afkChannelId.value();
        if(guildOptions.afkTimeout.has_value()) createGuildPayload["afk_timeout"] = guildOptions.afkTimeout.value();
        if(guildOptions.systemChannelId.has_value()) createGuildPayload["system_channel_id"] = guildOptions.systemChannelId.value();
        if(guildOptions.systemChannelFlags.has_value()) createGuildPayload["system_channel_flags"] = guildOptions.systemChannelFlags.value();

        const std::string newGuild = request::httpsRequest("discord.com", "/api/guilds", createGuildPayload.dump(), "post",this->token);
        return eventData::getGuildData(json::parse(newGuild));
    }

    guild guildOptions::getGuild(const long& guildId, const bool& withCounts, const bool& cacheObject) const {
        json payload;
        payload["with_counts"] = withCounts;
        const std::string newGuild = request::httpsRequest("discord.com", "/api/guilds/" + std::to_string(guildId), payload.dump(), "get", this->token);
        if(cacheObject) {
            //TODO cache this shit
        }
        return eventData::getGuildData(json::parse(newGuild));
    }
    guild guildOptions::getGuild(const std::string& guildId, const bool& withCounts, const bool& cacheObject) const {
        return guildOptions::getGuild(std::stol(guildId), withCounts, cacheObject);
    }

}