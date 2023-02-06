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
}