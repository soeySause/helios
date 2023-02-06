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
#include "discordClassses/discordClasses.hpp"

using json = nlohmann::json;
using namespace boost::archive;

namespace helios {
    class readyEvent {
    public:
        user me;
        std::vector<unavailableGuild> guilds;
        std::optional<int> shard[2];
        application application;
    };

    class resumedEvent {};

    class applicationCommandPermissionsUpdateEvent {

    };

    class autoModerationRuleCreateEvent {

    };

    class autoModerationRuleUpdateEvent {

    };

    class autoModerationRuleDeleteEvent {

    };

    class autoModerationActionExecutionEvent {

    };

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

    class channelPinUpdateEvent {

    };

    class threadCreateEvent {

    };

    class threadUpdateEvent {

    };

    class threadDeleteEvent {

    };

    class threadListSyncEvent {

    };

    class threadMemberUpdateEvent {

    };

    class threadMembersUpdateEvent {

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

    class guildBanAddEvent {

    };

    class guildBanRemoveEvent {

    };

    class guildEmojiUpdateEvent {

    };

    class guildStickersUpdateEvent {

    };

    class guildIntegrationsUpdateEvent {

    };

    class guildMemberAddEvent {

    };

    class guildMemberRemoveEvent {

    };

    class guildMemberUpdateEvent {

    };

    class guildMembersChunkEvent {

    };

    class guildRoleCreateEvent {

    };

    class guildRoleUpdateEvent {

    };

    class guildRoleDeleteEvent {

    };

    class guildScheduledEventCreateEvent {

    };

    class guildScheduledEventUpdateEvent {

    };

    class guildScheduledEventDeleteEvent {

    };

    class guildScheduledEventUserAddEvent {

    };

    class guildScheduledEventUserRemoveEvent {

    };

    class integrationCreateEvent {

    };

    class integrationUpdateEvent {

    };

    class integrationDeleteEvent {

    };

    class inviteCreateEvent {

    };

    class inviteDeleteEvent {

    };

    class messageCreateEvent {

    };

    class messageUpdateEvent {

    };

    class messageDeleteEvent {

    };

    class messageDeleteBulkEvent {

    };

    class messageReactionAddEvent {

    };

    class messageReactionRemoveAllEvent {

    };

    class messageReactionRemoveEmojiEvent {

    };

    class presenceUpdateEvent {

    };

    class stageInstanceCreateEvent {

    };

    class stageInstanceUpdateEvent {

    };

    class stageInstanceDeleteEvent {

    };

    class typingStartEvent {

    };

    class userUpdateEvent {

    };

    class voiceStateUpdateEvent {

    };

    class voiceServerUpdateEvent {

    };

    class webhooksUpdateEvent {

    };



    class eventData {
    private:
        friend class client;

        [[nodiscard]] readyEvent getReadyEventData(const json& jsonData);
        [[nodiscard]] resumedEvent getResumedEventData(const json& jsonData) const;

        [[nodiscard]] applicationCommandPermissionsUpdateEvent getApplicationCommandPermissionsUpdateData(const json& jsonData);

        [[nodiscard]] autoModerationRuleCreateEvent getAutoModerationRuleCreateData(const json& jsonData);
        [[nodiscard]] autoModerationRuleUpdateEvent getAutoModerationRuleUpdateData(const json& jsonData);
        [[nodiscard]] autoModerationRuleDeleteEvent getAutoModerationRuleDeleteData(const json& jsonData);
        [[nodiscard]] autoModerationActionExecutionEvent getAutoModerationActionExecutionData(const json& jsonData);

        [[nodiscard]] channelCreateEvent getChannelCreateEventData(const json& jsonData);
        [[nodiscard]] channelUpdateEvent getChannelUpdateEventData(const json& jsonData);
        [[nodiscard]] channelDeleteEvent getChannelDeleteEventData(const json& jsonData);
        [[nodiscard]] channelPinUpdateEvent getChannelPinUpdateData(const json& jsonData);

        [[nodiscard]] threadCreateEvent getThreadCreateData(const json& jsonData);
        [[nodiscard]] threadUpdateEvent getThreadUpdateData(const json& jsonData);
        [[nodiscard]] threadDeleteEvent getThreadDeleteData(const json& jsonData);
        [[nodiscard]] threadListSyncEvent getThreadListSyncData(const json& jsonData);
        [[nodiscard]] threadMemberUpdateEvent getThreadMemberUpdateData(const json& jsonData);
        [[nodiscard]] threadMembersUpdateEvent getThreadMembersUpdateData(const json& jsonData);

        [[nodiscard]] guildCreateEvent getGuildCreateEventData(const json& jsonData);
        [[nodiscard]] guildUpdateEvent getGuildUpdateEventData(const json& jsonData);
        [[nodiscard]] guildDeleteEvent getGuildDeleteEventData(const json& jsonData);
        [[nodiscard]] guildBanAddEvent getGuildBanAddData(const json& jsonData);
        [[nodiscard]] guildBanRemoveEvent getGuildBanRemoveData(const json& jsonData);
        [[nodiscard]] guildEmojiUpdateEvent getGuildEmojiUpdateData(const json& jsonData);
        [[nodiscard]] guildStickersUpdateEvent getGuildStickersUpdateData(const json& jsonData);
        [[nodiscard]] guildIntegrationsUpdateEvent getGuildIntegrationsUpdateData(const json& jsonData);

        [[nodiscard]] guildMemberAddEvent getGuildMemberAddData(const json& jsonData);
        [[nodiscard]] guildMemberRemoveEvent getGuildMemberRemoveData(const json& jsonData);
        [[nodiscard]] guildMemberUpdateEvent getGuildMemberUpdateData(const json& jsonData);
        [[nodiscard]] guildMembersChunkEvent getGuildMembersChunkData(const json& jsonData);

        [[nodiscard]] guildRoleCreateEvent getGuildRoleCreateData(const json& jsonData);
        [[nodiscard]] guildRoleUpdateEvent getGuildRoleUpdateData(const json& jsonData);
        [[nodiscard]] guildRoleDeleteEvent getGuildRoleDeleteData(const json& jsonData);

        [[nodiscard]] guildScheduledEventCreateEvent getGuildScheduledEventCreateData(const json& jsonData);
        [[nodiscard]] guildScheduledEventUpdateEvent getGuildScheduledEventUpdateData(const json& jsonData);
        [[nodiscard]] guildScheduledEventDeleteEvent getGuildScheduledEventDeleteData(const json& jsonData);
        [[nodiscard]] guildScheduledEventUserAddEvent getGuildScheduledEventUserAddData(const json& jsonData);
        [[nodiscard]] guildScheduledEventUserRemoveEvent getGuildScheduledEventUserRemoveData(const json& jsonData);

        [[nodiscard]] integrationCreateEvent getIntegrationCreateData(const json& jsonData);
        [[nodiscard]] integrationUpdateEvent getIntegrationUpdateData(const json& jsonData);
        [[nodiscard]] integrationDeleteEvent getIntegrationDeleteData(const json& jsonData);

        [[nodiscard]] inviteCreateEvent getInviteCreateData(const json& jsonData);
        [[nodiscard]] inviteDeleteEvent getInviteDeleteData(const json& jsonData);

        [[nodiscard]] messageCreateEvent getMessageCreateData(const json& jsonData);
        [[nodiscard]] messageUpdateEvent getMessageUpdateData(const json& jsonData);
        [[nodiscard]] messageDeleteEvent getMessageDeleteData(const json& jsonData);
        [[nodiscard]] messageDeleteBulkEvent getMessageDeleteBulkData(const json& jsonData);
        [[nodiscard]] messageReactionAddEvent getMessageReactionAddData(const json& jsonData);
        [[nodiscard]] messageReactionRemoveAllEvent getMessageReactionRemoveAllData(const json& jsonData);
        [[nodiscard]] messageReactionRemoveEmojiEvent getMessageReactionRemoveEmojiData(const json& jsonData);

        [[nodiscard]] presenceUpdateEvent getPresenceUpdateData(const json& jsonData);

        [[nodiscard]] stageInstanceCreateEvent getStageInstanceCreateData(const json& jsonData);
        [[nodiscard]] stageInstanceUpdateEvent getStageInstanceUpdateData(const json& jsonData);
        [[nodiscard]] stageInstanceDeleteEvent getStageInstanceDeleteData(const json& jsonData);

        [[nodiscard]] typingStartEvent getTypingStartData(const json& jsonData);
        [[nodiscard]] userUpdateEvent getUserUpdateData(const json& jsonData);

        [[nodiscard]] voiceStateUpdateEvent getVoiceStateUpdateData(const json& jsonData);
        [[nodiscard]] voiceServerUpdateEvent getVoiceServerUpdateData(const json& jsonData);

        [[nodiscard]] webhooksUpdateEvent getWebhooksUpdateEvent(const json& jsonData);

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

    class onEvent {
    private:
        friend class client;
        std::function<void(readyEvent)> readyFunction;
        std::function<void(resumedEvent)> resumedFunction;

        std::function<void(applicationCommandPermissionsUpdateEvent)> applicationCommandPermissionsUpdateFunction;

        std::function<void(autoModerationRuleCreateEvent)> autoModerationRuleCreateFunction;
        std::function<void(autoModerationRuleUpdateEvent)> autoModerationRuleUpdateFunction;
        std::function<void(autoModerationRuleDeleteEvent)> autoModerationRuleDeleteFunction;
        std::function<void(autoModerationActionExecutionEvent)> autoModerationActionExecutionFunction;

        std::function<void(channelCreateEvent)> channelCreateFunction;
        std::function<void(channelUpdateEvent)> channelUpdateFunction;
        std::function<void(channelDeleteEvent)> channelDeleteFunction;
        std::function<void(channelPinUpdateEvent)> channelPinUpdateFunction;

        std::function<void(threadCreateEvent)> threadCreateFunction;
        std::function<void(threadUpdateEvent)> threadUpdateFunction;
        std::function<void(threadDeleteEvent)> threadDeleteFunction;
        std::function<void(threadListSyncEvent)> threadListSyncFunction;
        std::function<void(threadMemberUpdateEvent)> threadMemberUpdateFunction;
        std::function<void(threadMembersUpdateEvent)> threadMembersUpdateFunction;

        std::function<void(guildCreateEvent)> guildCreateFunction;
        std::function<void(guildUpdateEvent)> guildUpdateFunction;
        std::function<void(guildDeleteEvent)> guildDeleteFunction;
        std::function<void(guildBanAddEvent)> guildBanAddFunction;
        std::function<void(guildBanRemoveEvent)> guildBanRemoveFunction;
        std::function<void(guildEmojiUpdateEvent)> guildEmojiUpdateFunction;
        std::function<void(guildStickersUpdateEvent)> guildStickersUpdateFunction;
        std::function<void(guildIntegrationsUpdateEvent)> guildIntegrationsUpdateFunction;

        std::function<void(guildMemberAddEvent)> guildMemberAddFunction;
        std::function<void(guildMemberRemoveEvent)> guildMemberRemoveFunction;
        std::function<void(guildMemberUpdateEvent)> guildMemberUpdateFunction;
        std::function<void(guildMembersChunkEvent)> guildMembersChunkFunction;
        std::function<void(guildRoleCreateEvent)> guildRoleCreateFunction;
        std::function<void(guildRoleUpdateEvent)> guildRoleUpdateFunction;
        std::function<void(guildRoleDeleteEvent)> guildRoleDeleteFunction;

        std::function<void(guildScheduledEventCreateEvent)> guildScheduledEventCreateFunction;
        std::function<void(guildScheduledEventUpdateEvent)> guildScheduledEventUpdateFunction;
        std::function<void(guildScheduledEventDeleteEvent)> guildScheduledEventDeleteFunction;
        std::function<void(guildScheduledEventUserAddEvent)> guildScheduledEventUserAddFunction;
        std::function<void(guildScheduledEventUserRemoveEvent)> guildScheduledEventUserRemoveFunction;

        std::function<void(integrationCreateEvent)> integrationCreateFunction;
        std::function<void(integrationUpdateEvent)> integrationUpdateFunction;
        std::function<void(integrationDeleteEvent)> integrationDeleteFunction;

        std::function<void(inviteCreateEvent)> inviteCreateFunction;
        std::function<void(inviteDeleteEvent)> inviteDeleteFunction;

        std::function<void(messageCreateEvent)> messageCreateFunction;
        std::function<void(messageUpdateEvent)> messageUpdateFunction;
        std::function<void(messageDeleteEvent)> messageDeleteFunction;
        std::function<void(messageDeleteBulkEvent)> messageDeleteBulkFunction;
        std::function<void(messageReactionAddEvent)> messageReactionAddFunction;
        std::function<void(messageReactionRemoveAllEvent)> messageReactionRemoveAllFunction;
        std::function<void(messageReactionRemoveEmojiEvent)> messageReactionRemoveEmojiFunction;

        std::function<void(presenceUpdateEvent)> presenceUpdateFunction;

        std::function<void(stageInstanceCreateEvent)> stageInstanceCreateFunction;
        std::function<void(stageInstanceUpdateEvent)> stageInstanceUpdateFunction;
        std::function<void(stageInstanceDeleteEvent)> stageInstanceDeleteFunction;

        std::function<void(typingStartEvent)> typingStartFunction;
        std::function<void(userUpdateEvent)> userUpdateFunction;

        std::function<void(voiceStateUpdateEvent)> voiceStateUpdateFunction;
        std::function<void(voiceServerUpdateEvent)> voiceServerUpdateFunction;

        std::function<void(webhooksUpdateEvent)> webhooksUpdateFunction;


    public:
        [[maybe_unused]] void ready(const std::function<void(readyEvent)>& userFunction);
        [[maybe_unused]] void resumed(const std::function<void(resumedEvent)>& userFunction);

        [[maybe_unused]] void applicationCommandPermissionsUpdate(const std::function<void(applicationCommandPermissionsUpdateEvent)>& userFunction);

        [[maybe_unused]] void autoModerationRuleCreate(const std::function<void(autoModerationRuleCreateEvent)>& userFunction);
        [[maybe_unused]] void autoModerationRuleUpdate(const std::function<void(autoModerationRuleUpdateEvent)>& userFunction);
        [[maybe_unused]] void autoModerationRuleDelete(const std::function<void(autoModerationRuleDeleteEvent)>& userFunction);
        [[maybe_unused]] void autoModerationActionExecution(const std::function<void(autoModerationActionExecutionEvent)>& userFunction);

        [[maybe_unused]] void channelCreate(const std::function<void(channelCreateEvent)>& userFunction);
        [[maybe_unused]] void channelUpdate(const std::function<void(channelUpdateEvent)>& userFunction);
        [[maybe_unused]] void channelDelete(const std::function<void(channelDeleteEvent)>& userFunction);
        [[maybe_unused]] void channelPinUpdate(const std::function<void(channelPinUpdateEvent)>& userFunction);

        [[maybe_unused]] void threadCreate(const std::function<void(threadCreateEvent)>& userFunction);
        [[maybe_unused]] void threadUpdate(const std::function<void(threadUpdateEvent)>& userFunction);
        [[maybe_unused]] void threadDelete(const std::function<void(threadDeleteEvent)>& userFunction);
        [[maybe_unused]] void threadListSync(const std::function<void(threadListSyncEvent)>& userFunction);
        [[maybe_unused]] void threadMemberUpdate(const std::function<void(threadMemberUpdateEvent)>& userFunction);
        [[maybe_unused]] void threadMembersUpdate(const std::function<void(threadMembersUpdateEvent)>& userFunction);

        [[maybe_unused]] void guildCreate(const std::function<void(guildCreateEvent)>& userFunction);
        [[maybe_unused]] void guildUpdate(const std::function<void(guildUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildDelete(const std::function<void(guildDeleteEvent)>& userFunction);
        [[maybe_unused]] void guildBanAdd(const std::function<void(guildBanAddEvent)>& userFunction);
        [[maybe_unused]] void guildBanRemove(const std::function<void(guildBanRemoveEvent)>& userFunction);
        [[maybe_unused]] void guildEmojiUpdate(const std::function<void(guildEmojiUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildStickersUpdate(const std::function<void(guildStickersUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildIntegrationsUpdate(const std::function<void(guildIntegrationsUpdateEvent)>& userFunction);

        [[maybe_unused]] void guildMemberAdd(const std::function<void(guildMemberAddEvent)>& userFunction);
        [[maybe_unused]] void guildMemberRemove(const std::function<void(guildMemberRemoveEvent)>& userFunction);
        [[maybe_unused]] void guildMemberUpdate(const std::function<void(guildMemberUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildMembersChunk(const std::function<void(guildMembersChunkEvent)>& userFunction);

        [[maybe_unused]] void guildRoleCreate(const std::function<void(guildRoleCreateEvent)>& userFunction);
        [[maybe_unused]] void guildRoleUpdate(const std::function<void(guildRoleUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildRoleDelete(const std::function<void(guildRoleDeleteEvent)>& userFunction);

        [[maybe_unused]] void guildScheduledEventCreate(const std::function<void(guildScheduledEventCreateEvent)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUpdate(const std::function<void(guildScheduledEventUpdateEvent)>& userFunction);
        [[maybe_unused]] void guildScheduledEventDelete(const std::function<void(guildScheduledEventDeleteEvent)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUserAdd(const std::function<void(guildScheduledEventUserAddEvent)>& userFunction);
        [[maybe_unused]] void guildScheduledEventUserRemove(const std::function<void(guildScheduledEventUserRemoveEvent)>& userFunction);

        [[maybe_unused]] void integrationCreate(const std::function<void(integrationCreateEvent)>& userFunction);
        [[maybe_unused]] void integrationUpdate(const std::function<void(integrationUpdateEvent)>& userFunction);
        [[maybe_unused]] void integrationDelete(const std::function<void(integrationDeleteEvent)>& userFunction);

        [[maybe_unused]] void inviteCreate(const std::function<void(inviteCreateEvent)>& userFunction);
        [[maybe_unused]] void inviteDelete(const std::function<void(inviteDeleteEvent)>& userFunction);

        [[maybe_unused]] void messageCreate(const std::function<void(messageCreateEvent)>& userFunction);
        [[maybe_unused]] void messageUpdate(const std::function<void(messageUpdateEvent)>& userFunction);
        [[maybe_unused]] void messageDelete(const std::function<void(messageDeleteEvent)>& userFunction);
        [[maybe_unused]] void messageDeleteBulk(const std::function<void(messageDeleteBulkEvent)>& userFunction);
        [[maybe_unused]] void messageReactionAdd(const std::function<void(messageReactionAddEvent)>& userFunction);
        [[maybe_unused]] void messageReactionRemoveAll(const std::function<void(messageReactionRemoveAllEvent)>& userFunction);
        [[maybe_unused]] void messageReactionRemoveEmoji(const std::function<void(messageReactionRemoveEmojiEvent)>& userFunction);

        [[maybe_unused]] void presenceUpdate(const std::function<void(presenceUpdateEvent)>& userFunction);

        [[maybe_unused]] void stageInstanceCreate(const std::function<void(stageInstanceCreateEvent)>& userFunction);
        [[maybe_unused]] void stageInstanceUpdate(const std::function<void(stageInstanceUpdateEvent)>& userFunction);
        [[maybe_unused]] void stageInstanceDelete(const std::function<void(stageInstanceDeleteEvent)>& userFunction);

        [[maybe_unused]] void typingStart(const std::function<void(typingStartEvent)>& userFunction);
        [[maybe_unused]] void userUpdate(const std::function<void(userUpdateEvent)>& userFunction);

        [[maybe_unused]] void voiceStateUpdate(const std::function<void(voiceStateUpdateEvent)>& userFunction);
        [[maybe_unused]] void voiceServerUpdate(const std::function<void(voiceServerUpdateEvent)>& userFunction);

        [[maybe_unused]] void webhooksUpdate(const std::function<void(webhooksUpdateEvent)>& userFunction);
    };
}

#endif //HELIOS_EVENT_HPP
