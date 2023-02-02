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

helios::applicationRoleConnectionMetadata
helios::applicationRoleConnectionMetadataOptions::getApplicationRoleConnectionMetadataRecords(const long &applicationId) {
    return helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData
    (nlohmann::json::parse(request::httpsRequest("discord.com", "/api/applications/" + std::to_string(applicationId) + "/role-connections/metadata", "", boost::beast::http::verb::get, token::botToken)));
}

helios::applicationRoleConnectionMetadata
helios::applicationRoleConnectionMetadataOptions::getApplicationRoleConnectionMetadataRecords(const std::string &applicationId) {
    return helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData
    (nlohmann::json::parse(request::httpsRequest("discord.com", "/api/applications/" + applicationId + "/role-connections/metadata", "", boost::beast::http::verb::get, token::botToken)));
}

helios::applicationRoleConnectionMetadata
helios::applicationRoleConnectionMetadataOptions::updateApplicationRoleConnectionMetadataRecords(const long &applicationId) {
    return helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData
    (nlohmann::json::parse(request::httpsRequest("discord.com", "/api/applications/" + std::to_string(applicationId) + "/role-connections/metadata", "", boost::beast::http::verb::put, token::botToken)));
}

helios::applicationRoleConnectionMetadata
helios::applicationRoleConnectionMetadataOptions::updateApplicationRoleConnectionMetadataRecords(const std::string &applicationId) {
    return helios::applicationRoleConnectionMetadata::getApplicationRoleConnectionMetadataRecordData
    (nlohmann::json::parse(request::httpsRequest("discord.com", "/api/applications/" + applicationId + "/role-connections/metadata", "", boost::beast::http::verb::put, token::botToken)));
}
