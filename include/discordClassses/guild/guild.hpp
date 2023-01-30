#ifndef HELIOS_GUILD_HPP
#define HELIOS_GUILD_HPP
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/serialization/access.hpp>
#include <nlohmann/json.hpp>

#include "role.hpp"
#include "../emoji.hpp"
#include "../voice.hpp"
#include "../sticker.hpp"
#include "../channel/channel.hpp"

using nlohmann::json;
namespace helios{
    class invite;
    class inviteMetadata;
    class guildPreview {
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<std::string> icon;
        std::optional<std::string> splash;
        std::optional<std::string> discoverySplash;
        std::unordered_map<long, emoji> emojis;
        std::vector<std::string> features;
        std::optional<int> approximateMemberCount;
        std::optional<int> approximatePresenceCount;
        std::optional<std::string> description;
        std::unordered_map<long, sticker> stickers;
    };

    class welcomeScreenChannelStructure {
    private:
        friend class welcomeScreen;
        friend class boost::serialization::access;
        template <typename Archive>
        [[maybe_unused]] void serialize(Archive &ar, const unsigned int version) {
            ar & channelId;
            ar & description;
            ar & emojiId;
            ar & emojiName;
        }
        static welcomeScreenChannelStructure getWelcomeScreenChannelStructureData(const json& jsonData);
    public:
        std::optional <long> channelId;
        std::optional <std::string> description;
        std::optional <long> emojiId;
        std::optional <std::string> emojiName;
    };

    class welcomeScreen {
    private:
        friend class guild;
        friend class boost::serialization::access;
        template <typename Archive>
        [[maybe_unused]] void serialize(Archive &ar, const unsigned int version) {
            ar & description;
            ar & welcome_channels;
        }
        static welcomeScreen getWelcomeScreenData(const json& jsonData);
    public:
        std::optional <std::string> description;
        std::vector<welcomeScreenChannelStructure> welcome_channels;
    };

    class integrationAccount {
        std::optional<std::string> id;
        std::optional<std::string> name;
    };

    class integration {
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<std::string> type;
        std::optional<bool> enabled;
        std::optional<bool> syncing;
        std::optional<long> role_id;
        std::optional<bool> enable_emoticons;
        std::optional<int> expire_behavior;
        std::optional<int> expire_grace_period;
        user user;
        integrationAccount account;
        std::optional<std::string> synced_at;
        std::optional<int> subscriber_count;
        std::optional<bool> revoked;
        std::optional<bool> application;
        std::vector<std::string> scopes;
    };

    class guildWidgetSettings {
        std::optional<long> channelId;
        std::optional<bool> enabled;
    };

    class guildWidget {
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<std::string> instantInvite;
        std::unordered_map<long, channel> channels;
        std::unordered_map<long, user> members;
        std::optional<int> presenceCount;
    };

    class ban {
        user user;
        std::string reason;
    };

    class unavailableGuild {
    private:
        friend class eventData;
        static unavailableGuild getUnavailableGuildData(const json& jsonData);
    public:
        std::optional<long> id;
        std::optional<bool> unavailable;
    };

    class guild {
    private:
        friend class guildOptions;
        friend class eventData;
        friend class boost::serialization::access;
        template <typename Archive>
        [[maybe_unused]] void serialize(Archive &ar, const unsigned int version) {
            ar & id;
            ar & name;
            ar & icon;
            ar & iconHash;
            ar & splash;
            ar & discoverySplash;
            ar & owner;
            ar & ownerId;
            ar & permissions;
            ar & afkChannelId;
            ar & afkTimeout;
            ar & widget_enabled;
            ar & widgetChannelId;
            ar & verification_level;
            ar & defaultMessageNotifications;
            ar & explicitContentFilter;
            ar & roles;
            ar & emojis;
            ar & features;
            ar & mfaLevel;
            ar & applicationId;
            ar & systemChannelId;
            ar & systemChannelFlags;
            ar & rulesChannelId;
            ar & maxPresences;
            ar & maxMembers;
            ar & vanityUrlCode;
            ar & description;
            ar & banner;
            ar & premiumTier;
            ar & premiumSubscriptionCount;
            ar & preferredLocale;
            ar & publicUpdatesChannelId;
            ar & maxVideoChannelUsers;
            ar & approximateMemberCount;
            ar & welcome_screen;
            ar & nsfwLevel;
            ar & stickers;
            ar & premiumProgressBarEnabled;
        }
        static guild getGuildData(const json& jsonData);
        const static guildMember defaultGuildMember;
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<std::string> icon;
        std::optional<std::string> iconHash;
        std::optional<std::string> splash;
        std::optional<std::string> discoverySplash;
        std::optional<bool> owner;
        std::optional<long> ownerId;
        std::optional<std::string> permissions;
        std::optional<long> afkChannelId;
        std::optional<int> afkTimeout;
        std::optional<bool> widget_enabled;
        std::optional<long> widgetChannelId;
        std::optional<int> verification_level;
        std::optional<int> defaultMessageNotifications;
        std::optional<int> explicitContentFilter;
        std::unordered_map<long, role> roles;
        std::unordered_map<long, emoji> emojis;
        std::vector<std::string> features;
        std::optional<int> mfaLevel;
        std::optional<long> applicationId;
        std::optional<long> systemChannelId;
        std::optional<int> systemChannelFlags;
        std::optional<long> rulesChannelId;
        std::optional<int> maxPresences;
        std::optional<int> maxMembers;
        std::optional<std::string> vanityUrlCode;
        std::optional<std::string> description;
        std::optional<std::string> banner;
        std::optional<int> premiumTier;
        std::optional<int> premiumSubscriptionCount;
        std::optional<std::string> preferredLocale;
        std::optional<long> publicUpdatesChannelId;
        std::optional<int> maxVideoChannelUsers;
        std::optional<int> approximateMemberCount;
        std::optional<int> approximatePresenceCount;
        welcomeScreen welcome_screen;
        std::optional<int> nsfwLevel;
        std::unordered_map<long, sticker> stickers;
        std::optional<bool> premiumProgressBarEnabled;

        guild modifyGuild(guild guildOptions);
        guild delteGuild();
        std::unordered_map<long, channel> getChannels();
        std::unordered_map<long, channel> fetchChannels();
        channel createChannel(channel channelOptions);
        channel modifyChannel(channel channelOptions);
        std::unordered_map<long, channel> getActiveThreads();
        std::unordered_map<long, channel> fetchActiveThreads();
        std::unordered_map<long, threadMember> getActiveThreadMembers(channel thread);
        std::unordered_map<long, threadMember> fetchActiveThreadMembers(channel thread);
        std::unordered_map<long, guildMember> getGuildMember(int limit = 1, long after = 0);
        std::unordered_map<long, guildMember> getGuildMember(int limit = 1, std::string after = 0);
        std::unordered_map<long, guildMember> fetchGuildMember(int limit = 1, long after = 0);
        std::unordered_map<long, guildMember> fetchGuildMember(int limit = 1, std::string after = 0);
        std::unordered_map<long, guildMember> searchGuildMember(std::string query = "", int limit = 1);
        guildMember addGuildMember(long userId, std::string access_token, guildMember guildMemberOptions = defaultGuildMember);
        guildMember addGuildMember(std::string userId, std::string access_token, guildMember guildMemberOptions = defaultGuildMember);
        guildMember modifyGuildMember(long userId, guildMember guildMemberOptions = defaultGuildMember);
        guildMember modifyGuildMember(std::string userId, guildMember guildMemberOptions = defaultGuildMember);
        guildMember modifyCurrentMember(std::string nick = "");
        void addGuildMemberRole(long userId, long roleId);
        void addGuildMemberRole(std::string userId, std::string roleId);
        void removeGuildMember(long userId);
        void removeGuildMember(std::string userId);
        std::unordered_map<user, ban> getGuildBans(int limit = 1000, long after = -1, long before = -1);
        std::unordered_map<user, ban> getGuildBans(int limit = 1000, std::string after = "", std::string before = "");
        ban getGuildBan(long userId);
        ban getGuildBan(std::string userId);
        void createGuildBan(long userId, int deleteMessageSeconds = 0);
        void createGuildBan(std::string userId, int deleteMessageSeconds = 0);
        void removeGuildBan(long userId);
        void removeGuildBan(std::string userId);
        std::unordered_map<long, role> getGuildRoles();
        std::unordered_map<long, role> fetchGuildRoles();
        role createGuildRole(role roleOptions);
        role modifyGuildRolePosition(long roleId, int position = -1);
        role modifyGuildRolePosition(std::string roleId, int position = -1);
        role modifyGuildRole(long roleId, role roleOptions);
        role modifyGuildRole(std::string roleId, role roleOptions);
        int modifyGuildMFALevel(const int& level);
        void deleteGuildRole(const long& roleId);
        void deleteGuildRole(const std::string& roleId);
        int getGuildPruneCount(const int& days = 7, const std::vector<long>& includeRoles = {});
        int getGuildPruneCount(const int& days = 7, const std::vector<std::string>& includeRoles = {});
        int beginGuildPrune(const int& days = 7, const bool& computePruneCound = true, const std::vector<long>& includeRoles = {});
        int beginGuildPrune(const int& days = 7, const bool& computePruneCound = true, const std::vector<std::string>& includeRoles = {});
        std::unordered_map<long,voiceRegion> getVoiceRegions();
        inviteMetadata getGuildInvites();
        integration getGuildIntegrations();
        void deleteIntegration(const int& integrationId);
        guildWidgetSettings getGuildWidgetSettings();
        guildWidget modifyGuildWidget();
        guildWidget getGuildWidget();
        invite getGuildVanityURL();
        std::string getGuildWidgetImage(const std::string& style);
        welcomeScreen getGuildWelcomeScreen();
        welcomeScreen modifyGuildWelcomeScreen(const welcomeScreen& welcomeScreenOptions);
        void modifyCurrentUserVoiceState(const voiceState& voiceStateOptions);
        void modifyUserVoiceState(const long& userId,const voiceState& voiceStateOptions);
        void modifyUserVoiceState(const std::string& userId,const voiceState& voiceStateOptions);
    };

    class guildOptions {
    private:
        friend class client;
        std::string token;
        std::unordered_map<long, guild> guilds;
    public:
        [[maybe_unused]] guild createGuild(const guild& guildOptions, const std::vector<role>& roles = {}, const std::vector<channel>& channels = {});
        [[maybe_unused]] [[nodiscard]] guild getGuild(const long& guildId, const bool& withCounts = false, const bool& cacheObject = true) const;
        guild getGuildFromCache(const long& guildId) const;
        bool guildExistsInCache(const long& guildId) const;
        guildPreview getGuildPreview(const long& guildId) const;
    };
}

#endif //HELIOS_GUILD_HPP
