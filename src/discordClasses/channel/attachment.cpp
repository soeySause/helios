#include "discordClasses/channel/attachment.hpp"

helios::attachment helios::attachment::getAttachmentData(const nlohmann::json &jsonData) {
    helios::attachment attachmentData;
    attachmentData.id = std::stol(jsonData["id"].get<std::string>());
    attachmentData.filename = jsonData["filename"];
    if(jsonData.contains("description")) attachmentData.description = jsonData["description"];
    if(jsonData.contains("content_type")) attachmentData.contentType = jsonData["content_type"];
    attachmentData.size = jsonData["size"];
    attachmentData.url = jsonData["url"];
    attachmentData.proxyUrl = jsonData["proxy_url"];
    if(jsonData.contains("height") && !jsonData["height"].is_null()) attachmentData.height = jsonData["height"];
    if(jsonData.contains("width") && !jsonData["height"].is_null()) attachmentData.width = jsonData["width"];
    if(jsonData.contains("ephemeral")) attachmentData.ephemeral = jsonData["ephemeral"];
    return attachmentData;
}
