#include "discordClasses/auditLog.hpp"
 helios::auditLogChange helios::auditLogChange::getAuditLogChangeData(const nlohmann::json &guildData) {
    auditLogChange auditLogChangeData;
     if(guildData.contains("new_value")) auditLogChangeData.newValue = guildData["new_value"];
     if(guildData.contains("old_value")) auditLogChangeData.oldValue = guildData["old_value"];
     if(guildData.contains("key")) auditLogChangeData.key = guildData["key"];
     return auditLogChangeData;
 }

helios::optionalAuditEntryInfo
helios::optionalAuditEntryInfo::getOptionalAuditEntryInfoData(const nlohmann::json &guildData) {
    optionalAuditEntryInfo optionalAuditEntryInfoData;
    if(guildData.contains("application_id")) optionalAuditEntryInfoData.applicationId = std::stol(guildData["application_id"].get<std::string>());
    if(guildData.contains("auto_moderation_rule_name")) optionalAuditEntryInfoData.autoModerationRuleName = guildData["auto_moderation_rule_name"];
    if(guildData.contains("auto_moderation_rule_trigger_type")) optionalAuditEntryInfoData.autoModerationRuleTriggerType = guildData["auto_moderation_rule_trigger_type"];
    if(guildData.contains("channel_id")) optionalAuditEntryInfoData.channelId = std::stol(guildData["channel_id"].get<std::string>());
    if(guildData.contains("count")) optionalAuditEntryInfoData.count = guildData["count"];
    if(guildData.contains("delete_member_days")) optionalAuditEntryInfoData.deleteMemberDays = guildData["delete_member_days"];
    if(guildData.contains("id")) optionalAuditEntryInfoData.id = std::stol(guildData["id"].get<std::string>());
    if(guildData.contains("members_removed")) optionalAuditEntryInfoData.membersRemoved = guildData["members_removed"];
    if(guildData.contains("message_id")) optionalAuditEntryInfoData.messageId = std::stol(guildData["message_id"].get<std::string>());
    if(guildData.contains("role_name")) optionalAuditEntryInfoData.roleName = guildData["role_name"];
    if(guildData.contains("type")) optionalAuditEntryInfoData.type = guildData["type"];

    return optionalAuditEntryInfoData;
}

helios::auditLogEntry helios::auditLogEntry::getAuditLogEntryData(const nlohmann::json &guildData) {
    auditLogEntry auditLogEntryData;
     if(guildData.contains("target_id") && !guildData["target_id"].is_null()) auditLogEntryData.targetId = guildData["target_id"];
     if(guildData.contains("changes"))  {
         for(const nlohmann::basic_json<>& change : guildData["changes"]) {
             auditLogEntryData.changes.emplace_back(auditLogChange::getAuditLogChangeData(guildData["changes"]));
         }
     }

     if(guildData.contains("user_id") && !guildData["user_id"].is_null()) auditLogEntryData.userId = std::stol(guildData["user_id"].get<std::string>());
     if(guildData.contains("id")) auditLogEntryData.id = std::stol(guildData["id"].get<std::string>());
     if(guildData.contains("action_type")) auditLogEntryData.actionType = guildData["action_type"];
     if(guildData.contains("options")) auditLogEntryData.options = optionalAuditEntryInfo::getOptionalAuditEntryInfoData(guildData["options"]);
     if(guildData.contains("reason")) auditLogEntryData.reason = guildData["reason"];
    return auditLogEntryData;
 }

helios::auditLog helios::auditLog::getAuditLog(const nlohmann::json &guildData) {
    auditLog auditLogData;
    //auditLogData.applicationCommand;
    for (const nlohmann::basic_json<>& entry: guildData["audit_log_entries"]) {
        auditLogData.auditLogEntries[std::stol(entry["id"].get<std::string>())] = auditLogEntry::getAuditLogEntryData(entry);
    }

    //auditLogData.autoModerationRules;
    //auditLogData.guildScheduledEvent;
    //auditLogData.integration;
    //auditLogData.threads;
    for (const nlohmann::basic_json<>& user: guildData["users"]) {
        auditLogData.users[std::stol(user["id"].get<std::string>())] = user::getUserData(user);
    }

    //auditLogData.webhooks;
    return auditLogData;
}
/*
helios::auditLog
helios::auditLogOptions::getGuildAuditLog(const long &guildId, const long &userId, const int &actionType,
                                          const long &before, const long &after, const int &limit) {
    nlohmann::json queryParams;
    if(userId != -1) queryParams["user_id"] = userId;
    if(actionType != -1) queryParams["action_type"] = actionType;
    if(before != -1) queryParams["before"] = before;
    if(after != -1) queryParams["after"] = after;
    if(limit != -1) queryParams["limit"] = limit;
    std::cout << request::httpsRequest("discord.com", "/api/guilds/" + std::to_string(guildId) + "/audit-logs", queryParams.dump(), boost::beast::http::verb::get, this->token);
    return helios::auditLog::getAuditLog(nlohmann::json::parse(request::httpsRequest("discord.com", "/api/guilds/" + std::to_string(guildId) + "/audit-logs", queryParams.dump(), boost::beast::http::verb::get, this->token)));

}
*/