#ifndef HELIOS_AUDITLOG_HPP
#define HELIOS_AUDITLOG_HPP
#include <optional>
#include <vector>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include "request.hpp"
//#include "applicationCommands.hpp"
//#include "autoModerationRules.hpp"
//#include "guildScheduledEvent.hpp"
#include "user.hpp"
//#include "webhook.hpp"
//#include "channel.hpp"
//#include "guild.hpp"

namespace helios {

    class auditLogChange {
    private:
        static auditLogChange getAuditLogChangeData(const nlohmann::json& guildData);
        friend class auditLogEntry;
    public:
        std::optional<nlohmann::json> newValue;
        std::optional<nlohmann::json> oldValue;
        std::optional<std::string> key;
    };

    class optionalAuditEntryInfo {
    private:
        static optionalAuditEntryInfo getOptionalAuditEntryInfoData (const nlohmann::json& guildData);
        friend class auditLogEntry;
    public:
        std::optional<long> applicationId;
        std::optional<std::string> autoModerationRuleName;
        std::optional<std::string> autoModerationRuleTriggerType;
        std::optional<long> channelId;
        std::optional<std::string> count;
        std::optional<std::string> deleteMemberDays;
        std::optional<long> id;
        std::optional<std::string> membersRemoved;
        std::optional<long> messageId;
        std::optional<std::string> roleName;
        std::optional<std::string> type;
    };

    class auditLogEntry {
    private:
        friend class auditLog;
        static auditLogEntry getAuditLogEntryData(const nlohmann::json& guildData);
    public:
        std::optional<std::string> targetId;
        std::vector<auditLogChange> changes;
        std::optional<long> userId;
        std::optional<long> id;

        //https://discord.com/developers/docs/resources/audit-log#audit-log-entry-object-audit-log-events
        std::optional<int> actionType;
        optionalAuditEntryInfo options;
        std::optional<std::string> reason;
    };

    class auditLog {
    public:
        static auditLog getAuditLog(const nlohmann::json& guildData);
        //applicationCommands applicationCommand;
        std::unordered_map<long, auditLogEntry> auditLogEntries;
        //autoModerationRules autoModerationRules;
        //guildScheduledEvent guildScheduledEvent;
        //integrations integration;
        //channel threads;
        std::unordered_map<long, user> users;
        //webhook webhooks;
    };
}
#endif //HELIOS_AUDITLOG_HPP
