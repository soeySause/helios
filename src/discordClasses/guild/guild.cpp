#include "discordClassses/guild/guild.hpp"
#include <optional>
namespace helios {

    welcomeScreenChannelStructure
    welcomeScreenChannelStructure::getWelcomeScreenChannelStructureData(const json &jsonData) {
        welcomeScreenChannelStructure welcomeScreenChannelStructureData;
        welcomeScreenChannelStructureData.channelId = std::stol(jsonData["channel_id"].get<std::string>());
        welcomeScreenChannelStructureData.description = jsonData["description"];
        if(!jsonData["emoji_id"].is_null()) welcomeScreenChannelStructureData.emojiId = std::stol(jsonData["integration_id"].get<std::string>());
        if(!jsonData["emoji_name"].is_null()) welcomeScreenChannelStructureData.emojiName = jsonData["emoji_name"];
        return welcomeScreenChannelStructureData;
    }

    welcomeScreen welcomeScreen::getWelcomeScreenData(const json &jsonData) {
        welcomeScreen welcomeScreenData;
        if(!jsonData["description"].is_null()) {welcomeScreenData.description = jsonData["description"]; }

        for(const nlohmann::basic_json<>& channels : jsonData["welcome_channels"]) {
            welcomeScreenData.welcome_channels.emplace_back(welcomeScreenChannelStructure::getWelcomeScreenChannelStructureData(channels));
        }
        return welcomeScreenData;
    }

    unavailableGuild unavailableGuild::getUnavailableGuildData(const json &jsonData) {
        unavailableGuild unavailableGuildsData;
        if(jsonData.contains("id")) {unavailableGuildsData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("unavailable")) {unavailableGuildsData.unavailable = jsonData["unavailable"]; }
        return unavailableGuildsData;
    }


    guild guild::getGuildData(const json& jsonData) {
        guild guildData;
        guildData.id = std::stol(jsonData["id"].get<std::string>());
        guildData.name = jsonData["name"];
        if(!jsonData["icon"].is_null()) guildData.icon = jsonData["icon"];
        if(jsonData.contains("icon_hash") && !jsonData["icon_hash"].is_null()) {guildData.iconHash = jsonData["icon_hash"]; }
        if(!jsonData["splash"].is_null()) guildData.splash = jsonData["splash"];
        if(!jsonData["discovery_splash"].is_null()) guildData.discoverySplash = jsonData["discovery_splash"];
        if(jsonData.contains("owner")) {guildData.owner = jsonData["owner"]; }
        guildData.ownerId = std::stol(jsonData["owner_id"].get<std::string>());
        if(jsonData.contains("permissions")) {guildData.permissions = jsonData["permissions"]; }
        if(!jsonData["afk_channel_id"].is_null()) guildData.afkChannelId = std::stol(jsonData["afk_channel_id"].get<std::string>());
        guildData.afkTimeout = jsonData["afk_timeout"];
        if(jsonData.contains("widget_enabled")) {guildData.widget_enabled = jsonData["widget_enabled"]; }
        if(jsonData.contains("widget_channel_id") && !jsonData["widget_channel_id"].is_null()) {guildData.widgetChannelId = std::stol(jsonData["widget_channel_id"].get<std::string>()); }
        guildData.verification_level = jsonData["verification_level"];
        guildData.defaultMessageNotifications = jsonData["default_message_notifications"];
        guildData.explicitContentFilter = jsonData["explicit_content_filter"];

        for(const nlohmann::basic_json<>& role : jsonData["roles"]) {
            if(role["id"].empty()) continue;
            guildData.roles[std::stol(role["id"].get<std::string>())] = role::getRoleData(role);
        }

        for (const nlohmann::basic_json<>& emoji: jsonData["emojis"]) {
            if(emoji["id"].empty()) continue;
            guildData.emojis[std::stol(emoji["id"].get<std::string>())] = emoji::getEmojiData(emoji);
        }

        for (const nlohmann::basic_json<>& feature: jsonData["features"]) {
            guildData.features.emplace_back(feature);
        }

        guildData.mfaLevel = jsonData["mfa_level"];
        if(!jsonData["application_id"].is_null()) guildData.applicationId = std::stol(jsonData["application_id"].get<std::string>());
        if(!jsonData["system_channel_id"].is_null()) guildData.afkChannelId = std::stol(jsonData["system_channel_id"].get<std::string>());
        guildData.systemChannelFlags = jsonData["system_channel_flags"];
        if(!jsonData["rules_channel_id"].is_null()) guildData.rulesChannelId = std::stol(jsonData["rules_channel_id"].get<std::string>());
        if(jsonData.contains("max_presences") && !jsonData["max_presences"].is_null()) {guildData.maxPresences = jsonData["max_presences"]; }
        if(jsonData.contains("max_members")) guildData.maxMembers = jsonData["max_members"];
        if(!jsonData["vanity_url_code"].is_null()) guildData.vanityUrlCode = jsonData["vanity_url_code"];
        if(!jsonData["description"].is_null()) guildData.description = jsonData["description"];
        if(!jsonData["banner"].is_null()) guildData.banner = jsonData["banner"];
        guildData.premiumTier = jsonData["premium_tier"];
        if(jsonData.contains("premium_subscription_count")) {guildData.premiumSubscriptionCount = jsonData["premium_subscription_count"]; }
        guildData.preferredLocale = jsonData["preferred_locale"];
        if(!jsonData["public_updates_channel_id"].is_null()) guildData.publicUpdatesChannelId = std::stol(jsonData["public_updates_channel_id"].get<std::string>());
        if(jsonData.contains("max_video_channel_users")) {guildData.maxVideoChannelUsers = jsonData["max_video_channel_users"]; }
        if(jsonData.contains("approximate_member_count")) {guildData.approximateMemberCount = jsonData["approximate_member_count"]; }
        if(jsonData.contains("approximate_presence_count")) {guildData.approximatePresenceCount = jsonData["approximate_presence_count"]; }
        if(jsonData.contains("welcome_screen")) {welcomeScreen welcome_screen = welcomeScreen::getWelcomeScreenData(jsonData["welcome_screen"]); }
        guildData.nsfwLevel = jsonData["nsfw_level"];

        if(jsonData.contains("stickers")) {
            for (const nlohmann::basic_json<> &sticker: jsonData["stickers"]) {
                if(sticker["id"].empty()) continue;
                guildData.stickers[std::stol(sticker["id"].get<std::string>())] = sticker::getStickerData(sticker);
            }
        }

        guildData.premiumProgressBarEnabled = jsonData["premium_progress_bar_enabled"];
        return guildData;
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
        return guild::getGuildData(json::parse(newGuild));
    }

    guild guildOptions::getGuild(const long& guildId, const bool& withCounts, const bool& cacheObject) const {
        json payload;
        payload["with_counts"] = withCounts;
        const std::string newGuild = request::httpsRequest("discord.com", "/api/guilds/" + std::to_string(guildId), payload.dump(), "get", this->token);
        if(cacheObject) {

        }
        return guild::getGuildData(json::parse(newGuild));
    }
}