#ifndef HELIOS_EVENTS_H
#define HELIOS_EVENTS_H

namespace helios::event
{
    class eventData{

    };

    static void ready(eventData e);
    static void resumed(eventData e);
    static void reconnect(eventData e);

    static void applicationCommandPermissionsUpdate(eventData e);

    static void autoModerationRuleCreate(eventData e);
    static void autoModerationRuleUpdate(eventData e);
    static void autoModerationRuleDelete(eventData e);
    static void autoModerationActionExecution(eventData e);

    static void channelCreate(eventData e);
    static void channelUpdate(eventData e);
    static void channelDelete(eventData e);
    static void channelPinUpdate(eventData e);

    static void threadCreate(eventData e);
    static void threadUpdate(eventData e);
    static void threadDelete(eventData e);
    static void threadListSync(eventData e);
    static void threadMemberUpdate(eventData e);
    static void threadMembersUpdate(eventData e);

    static void guildCreate(eventData e);
    static void guildUpdate(eventData e);
    static void guildDelete(eventData e);
    static void guildBanAdd(eventData e);
    static void guildBanRemove(eventData e);
    static void guildEmojiUpdate(eventData e);
    static void guildStickersUpdate(eventData e);
    static void guildIntegrationsUpdate(eventData e);

    static void guildMemberAdd(eventData e);
    static void guildMemberRemove(eventData e);
    static void guildMemberUpdate(eventData e);
    static void guildMembersChunk(eventData e);

    static void guildRoleCreate(eventData e);
    static void guildRoleUpdate(eventData e);
    static void guildRoleDelete(eventData e);

    static void guildScheduledEventCreate(eventData e);
    static void guildScheduledEventUpdate(eventData e);
    static void guildScheduledEventDelete(eventData e);
    static void guildScheduledEventUserAdd(eventData e);
    static void guildScheduledEventUserRemove(eventData e);

    static void integrationCreate(eventData e);
    static void integrationUpdate(eventData e);
    static void integrationDelete(eventData e);

    static void inviteCreate(eventData e);
    static void inviteDelete(eventData e);

    static void messageCreate(eventData e);
    static void messageUpdate(eventData e);
    static void messageDelete(eventData e);
    static void messageDeleteBulk(eventData e);
    static void messageReactionAdd(eventData e);
    static void messageReactionRemoveAll(eventData e);
    static void messageReactionRemoveEmoji(eventData e);

    static void presenceUpdate(eventData e);

    static void stageInstanceCreate(eventData e);
    static void stageInstanceUpdate(eventData e);
    static void stageInstanceDelete(eventData e);

    static void typingStart(eventData e);
    static void userUpdate(eventData e);

    static void voiceStateUpdate(eventData e);
    static void voiceServerUpdate(eventData e);

    static void webhooksUpdate(eventData e);


}


#endif //HELIOS_EVENTS_H
