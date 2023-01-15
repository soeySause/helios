#include "event.hpp"

namespace helios {
    user eventData::getUserData(const json &jsonData) {
        user userData;
        if(jsonData.contains("id")) {userData.id = std::stol(jsonData["id"].get<std::string>()); }
        if(jsonData.contains("username")) {userData.username = jsonData["username"]; }
        if(jsonData.contains("discriminator")) {userData.discriminator = jsonData["discriminator"]; }
        if(jsonData.contains("avatar") && !jsonData["avatar"].is_null()) {userData.avatar = jsonData["avatar"]; }
        if(jsonData.contains("bot")) {userData.bot = jsonData["bot"]; }
        if(jsonData.contains("system")) {userData.system = jsonData["system"]; }
        if(jsonData.contains("mfa_enabled")) {userData.mfa_enabled = jsonData["mfa_enabled"]; }
        if(jsonData.contains("banner") && !jsonData["banner"].is_null()) {userData.banner = jsonData["banner"]; }
        if(jsonData.contains("accent_color") && !jsonData["accent_color"].is_null()) {userData.accent_color = jsonData["accent_color"]; }
        if(jsonData.contains("locale")) {userData.locale = jsonData["locale"]; }
        if(jsonData.contains("verified")) {userData.verified = jsonData["verified"]; }
        if(jsonData.contains("email") && !jsonData["email"].is_null()) {userData.email = jsonData["email"]; }
        if(jsonData.contains("flags")) {userData.flags = jsonData["flags"]; }
        if(jsonData.contains("premium_type")) {userData.premium_type = jsonData["premium_type"]; }
        if(jsonData.contains("avatar")) {userData.avatar = jsonData["avatar"]; }
        if(jsonData.contains("public_flags")) {userData.public_flags = jsonData["public_flags"]; }
        return userData;
    }

    roleTags eventData::getRoleTagsData(const json &jsonData) {
        roleTags roleTagsData;
        if(jsonData.contains("bot_id")) {roleTagsData.bot_id = jsonData["bot_id"]; }
        if(jsonData.contains("integration_id")) {roleTagsData.integration_id = jsonData["integration_id"]; }
        if(jsonData.contains("premium_subscriber")) {roleTagsData.premium_subscriber = jsonData["premium_subscriber"]; }
        if(jsonData.contains("subscription_listing_id")) {roleTagsData.subscription_listing_id = jsonData["subscription_listing_id"]; }
        if(jsonData.contains("available_for_purchase")) {roleTagsData.available_for_purchase = jsonData["available_for_purchase"]; }
        if(jsonData.contains("guild_connections")) {roleTagsData.guild_connections = jsonData["guild_connections"]; }
        return roleTagsData;
    }

    role eventData::getRoleData(const json &jsonData) {
        role roleData;
        if(jsonData.contains("id")) {roleData.id = jsonData["id"]; }
        if(jsonData.contains("name")) {roleData.name = jsonData["name"]; }
        if(jsonData.contains("color")) {roleData.color = jsonData["color"]; }
        if(jsonData.contains("hoist")) {roleData.hoist = jsonData["hoist"]; }
        if(jsonData.contains("icon")) {roleData.icon = jsonData["icon"]; }
        if(jsonData.contains("unicode_emoji")) {roleData.unicode_emoji = jsonData["unicode_emoji"]; }
        if(jsonData.contains("position")) {roleData.position = jsonData["position"]; }
        if(jsonData.contains("permissions")) {roleData.permissions = jsonData["permissions"]; }
        if(jsonData.contains("managed")) {roleData.managed = jsonData["managed"]; }
        if(jsonData.contains("mentionable")) {roleData.mentionable = jsonData["mentionable"]; }
        if(jsonData.contains("tags")) {roleData.tags = getRoleTagsData(jsonData["tags"]); }
    }

    emoji eventData::getEmojiData(const json& jsonData) {
        emoji emojiData;
        emojiData.id = jsonData["id"];
        emojiData.name = jsonData["name"];

        if(jsonData.contains("roles")) {
            for(const nlohmann::basic_json<>& role : jsonData["roles"]) {
                if(role["id"].empty()) continue;
                emojiData.roles[role["id"]] = getRoleData(role);
            }
        }

        if(jsonData.contains("user")) { emojiData.user = getUserData(jsonData["user"]); }
        if(jsonData.contains("require_colons")) {emojiData.require_colons = jsonData["require_colons"]; }
        if(jsonData.contains("managed")) {emojiData.managed = jsonData["managed"]; }
        if(jsonData.contains("animated")) {emojiData.animated = jsonData["animated"]; }
        if(jsonData.contains("available")) {emojiData.available = jsonData["available"]; }
        return emojiData;
    };

    welcomeScreenChannelStructure eventData::getWelcomeScreenChannelStructureData(const json& jsonData) {
        welcomeScreenChannelStructure welcomeScreenChannelStructureData;
        if(jsonData.contains("channel_id")) welcomeScreenChannelStructureData.channel_id = jsonData["channel_id"];
        if(jsonData.contains("description")) welcomeScreenChannelStructureData.description = jsonData["description"];
        if(jsonData.contains("emoji_id")) welcomeScreenChannelStructureData.emoji_id = jsonData["emoji_id"];
        if(jsonData.contains("emoji_name")) {welcomeScreenChannelStructureData.emoji_name = jsonData["emoji_name"]; }

    }

    welcomeScreen eventData::getWelcomeScreenData(const json& jsonData) {
        welcomeScreen welcomeScreenData;
        if(jsonData.contains("description")) {welcomeScreenData.description = jsonData["description"]; }

        if(jsonData.contains("welcome_channels")) {
            for(const nlohmann::basic_json<>& channels : jsonData["welcome_channels"]) {
                welcomeScreenData.welcome_channels.emplace_back(getWelcomeScreenChannelStructureData(channels));
            }
        }

        return welcomeScreenData;
    }

    sticker eventData::getStickerData(const json &jsonData) {
        sticker stickerData;
        if(jsonData.contains("id")) { stickerData.id = jsonData["id"]; }
        if(jsonData.contains("pack_id")) { stickerData.pack_id = jsonData["pack_id"]; }
        if(jsonData.contains("name")) { stickerData.name = jsonData["name"]; }
        if(jsonData.contains("description")) { stickerData.description = jsonData["description"]; }
        if(jsonData.contains("tags")) { stickerData.tags = jsonData["tags"]; }
        if(jsonData.contains("asset")) { stickerData.asset = jsonData["asset"]; }
        if(jsonData.contains("type")) { stickerData.type = jsonData["type"]; }
        if(jsonData.contains("format_type")) { stickerData.format_type = jsonData["format_type"]; }
        if(jsonData.contains("available")) { stickerData.available = jsonData["available"]; }
        if(jsonData.contains("guild_id")) { stickerData.guild_id = jsonData["guild_id"]; }
        if(jsonData.contains("user")) { stickerData.user = getUserData("user"); }
        if(jsonData.contains("sort_value")) { stickerData.sort_value = jsonData["sort_value"]; }
        return stickerData;
    }

    guild eventData::getGuildData(const json &jsonData) {
        guild guildData;
        if(jsonData.contains("id")) guildData.id = jsonData["id"];
        if(jsonData.contains("name")) guildData.name = jsonData["name"];
        if(jsonData.contains("icon")) guildData.icon = jsonData["icon"];
        if(jsonData.contains("icon_hash")) {guildData.icon_hash = jsonData["icon_hash"]; }
        if(jsonData.contains("discovery_splash")) guildData.discovery_splash = jsonData["discovery_splash"];
        if(jsonData.contains("discovery_splash")) {guildData.discovery_splash = jsonData["discovery_splash"]; }
        if(jsonData.contains("owner")) {guildData.owner = jsonData["owner"]; }
        if(jsonData.contains("owner_id")) {guildData.owner_id = jsonData["owner_id"]; }
        if(jsonData.contains("permissions")) {guildData.permissions = jsonData["permissions"]; }
        if(jsonData.contains("afk_channel_id")) {guildData.afk_channel_id = jsonData["afk_channel_id"]; }
        if(jsonData.contains("afk_timeout")) {guildData.afk_timeout = jsonData["afk_timeout"]; }
        if(jsonData.contains("widget_enabled")) {guildData.widget_enabled = jsonData["widget_enabled"]; }
        if(jsonData.contains("widget_channel_id")) {guildData.widget_channel_id = jsonData["widget_channel_id"]; }
        if(jsonData.contains("verification_level")) {guildData.verification_level = jsonData["verification_level"]; }
        if(jsonData.contains("default_message_notifications")) {guildData.default_message_notifications = jsonData["default_message_notifications"]; }
        if(jsonData.contains("explicit_content_filter")) {guildData.explicit_content_filter = jsonData["explicit_content_filter"]; }

        if(jsonData.contains("roles")) {
            for(const nlohmann::basic_json<>& role : jsonData["roles"]) {
                if(role["id"].empty()) continue;
                guildData.roles[role["id"]] = getRoleData(role);
            }
        }


        if(jsonData.contains("emojis")) {
            for (const nlohmann::basic_json<> &emoji: jsonData["emojis"]) {
                if(emoji["id"].empty()) continue;
                guildData.roles[emoji["id"]] = getRoleData(emoji);
            }
        }

        if(jsonData.contains("features")) std::vector<std::string> features = jsonData["features"].array();
        if(jsonData.contains("mfa_level")) {guildData.mfa_level = jsonData["mfa_level"]; }
        if(jsonData.contains("application_id")) {guildData.application_id = jsonData["application_id"]; }
        if(jsonData.contains("system_channel_id")) {guildData.system_channel_id = jsonData["system_channel_id"]; }
        if(jsonData.contains("system_channel_flags")) {guildData.system_channel_flags = jsonData["system_channel_flags"]; }
        if(jsonData.contains("rules_channel_id")) {guildData.rules_channel_id = jsonData["rules_channel_id"]; }
        if(jsonData.contains("max_presences")) {guildData.max_presences = jsonData["max_presences"]; }
        if(jsonData.contains("max_members")) {guildData.max_members = jsonData["max_members"]; }
        if(jsonData.contains("vanity_url_code")) {guildData.vanity_url_code = jsonData["vanity_url_code"]; }
        if(jsonData.contains("description")) {guildData.description = jsonData["description"]; }
        if(jsonData.contains("banner")) {guildData.banner = jsonData["banner"]; }
        if(jsonData.contains("premium_tier")) {guildData.premium_tier = jsonData["premium_tier"]; }
        if(jsonData.contains("premium_subscription_count")) {guildData.premium_subscription_count = jsonData["premium_subscription_count"]; }
        if(jsonData.contains("preferred_locale")) {guildData.preferred_locale = jsonData["preferred_locale"]; }
        if(jsonData.contains("public_updates_channel_id")) {guildData.public_updates_channel_id = jsonData["public_updates_channel_id"]; }
        if(jsonData.contains("max_video_channel_users")) {guildData.max_video_channel_users = jsonData["max_video_channel_users"]; }
        if(jsonData.contains("approximate_member_count")) {guildData.approximate_member_count = jsonData["approximate_member_count"]; }
        if(jsonData.contains("approximate_presence_count")) {guildData.approximate_presence_count = jsonData["approximate_presence_count"]; }
        if(jsonData.contains("welcome_screen")) {welcomeScreen welcome_screen = getWelcomeScreenData(jsonData["welcome_screen"]); }
        if(jsonData.contains("nsfw_level")) {guildData.nsfw_level = jsonData["nsfw_level"]; }

        if(jsonData.contains("stickers")) {
            for (const nlohmann::basic_json<> &sticker: jsonData["stickers"]) {
                if(sticker["id"].empty()) continue;
                guildData.roles[sticker["id"]] = getRoleData(sticker);
            }
        }


        if(jsonData.contains("premium_progress_bar_enabled")) {guildData.premium_progress_bar_enabled = jsonData["premium_progress_bar_enabled"]; }
        return guildData;
    }

    partialApplication eventData::getPartialApplicationData(const json &jsonData) {
        partialApplication partialApplicationData;
        if(jsonData.contains("id")) {partialApplicationData.id = jsonData["id"]; }
        if(jsonData.contains("flags")) {partialApplicationData.flags = jsonData["flags"]; }
        return partialApplicationData;
    }

    unavailableGuilds eventData::getUnavailableGuildsData(const json &jsonData) {
        unavailableGuilds unavailableGuildsData;
        if(jsonData.contains("id")) {unavailableGuildsData.id = jsonData["id"]; }
        if(jsonData.contains("unavailable")) {unavailableGuildsData.unavailable = true; }
        return unavailableGuildsData;
    }

    readyEvent eventData::getReadyEventData(const json &jsonData) {
        this->readyEventData.user = getUserData(jsonData["user"]);

        for(const nlohmann::basic_json<>& guild : jsonData["guilds"]) {
            this->readyEventData.guilds.emplace_back(this->getUnavailableGuildsData(guild));
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

    guildCreatEvent eventData::getGuildCreateEventData(const json& jsonData) {
        this->guildCreateData.guild = getGuildData(jsonData);
        if(jsonData.contains("unavailable")) {this->guildCreateData.unavailable = jsonData["unavailable"]; }
        return this->guildCreateData;
    }

    [[maybe_unused]] void onEvent::ready(const std::function<void(readyEvent)>& userFunction) {
        this->readyFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::resumed(const std::function<void(resumedEvent)>& userFunction) {
        this->resumedFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildCreate(const std::function<void(guildCreatEvent)> &userFunction) {
        this->guildCreateFunction = userFunction;
    }

    guildMember::operator bool() const {
        this->joined_at.has_value();
    }

    bool user::operator==(const user &userToCompare) const {
        return this->id == userToCompare.id;
    }

    user::operator bool() const {
        this->id.has_value();
    }
}