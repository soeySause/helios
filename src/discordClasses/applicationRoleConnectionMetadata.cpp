#include "discordClassses/applicationRoleConnectionMetadata.hpp"

helios::applicationRoleConnectionMetadata
helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData(
        const nlohmann::json &jsonData) {
    applicationRoleConnectionMetadata applicationRoleConnectionMetadataData;
    if(jsonData.contains("type")) applicationRoleConnectionMetadataData.type = jsonData["type"];
    if(jsonData.contains("key")) applicationRoleConnectionMetadataData.key = jsonData["key"];
    if(jsonData.contains("name")) applicationRoleConnectionMetadataData.name = jsonData["name"];
    if(jsonData.contains("name_localizations")) applicationRoleConnectionMetadataData.nameLocalizations = jsonData["name_localizations"];
    if(jsonData.contains("description")) applicationRoleConnectionMetadataData.description = jsonData["description"];
    if(jsonData.contains("description_localizations")) applicationRoleConnectionMetadataData.descriptionLocalizations = jsonData["description_localizations"];
    return applicationRoleConnectionMetadataData;
}