#ifndef HELIOS_EVENT_HPP
#define HELIOS_EVENT_HPP

#include <boost/serialization/string.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include "request.hpp"

using json = nlohmann::json;
using namespace boost::archive;

namespace helios {
    class invite;
    class inviteMetadata;
    class guildOptions;
    class user {
    public:
        bool operator==(const user& userToCompare) const;
        explicit operator bool() const;
        std::optional<long> id;
        std::optional<std::string> username;
        std::optional<std::string> discriminator;
        std::optional<std::string> avatar;
        std::optional<bool> bot;
        std::optional<bool> system;
        std::optional<bool> mfaEnabled;
        std::optional<std::string> banner;
        std::optional<int> accentColor;
        std::optional<std::string> locale;
        std::optional<bool> verified;
        std::optional<std::string> email;
        std::optional<int> flags;
        std::optional<int> premiumType;
        std::optional<int> publicFlags;
    };

    class roleTags {
    public:
        std::optional<long> botId;
        std::optional<long> integrationId;
        std::optional<bool> premiumSubscriber;
        std::optional<long> subscriptionListing_id;
        std::optional<bool> availableForPurchase;
        std::optional<bool> guildConnections;
    };

    class role {
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<int> color;
        std::optional<bool> hoist;
        std::optional<std::string> icon;
        std::optional<std::string> unicodeEmoji;
        std::optional<int> position;
        std::optional<std::string> permissions;
        std::optional<bool> managed;
        std::optional<bool> mentionable;
        roleTags tags;
    };

    class emoji {
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::vector<long> roles;
        user user;
        std::optional<bool> require_colons;
        std::optional<bool> managed;
        std::optional<bool> animated;
        std::optional<bool> available;
    };

    class welcomeScreenChannelStructure {
    public:
        std::optional <long> channelId;
        std::optional <std::string> description;
        std::optional <long> emojiId;
        std::optional <std::string> emojiName;

    };

    class welcomeScreen {
    private:
        friend class boost::serialization::access;

        template <typename Archive>
        [[maybe_unused]] void serialize(Archive &ar, const unsigned int version) {
            ar & description;
            ar & welcome_channels;

        }


        public:
        std::optional <std::string> description;
        std::vector<welcomeScreenChannelStructure> welcome_channels;
    };

    class sticker {
    public:
        std::optional<long> id;
        std::optional<long> packId;
        std::optional<std::string> name;
        std::optional<std::string> description;
        std::optional<std::string> tags;
        std::optional<std::string> asset;
        std::optional<int> type;
        std::optional<int> formatType;
        std::optional<bool> available;
        std::optional<long> guildId;
        user user;
        std::optional<int> sortValue;
    };

    class guildMember {
    private:
    public:
        explicit operator bool() const;
        user user;
        std::optional<std::string> nick;
        std::optional<std::string> avatar;
        std::vector<long> roles;
        std::optional<std::string> joinedAt;
        std::optional<std::string> premiumSince;
        std::optional<bool> deaf;
        std::optional<bool> mute;
        std::optional<bool> pending;
        std::optional<std::string> permissions;
        std::optional<std::string> communicationDisabledUntil;
        std::optional<long> guildId;
    };

    class voiceState {
    public:
        std::optional<long> guildId;
        std::optional<long> channelId;
        std::optional<long> userId;
        guildMember member;
        std::optional<std::string> session_id;
        std::optional<bool> deaf;
        std::optional<bool> mute;
        std::optional<bool> selfDeaf;
        std::optional<bool> selfMute;
        std::optional<bool> selfStream;
        std::optional<bool> selfVideo;
        std::optional<bool> suppress;
        std::optional<std::string> requestToSpeakTimestamp;
    };

    class overwrite {
    public:
        std::optional<long> id;
        std::optional<std::string> type;
        std::optional<std::string> allow;
        std::optional<std::string>deny;
    };

    class threadMetadata {
    public:
        std::optional<bool> archived;
        std::optional<int> autoArchiveDuration;
        std::optional<std::string> archiveTimestamp;
        std::optional<bool> locked;
        std::optional<bool> invitable;
        std::optional<std::string> createTimestamp;
    };

    class threadMember {
    public:
        std::optional<long> userId;
        std::optional<std::string> joinTimestamp;
        std::optional<int> flags;
        guildMember member;
        std::optional<long> id;
    };

    class tag {
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<bool> moderated;
        std::optional<long> emojiId;
        std::optional<std::string> emojiName;
    };

    class defaultReaction {
    public:
        std::optional<long> emojiId;
        std::optional<std::string> emojiName;
    };

    class channel {
    public:
        std::optional<long> id;
        std::optional<int> type;
        std::optional<long> guildId;
        std::optional<int> position;
        std::unordered_map<long, overwrite> permissionOverwrites;
        std::optional<std::string> name;
        std::optional<std::string> topic;
        std::optional<bool> nsfw;
        std::optional<long> lastMessageId;
        std::optional<int> bitrate;
        std::optional<int> userLimit;
        std::optional<int> rateLimitPerUser;
        std::unordered_map<long, user> recipients;
        std::optional<std::string> icon;
        std::optional<long> ownerId;
        std::optional<long> applicationId;
        std::optional<long> parentId;
        std::optional<std::string> lastPinTimestamp;
        std::optional<std::string> rtcRegion;
        std::optional<int> videoQualityMode;
        std::optional<int> messageCount;
        std::optional<int> memberCount;
        threadMetadata thread_metadata;
        threadMember member;
        std::optional<int> defaultAutoArchiveDuration;
        std::optional<std::string> permissions;
        std::optional<int> flags;
        std::optional<int> totalMessageSent;
        std::unordered_map<long, tag> availableTags;
        std::vector<long> appliedTags;
        defaultReaction defaultReactionEmoji;
        std::optional<int> defaultThreadRateLimitPerUse;
        std::optional<int> defaultSortOrder;
        std::optional<int> defaultForumLayout;
    };

    class timestamps {

    };

    class party {

    };

    class assets {

    };

    class secrets {

    };

    class buttons {

    };

    class activity {
        std::optional<std::string> name;
        std::optional<int> type;
        std::optional<std::string> url;
        std::optional<int> createdAt;
        timestamps timestamps;
        std::optional<long> applicationId;
        std::optional<std::string> details;
        std::optional<std::string> state;
        emoji emoji;
        party party;
        assets assets;
        secrets secrets;
        std::optional<bool> instance;
        std::optional<int> flags;
        buttons buttons;
    };

    class presenceUpdate {
        user user;
        std::optional<long> guildId;
        std::optional<std::string> status;

    };

    class stageInstance {
        std::optional<long> id;
        std::optional<long> guildId;
        std::optional<long> channelId;
        std::optional<std::string> topic;
        std::optional<int> privacyLevel;
        std::optional<bool> discoveryDisabled;
        std::optional<long> guildScheduledEventId;
    };

    class guildScheduledEvent {

    };


    class application {
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<std::string> icon;
        std::optional<std::string> description;
        user bot;

    };

    class inviteStageInstance {
    public:
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version) { ar & id; }
        int id;
    };

    class voiceRegion {
        std::optional<std::string> id;
        std::optional<std::string> name;
        std::optional<bool> optimal;
        std::optional<bool> deprecated;
        std::optional<bool> custom;
    };

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

    class account {
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
        account account;
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

    class guild {
private:
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
        //std::ar & roles;
        //std::ar & emojis;
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
        //std::ar & stickers;
        ar & premiumProgressBarEnabled;

    }

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

    class invite {
        std::optional<std::string> code;
        guild guild;
        channel channel;
        user inviter;
        std::optional<int> targetType;
        user targetUser;
        application targetApplication;
        std::optional<int> approximatePresenceCount;
        std::optional<int> approximateMemberCount;
        std::optional<std::string> expiresAt;
        inviteStageInstance stageInstance;
        guildScheduledEvent guildScheduledEvent;
    };

    class inviteMetadata : public invite {
        std::unordered_map<long, guildMember> members;
        std::optional<int> participantCount;
        std::optional<int> speakerCount;
        std::optional<std::string> topic;
    };

    class unavailableGuild {
    public:
        std::optional<long> id;
        std::optional<bool> unavailable;
    };

    class partialApplication {
    public:
        std::optional<long> id;
        std::optional<int> flags;
    };

    class guildCacheObject {
        guild guild;
        std::optional<std::string> joinedAt;
        std::optional<bool> large;
        std::optional<bool> unavailable;
        std::optional<int> memberCount;
        std::unordered_map<long, voiceState> voiceStates;
        std::unordered_map<long, guildMember> members;
        std::unordered_map<long, channel> channels;
        std::unordered_map<long, channel> threads;
        std::unordered_map<long, presenceUpdate> presences;
        std::unordered_map<long, stageInstance> stageInstances;
        std::unordered_map<long, guildScheduledEvent> guildScheduledEvents;
    };

    class readyEvent {
    public:
        user user;
        std::vector<unavailableGuild> guilds;
        std::optional<int> shard[2];
        partialApplication application;
    };

    class resumedEvent {};

    class channelCreateEvent {
    public:
        channel channel;
    };

    class channelUpdateEvent {
    public:
        channel channel;
    };

    class channelDeleteEvent {
    public:
        channel channel;
    };

    class guildCreateEvent {
    public:
        guild guild;
        std::optional<std::string> joinedAt;
        std::optional<bool> large;
        std::optional<bool> unavailable;
        std::optional<int> memberCount;
        std::unordered_map<long, voiceState> voiceStates;
        std::unordered_map<long, guildMember> members;
        std::unordered_map<long, channel> channels;
        std::unordered_map<long, channel> threads;
        std::unordered_map<long, presenceUpdate> presences;
        std::unordered_map<long, stageInstance> stageInstances;
        std::unordered_map<long, guildScheduledEvent> guildScheduledEvents;

    };

    class guildUpdateEvent {
    public:
        guild guild;
    };

    class guildDeleteEvent {
    public:
        unavailableGuild unavailableGuild;
    };

    class eventData {
    private:
        friend class client;
        friend class guildOptions;
        friend class channelOptions;
        static user getUserData(const json& jsonData);
        static roleTags getRoleTagsData(const json& jsonData);
        static role getRoleData(const json& jsonData);
        static emoji getEmojiData(const json& jsonData);
        static partialApplication getPartialApplicationData(const json& jsonData);
        static unavailableGuild getUnavailableGuildData(const json& jsonData);
        static welcomeScreenChannelStructure getWelcomeScreenChannelStructureData(const json& jsonData);
        static welcomeScreen getWelcomeScreenData(const json& jsonData);
        static sticker getStickerData(const json& jsonData);
        static guild getGuildData(const json& jsonData);
        static guildMember getGuildMemberData(const json &jsonData, const long& guildId);
        static voiceState getVoiceStateData(const json& jsonData, const long& guildId);
        static guildPreview getGuildPreviewData(const json& jsonData);
        static overwrite getOverwriteData(const json& jsonData);
        static channel getChannelData(const json& jsonData, const long& guildId = 0);
        static threadMetadata getThreadMetadataData(const json& jsonData);
        static threadMember getThreadMemberData(const json& jsonData, const long& guildId);
        static tag getTagData(const json& jsonData);
        static defaultReaction getDefaultReactionData(const json& jsonData);

        [[nodiscard]] readyEvent getReadyEventData(const json& jsonData);
        [[nodiscard]] resumedEvent getResumedEventData(const json& jsonData) const;

        [[nodiscard]] channelCreateEvent getChannelCreateEventData(const json& jsonData);
        [[nodiscard]] channelUpdateEvent getChannelUpdateEventData(const json& jsonData);
        [[nodiscard]] channelDeleteEvent getChannelDeleteEventData(const json& jsonData);

        [[nodiscard]] guildCreateEvent getGuildCreateEventData(const json& jsonData);
        [[nodiscard]] guildUpdateEvent getGuildUpdateEventData(const json& jsonData);
        [[nodiscard]] guildDeleteEvent getGuildDeleteEventData(const json& jsonData);

        static void cacheGuild(const guildCacheObject& guildToBeCached);
        static void getGuildFromCache(const long& guildIdToGetFromCache, const int& shard);
        static void removeGuildFromCache(const long& guildIdToBeDeletedFromCache);
        static void removeAllGuildsFromCache();
    public:
        readyEvent readyEventData;
        resumedEvent resumedEventData;

        channelCreateEvent channelCreateData;
        channelUpdateEvent channelUpdateData;
        channelDeleteEvent channelDeleteData;

        guildCreateEvent guildCreateData;
        guildUpdateEvent guildUpdateData;
        guildDeleteEvent guildDeleteData;
    };


    class guildOptions {
    private:
        friend class client;
        std::string token;
        std::unordered_map<long, guild> guilds;
    public:
        [[maybe_unused]] guild createGuild(const guild& guildOptions, const std::vector<role>& roles = {}, const std::vector<channel>& channels = {});
        [[maybe_unused]] guild getGuild(const long& guildId, const bool& withCounts = false, const bool& cacheObject = true) const;
        [[maybe_unused]] guild getGuild(const std::string& guildId, const bool& withCounts = true, const bool& cacheObject = true) const;
        guild getGuildFromCache(const long& guildId) const;
        guild getGuildFromCache(const std::string& guildId) const;
        bool guildExistsInCache(const long& guildId) const;
        bool guildExistsInCache(const std::string& guildId) const;
        guildPreview getGuildPreview(const long& guildId) const;
        guildPreview getGuildPreview(const std::string& guildId) const;
    };

    class channelOptions {
    private:
        std::unordered_map<long, channel> channels;
    public:
        channel create(const channel& channelOptions);
        channel get(const long& channelId,const bool& cacheObject = true) const;
        channel get(const std::string& channelId,const bool& cacheObject = true) const;
        channel getFromCache(const long& channelId) const;
        channel getFromCache(const std::string& channelId) const;
        bool existsInCache(const long& guildId) const;
        bool existsInCache(const std::string& guildId) const;
    };


    class onEvent {
    private:
        friend class client;
        std::function<void(readyEvent)> readyFunction;
        std::function<void(resumedEvent)> resumedFunction;

        std::function<void(channelCreateEvent)> channelCreateFunction;
        std::function<void(channelUpdateEvent)> channelUpdateFunction;
        std::function<void(channelDeleteEvent)> channelDeleteFunction;

        std::function<void(guildCreateEvent)> guildCreateFunction;
        std::function<void(guildUpdateEvent)> guildUpdateFunction;
        std::function<void(guildDeleteEvent)> guildDeleteFunction;

    public:
        [[maybe_unused]] void ready(const std::function<void(readyEvent)>& userFunction);
        [[maybe_unused]] void resumed(const std::function<void(resumedEvent)>& userFunction);

        [[maybe_unused]] void applicationCommandPermissionsUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void autoModerationRuleCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationRuleUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationRuleDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationActionExecution(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void channelCreate(const std::function<void(channelCreateEvent)>& userFunction);
        [[maybe_unused]] void channelUpdate(const std::function<void(channelUpdateEvent)>& userFunction);
        [[maybe_unused]] void channelDelete(const std::function<void(channelDeleteEvent)>& userFunction);
        [[maybe_unused]] void channelPinUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void threadCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadListSync(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadMemberUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadMembersUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildCreate(const std::function<void(guildCreateEvent)>& userFunction);
        [[maybe_unused]] void guildUpdate(const std::function<void(guildUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildDelete(const std::function<void(guildDeleteEvent)>& userFunction);
        [[maybe_unused]] void guildBanAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildBanRemove(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildEmojiUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildStickersUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildIntegrationsUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildMemberAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildMemberRemove(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildMemberUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildMembersChunk(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildRoleCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildRoleUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildRoleDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildScheduledEventCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUserAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUserRemove(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void integrationCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void integrationUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void integrationDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void inviteCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void inviteDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void messageCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageDeleteBulk(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageReactionAdd(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageReactionRemoveAll(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void messageReactionRemoveEmoji(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void presenceUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void stageInstanceCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void stageInstanceUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void stageInstanceDelete(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void typingStart(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void userUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void voiceStateUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void voiceServerUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void webhooksUpdate(const std::function<void(eventData)>& userFunction);
    };
}

#endif //HELIOS_EVENT_HPP
