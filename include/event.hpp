#ifndef HELIOS_EVENT_HPP
#define HELIOS_EVENT_HPP

#include <iostream>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace helios {
    class invite;
    class inviteMetadata;
    class user {
    public:
        bool operator==(const user& userToCompare) const;
        operator bool() const;
        std::optional<long> id;
        std::optional<std::string> username;
        std::optional<std::string> discriminator;
        std::optional<std::string> avatar;
        std::optional<bool> bot;
        std::optional<bool> system;
        std::optional<bool> mfa_enabled;
        std::optional<std::string> banner;
        std::optional<int> accent_color;
        std::optional<std::string> locale;
        std::optional<bool> verified;
        std::optional<std::string> email;
        std::optional<int> flags;
        std::optional<int> premium_type;
        std::optional<int> public_flags;
    };

    class ban {
        user user;
        std::string reason;
    };

    class roleTags {
    public:
        std::optional<long> bot_id;
        std::optional<long> integration_id;
        std::optional<bool> premium_subscriber;
        std::optional<long> subscription_listing_id;
        std::optional<bool> available_for_purchase;
        std::optional<bool> guild_connections;
    };

    class role {
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<int> color;
        std::optional<bool> hoist;
        std::optional<std::string> icon;
        std::optional<std::string> unicode_emoji;
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
        std::unordered_map<long, role> roles;
        user user;
        std::optional<bool> require_colons;
        std::optional<bool> managed;
        std::optional<bool> animated;
        std::optional<bool> available;
    };

    class welcomeScreenChannelStructure {
    public:
        std::optional <long> channel_id;
        std::optional <std::string> description;
        std::optional <long> emoji_id;
        std::optional <std::string> emoji_name;

    };

    class welcomeScreen {
    public:
        std::optional <std::string> description;
        std::vector<welcomeScreenChannelStructure> welcome_channels;
    };

    class sticker {
    public:
        std::optional<long> id;
        std::optional<long> pack_id;
        std::optional<std::string> name;
        std::optional<std::string> description;
        std::optional<std::string> tags;
        std::optional<std::string> asset;
        std::optional<int> type;
        std::optional<int> format_type;
        std::optional<bool> available;
        std::optional<long> guild_id;
        user user;
        std::optional<int> sort_value;
    };

    class guildMember {
    public:
        explicit operator bool() const;
        user user;
        std::optional<std::string> nick;
        std::optional<std::string> avatar;
        std::vector<role> roles;
        std::optional<std::string> joined_at;
        std::optional<std::string> premium_since;
        std::optional<bool> deaf;
        std::optional<bool> mute;
        std::optional<bool> pending;
        std::optional<std::string> permissions;
        std::optional<std::string> communication_disabled_until;
    };

    class voiceState {
    public:
        std::optional<long> guild_id;
        std::optional<long> channel_id;
        std::optional<long> user_id;
        guildMember member;
        std::optional<std::string> session_id;
        std::optional<bool> deaf;
        std::optional<bool> mute;
        std::optional<bool> self_deaf;
        std::optional<bool> self_mute;
        std::optional<bool> self_stream;
        std::optional<bool> self_video;
        std::optional<bool> suppress;
        std::optional<std::string> request_to_speak_timestamp;
    };

    class overwrite {
    public:
        std::optional<long> id;
        std::optional<int> type;
        std::optional<std::string> allow;
        std::optional<std::string>deny;
    };

    class threadMetadata {
    public:
        std::optional<bool> archived;
        std::optional<int> auto_archive_duration;
        std::optional<std::string> archive_timestamp;
        std::optional<bool> locked;
        std::optional<bool> invitable;
        std::optional<std::string> create_timestamp;
    };

    class threadMember {
        std::optional<long> id;
        std::optional<long> user_id;
        std::optional<std::string> join_timestamp;
        std::optional<int> flags;
        guildMember member;
    };

    class tag {
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<bool> moderated;
        std::optional<long> emoji_id;
        std::optional<std::string> emoji_name;
    };

    class defaultReaction {
    public:
        std::optional<long> emoji_id;
        std::optional<std::string> emoji_name;
    };

    class channel {
    public:
        std::optional<long> id;
        std::optional<int> type;
        std::optional<long> guild_id;
        std::optional<int> position;
        std::vector<overwrite> permission_overwrites;
        std::optional<std::string> name;
        std::optional<std::string> topic;
        std::optional<bool> nsfw;
        std::optional<long> last_message_id;
        std::optional<int> bitrate;
        std::optional<int> user_limit;
        std::optional<int> rate_limit_per_user;
        std::unordered_map<long, user> recipients;
        std::optional<std::string> icon;
        std::optional<long> owner_id;
        std::optional<long> application_id;
        std::optional<long> parent_id;
        std::optional<std::string> last_pin_timestamp;
        std::optional<std::string> rtc_region;
        std::optional<int> video_quality_mode;
        std::optional<int> message_count;
        std::optional<int> member_count;
        threadMetadata thread_metadata;
        threadMember member;
        std::optional<int> default_auto_archive_duration;
        std::optional<std::string> permissions;
        std::optional<int> flags;
        std::optional<int> total_message_sent;
        std::unordered_map<long, tag> available_tags;
        std::vector<long> applied_tags;
        defaultReaction default_reaction_emoji;
        std::optional<int> default_thread_rate_limit_per_use;
        std::optional<int> default_sort_order;
        std::optional<int> default_forum_layout;
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

    };

    class voiceRegion {
        std::optional<std::string> id;
        std::optional<std::string> name;
        std::optional<bool> optimal;
        std::optional<bool> deprecated;
        std::optional<bool> custom;
    };

    class guildPreview {

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

class guild {
private:
    static guildMember defaultGuildMember;
public:
    std::optional<long> id;
    std::optional<std::string> name;
    std::optional<std::string> icon;
    std::optional<std::string> icon_hash;
    std::optional<std::string> splash;
    std::optional<std::string> discovery_splash;
    std::optional<bool> owner;
    std::optional<long> owner_id;
    std::optional<std::string> permissions;
    std::optional<long> afk_channel_id;
    std::optional<int> afk_timeout;
    std::optional<bool> widget_enabled;
    std::optional<long> widget_channel_id;
    std::optional<int> verification_level;
    std::optional<int> default_message_notifications;
    std::optional<int> explicit_content_filter;
    std::unordered_map<long, role> roles;
    std::unordered_map<long, emoji> emojis;
    std::vector<std::string> features;
    std::optional<int> mfa_level;
    std::optional<long> application_id;
    std::optional<long> system_channel_id;
    std::optional<int> system_channel_flags;
    std::optional<long> rules_channel_id;
    std::optional<int> max_presences;
    std::optional<int> max_members;
    std::optional<std::string> vanity_url_code;
    std::optional<std::string> description;
    std::optional<std::string> banner;
    std::optional<int> premium_tier;
    std::optional<int> premium_subscription_count;
    std::optional<std::string> preferred_locale;
    std::optional<long> public_updates_channel_id;
    std::optional<int> max_video_channel_users;
    std::optional<int> approximate_member_count;
    std::optional<int> approximate_presence_count;
    welcomeScreen welcome_screen;
    std::optional<int> nsfw_level;
    std::unordered_map<long, sticker> stickers;
    std::optional<bool> premium_progress_bar_enabled;
    std::optional<std::string> joined_at;
    std::optional<bool> large;
    std::optional<int> member_count;
    std::unordered_map<long, voiceState> voiceStates;
    std::unordered_map<long, guildMember> members;
    std::unordered_map<long, channel> channels;
    std::unordered_map<long, channel> threads;
    std::unordered_map<long, presenceUpdate> presences;
    std::unordered_map<long, stageInstance> stage_instances;
    std::unordered_map<long, guildScheduledEvent> guild_scheduled_events;

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

    class unavailableGuilds {
    public:
        std::optional<std::string> id;
        std::optional<bool> unavailable;
    };

    class partialApplication {
    public:
        std::optional<std::string> id;
        std::optional<int> flags;
    };

    class readyEvent {
    public:
        user user;
        std::vector<unavailableGuilds> guilds;
        std::optional<int> shard[2];
        partialApplication application;
    };

    class resumedEvent {};

    class guildCreatEvent {
    public:
        std::optional<bool> unavailable;
        guild guild;
    };

    class eventData {
    private:
        friend class client;
        static user getUserData(const json& jsonData);
        static roleTags getRoleTagsData(const json& jsonData);
        static role getRoleData(const json& jsonData);
        static emoji getEmojiData(const json& jsonData);
        static partialApplication getPartialApplicationData(const json& jsonData);
        static unavailableGuilds getUnavailableGuildsData(const json& jsonData);
        static welcomeScreenChannelStructure getWelcomeScreenChannelStructureData(const json& jsonData);
        static welcomeScreen getWelcomeScreenData(const json& jsonData);
        static sticker getStickerData(const json& jsonData);
        static guild getGuildData(const json& jsonData);


        [[nodiscard]] readyEvent getReadyEventData(const json& jsonData);
        [[nodiscard]] resumedEvent getResumedEventData(const json& jsonData) const;
        [[nodiscard]] guildCreatEvent getGuildCreateEventData(const json& jsonData);
    public:
        resumedEvent resumedEventData;
        readyEvent readyEventData;
        guildCreatEvent guildCreateData;
    };

    class guildOptions {
    private:
        std::unordered_map<long, guild> guilds;
    public:
        guild create(guild guildOptions);
        guild get(long guildId);
        guild get(std::string guildId);
        guild fetch(long guildId);
        guild fetch(std::string guildId);
        guildPreview fetchPreview(long guildId);
        guildPreview fetchPreview(std::string guildId);
    };

    class channelOptions {
    private:
        std::unordered_map<long, channel> channels;
    public:
        channel create(const channel& channelOptions);
        channel get(const long& channelId) const;
        channel get(const std::string& channelId) const;
        channel fetch(const long& channelId) const;
        channel fetch(const std::string& channelId) const;
    };


    class onEvent {
    private:
        friend class client;
        std::function<void(readyEvent)> readyFunction;
        std::function<void(resumedEvent)> resumedFunction;

        std::function<void(guildCreatEvent)> guildCreateFunction;
        std::function<void(resumedEvent)> guildUpdateFunction;
        std::function<void(resumedEvent)> guildDeleteFunction;

    public:
        [[maybe_unused]] void ready(const std::function<void(readyEvent)>& userFunction);
        [[maybe_unused]] void resumed(const std::function<void(resumedEvent)>& userFunction);

        [[maybe_unused]] void applicationCommandPermissionsUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void autoModerationRuleCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationRuleUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationRuleDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void autoModerationActionExecution(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void channelCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void channelUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void channelDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void channelPinUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void threadCreate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadDelete(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadListSync(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadMemberUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void threadMembersUpdate(const std::function<void(eventData)>& userFunction);

        [[maybe_unused]] void guildCreate(const std::function<void(guildCreatEvent)>& userFunction);
        [[maybe_unused]] void guildUpdate(const std::function<void(eventData)>& userFunction);
        [[maybe_unused]] void guildDelete(const std::function<void(eventData)>& userFunction);
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
