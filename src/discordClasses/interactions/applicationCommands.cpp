#include "discordClasses/interactions/applicationCommands.hpp"

namespace helios {
    applicationCommandOptionChoice
    applicationCommandOptionChoice::getApplicationCommandOptionChoiceData(const json &jsonData) {
        applicationCommandOptionChoice applicationCommandOptionChoiceData;
        applicationCommandOptionChoiceData.name = jsonData["name"];
        if(jsonData.contains("name_localizations") && !jsonData.is_null()) {
            for (auto it = jsonData["name_localizations"].begin(); it != jsonData["name_localizations"].end(); ++it) {
                applicationCommandOptionChoiceData.nameLocalizations[it.key()] = it.value();
            }
        }
        applicationCommandOptionChoiceData.value = jsonData["value"].dump();
        return applicationCommandOptionChoiceData;
    }

    applicationCommandOption applicationCommandOption::getApplicationCommandOptionData(const json& jsonData) {
        applicationCommandOption applicationCommandOptionData;

        return applicationCommandOptionData;
    }

    applicationCommand applicationCommand::getApplicationCommandData(const json& jsonData) {
        applicationCommand applicationCommandData;
        applicationCommandData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("type")) applicationCommandData.type = jsonData["type"];
        if(jsonData.contains("application_id")) applicationCommandData.applicationId = std::stol(jsonData["application_id"].get<std::string>());
        if(jsonData.contains("guild_id")) applicationCommandData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
        applicationCommandData.name = jsonData["name"];
        if(jsonData.contains("name_localizations") && !jsonData.is_null()) {
            for (auto it = jsonData["name_localizations"].begin(); it != jsonData["name_localizations"].end(); ++it) {
                applicationCommandData.nameLocalizations[it.key()] = it.value();
            }
        }

        applicationCommandData.description = jsonData[description];
        if(jsonData.contains("description_localizations") && !jsonData.is_null()) {
            for (auto it = jsonData["description_localizations"].begin(); it != jsonData["description_localizations"].end(); ++it) {
                applicationCommandData.descriptionLocalizations[it.key()] = it.value();
            }
        }

        if(jsonData.contains("options")) {
            for(const auto& option : jsonData["options"]) {
                applicationCommandData.options.emplace_back(applicationCommandOption::getApplicationCommandOptionData(option));
            }
        }
        if(!jsonData["default_member_permissions"].is_null()) applicationCommandData.defaultMemberPermissions = jsonData["default_member_permissions"];
        if(jsonData.contains("dm_permissions"))  applicationCommandData.dmPermissions = jsonData["dm_permissions"];
        if(jsonData.contains("nsfw"))  applicationCommandData.nsfw = jsonData["nsfw"];
        applicationCommandData.version = std::stol(jsonData["version"].get<std::string>());

        return applicationCommandData;
    }
} // helios