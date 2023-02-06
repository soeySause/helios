#include "event.hpp"
#include "discordClassses/guild/guild.hpp"

namespace helios {
    readyEvent eventData::getReadyEventData(const json &jsonData) {
        this->readyEventData.me = user::getUserData(jsonData["user"]);

        for(const nlohmann::basic_json<>& guild : jsonData["guilds"]) {
            this->readyEventData.guilds.emplace_back(unavailableGuild::getUnavailableGuildData(guild));
        }

        if(jsonData.contains("shard")) {
            this->readyEventData.shard[0] = jsonData["shard"].front();
            this->readyEventData.shard[1] = jsonData["shard"].back();
        }

        this->readyEventData.application = application::getApplicationData(jsonData["application"]);
        return this->readyEventData;
    }

    resumedEvent eventData::getResumedEventData(const json &jsonData) const {
        return this->resumedEventData;
    }

    channelCreateEvent eventData::getChannelCreateEventData(const json &jsonData) {
        this->channelCreateData.channel = channel::getChannelData(jsonData);
        return this->channelCreateData;
    }

    channelUpdateEvent eventData::getChannelUpdateEventData(const json &jsonData) {
        this->channelUpdateData.channel = channel::getChannelData(jsonData);
        return this->channelUpdateData;
    }

    channelDeleteEvent eventData::getChannelDeleteEventData(const json &jsonData) {
        this->channelDeleteData.channel = channel::getChannelData(jsonData);
        return this->channelDeleteData;
    }


    guildCreateEvent eventData::getGuildCreateEventData(const json& jsonData) {
        this->guildCreateData.guild = guild::getGuildData(jsonData);

        if(jsonData.contains("joined_at")) {this->guildCreateData.joinedAt = jsonData["joined_at"]; }
        if(jsonData.contains("large")) {this->guildCreateData.large = jsonData["large"]; }
        if(jsonData.contains("unavailable")) {this->guildCreateData.unavailable = jsonData["unavailable"]; }
        if(jsonData.contains("member_count")) {this->guildCreateData.memberCount = jsonData["member_count"]; }

        if(jsonData.contains("voice_states")) {
            for (const nlohmann::basic_json<> &voiceState: jsonData["voice_states"]) {
                this->guildCreateData.voiceStates[std::stol(voiceState["user_id"].get<std::string>())] = voiceState::getVoiceStateData(voiceState);
            }
        }

        if(jsonData.contains("members")) {
            for (const nlohmann::basic_json<> &member: jsonData["members"]) {
                this->guildCreateData.members[std::stol(member["user"]["id"].get<std::string>())] = guildMember::getGuildMemberData(member);
            }
        }

        if(jsonData.contains("channels")) {
            for (const nlohmann::basic_json<> &channel: jsonData["channels"]) {
                this->guildCreateData.channels[std::stol(channel["id"].get<std::string>())] = channel::getChannelData(channel);
            }
        }

        if(jsonData.contains("threads")) {
            for (const nlohmann::basic_json<> &thread: jsonData["threads"]) {
                this->guildCreateData.threads[std::stol(thread["id"].get<std::string>())] = channel::getChannelData(thread);
            }
        }
        return this->guildCreateData;
    }

    guildUpdateEvent eventData::getGuildUpdateEventData(const json &jsonData) {
        this->guildUpdateData.guild = guild::getGuildData(jsonData);
        return this->guildUpdateData;
    }

    guildDeleteEvent eventData::getGuildDeleteEventData(const json &jsonData) {
        this->guildDeleteData.unavailableGuild = unavailableGuild::getUnavailableGuildData(jsonData);
        return this->guildDeleteData;
    }


    [[maybe_unused]] void onEvent::ready(const std::function<void(readyEvent)>& userFunction) {
        this->readyFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::resumed(const std::function<void(resumedEvent)>& userFunction) {
        this->resumedFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::applicationCommandPermissionsUpdate(const std::function<void(applicationCommandPermissionsUpdateEvent)>& userFunction) {
        this->applicationCommandPermissionsUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::autoModerationRuleCreate(const std::function<void(autoModerationRuleCreateEvent)>& userFunction) {
        this->autoModerationRuleCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::autoModerationRuleUpdate(const std::function<void(autoModerationRuleUpdateEvent)>& userFunction) {
        this->autoModerationRuleUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::autoModerationRuleDelete(const std::function<void(autoModerationRuleDeleteEvent)>& userFunction) {
        this->autoModerationRuleDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::autoModerationActionExecution(const std::function<void(autoModerationActionExecutionEvent)>& userFunction) {
        this->autoModerationActionExecutionFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelCreate(const std::function<void(channelCreateEvent)>& userFunction) {
        this->channelCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelUpdate(const std::function<void(channelUpdateEvent)>& userFunction) {
        this->channelUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelDelete(const std::function<void(channelDeleteEvent)>& userFunction) {
        this->channelDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::channelPinUpdate(const std::function<void(channelPinUpdateEvent)>& userFunction) {
        this->channelPinUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::threadCreate(const std::function<void(threadCreateEvent)>& userFunction) {
        this->threadCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::threadUpdate(const std::function<void(threadUpdateEvent)>& userFunction) {
        this->threadUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::threadDelete(const std::function<void(threadDeleteEvent)>& userFunction) {
        this->threadDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::threadListSync(const std::function<void(threadListSyncEvent)>& userFunction) {
        this->threadListSyncFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::threadMemberUpdate(const std::function<void(threadMemberUpdateEvent)>& userFunction) {
        this->threadMemberUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::threadMembersUpdate(const std::function<void(threadMembersUpdateEvent)>& userFunction) {
        this->threadMembersUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildCreate(const std::function<void(guildCreateEvent)>& userFunction) {
        this->guildCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildUpdate(const std::function<void(guildUpdateEvent)>& userFunction) {
        this->guildUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildDelete(const std::function<void(guildDeleteEvent)>& userFunction) {
        this->guildDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildBanAdd(const std::function<void(guildBanAddEvent)>& userFunction) {
        this->guildBanAddFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildBanRemove(const std::function<void(guildBanRemoveEvent)>& userFunction) {
        this->guildBanRemoveFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildEmojiUpdate(const std::function<void(guildEmojiUpdateEvent)>& userFunction) {
        this->guildEmojiUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildStickersUpdate(const std::function<void(guildStickersUpdateEvent)>& userFunction) {
        this->guildStickersUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildIntegrationsUpdate(const std::function<void(guildIntegrationsUpdateEvent)>& userFunction) {
        this->guildIntegrationsUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildMemberAdd(const std::function<void(guildMemberAddEvent)>& userFunction) {
        this->guildMemberAddFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildMemberRemove(const std::function<void(guildMemberRemoveEvent)>& userFunction) {
        this->guildMemberRemoveFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildMemberUpdate(const std::function<void(guildMemberUpdateEvent)>& userFunction) {
        this->guildMemberUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildMembersChunk(const std::function<void(guildMembersChunkEvent)>& userFunction) {
        this->guildMembersChunkFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildRoleCreate(const std::function<void(guildRoleCreateEvent)>& userFunction) {
        this->guildRoleCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildRoleUpdate(const std::function<void(guildRoleUpdateEvent)>& userFunction) {
        this->guildRoleUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildRoleDelete(const std::function<void(guildRoleDeleteEvent)>& userFunction) {
        this->guildRoleDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildScheduledEventCreate(const std::function<void(guildScheduledEventCreateEvent)>& userFunction) {
        this->guildScheduledEventCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildScheduledEventUpdate(const std::function<void(guildScheduledEventUpdateEvent)>& userFunction) {
        this->guildScheduledEventUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildScheduledEventDelete(const std::function<void(guildScheduledEventDeleteEvent)>& userFunction) {
        this->guildScheduledEventDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildScheduledEventUserAdd(const std::function<void(guildScheduledEventUserAddEvent)>& userFunction) {
        this->guildScheduledEventUserAddFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::guildScheduledEventUserRemove(const std::function<void(guildScheduledEventUserRemoveEvent)>& userFunction) {
        this->guildScheduledEventUserRemoveFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::integrationCreate(const std::function<void(integrationCreateEvent)>& userFunction) {
        this->integrationCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::integrationUpdate(const std::function<void(integrationUpdateEvent)>& userFunction) {
        this->integrationUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::integrationDelete(const std::function<void(integrationDeleteEvent)>& userFunction) {
        this->integrationDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::inviteCreate(const std::function<void(inviteCreateEvent)>& userFunction) {
        this->inviteCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::inviteDelete(const std::function<void(inviteDeleteEvent)>& userFunction) {
        this->inviteDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageCreate(const std::function<void(messageCreateEvent)>& userFunction) {
        this->messageCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageUpdate(const std::function<void(messageUpdateEvent)>& userFunction) {
        this->messageUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageDelete(const std::function<void(messageDeleteEvent)>& userFunction) {
        this->messageDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageDeleteBulk(const std::function<void(messageDeleteBulkEvent)>& userFunction) {
        this->messageDeleteBulkFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageReactionAdd(const std::function<void(messageReactionAddEvent)>& userFunction) {
        this->messageReactionAddFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageReactionRemoveAll(const std::function<void(messageReactionRemoveAllEvent)>& userFunction) {
        this->messageReactionRemoveAllFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::messageReactionRemoveEmoji(const std::function<void(messageReactionRemoveEmojiEvent)>& userFunction) {
        this->messageReactionRemoveEmojiFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::presenceUpdate(const std::function<void(presenceUpdateEvent)>& userFunction) {
        this->presenceUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::stageInstanceCreate(const std::function<void(stageInstanceCreateEvent)>& userFunction) {
        this->stageInstanceCreateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::stageInstanceUpdate(const std::function<void(stageInstanceUpdateEvent)>& userFunction) {
        this->stageInstanceUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::stageInstanceDelete(const std::function<void(stageInstanceDeleteEvent)>& userFunction) {
        this->stageInstanceDeleteFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::typingStart(const std::function<void(typingStartEvent)>& userFunction) {
        this->typingStartFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::userUpdate(const std::function<void(userUpdateEvent)>& userFunction) {
        this->userUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::voiceStateUpdate(const std::function<void(voiceStateUpdateEvent)>& userFunction) {
        this->voiceStateUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::voiceServerUpdate(const std::function<void(voiceServerUpdateEvent)>& userFunction) {
        this->voiceServerUpdateFunction = userFunction;
    }

    [[maybe_unused]] void onEvent::webhooksUpdate(const std::function<void(webhooksUpdateEvent)>& userFunction) {
        this->webhooksUpdateFunction = userFunction;
    }
}