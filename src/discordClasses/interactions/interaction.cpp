#include "discordClassses/interactions/interaction.hpp"

namespace helios {
    messageInteraction messageInteraction::getMessageInteractionData(const nlohmann::json &jsonData) {
        messageInteraction messageInteractionData;
        messageInteractionData.id = std::stol(jsonData["id"].get<std::string>());
        messageInteractionData.type = jsonData["type"];
        messageInteractionData.name = jsonData["name"];
        messageInteractionData.user = user::getUserData(jsonData["user"]);
        //if(jsonData.contains("member")) messageInteractionData.member = guildMember::getGuildMemberData(jsonData["member"]);
        return messageInteractionData;
    }
} // helios