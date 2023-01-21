#include "discordClassses/application.hpp"

helios::installParams helios::installParams::getInstallParamsData(const nlohmann::json &jsonData) {
    installParams installParamsData;
    if(jsonData.contains("scopes")) {
        for(const nlohmann::basic_json<>& scope : jsonData["scopes"]) {
            installParamsData.scopes.emplace_back(scope);
        }
    }
    if(jsonData.contains("permissions")) installParamsData.permissions = jsonData["permissions"];
    return installParamsData;
}

helios::application::application(const nlohmann::json &jsonData) {
    if(jsonData.contains("id")) this->id = std::stol(jsonData["id"].get<std::string>());
    if(jsonData.contains("name")) this->name = jsonData["name"];
    if(jsonData.contains("icon") && !jsonData["icon"].is_null()) this->icon = jsonData["icon"];
    if(jsonData.contains("description")) this->description = jsonData["description"];
    if(jsonData.contains("rpc_origins")) {
        for(const nlohmann::basic_json<>& rpcOrigin : jsonData["rpc_origins"]) {
            this->rpcOrigins.emplace_back(rpcOrigin);
        }
    }

    if(jsonData.contains("bot_public")) this->botPublic = jsonData["bot_public"];
    if(jsonData.contains("bot_require_code_grant")) this->botRequireCodeGrant = jsonData["bot_require_code_grant"];
    if(jsonData.contains("terms_of_service_url")) this->termsOfServiceUrl = jsonData["terms_of_service_url"];
    if(jsonData.contains("privacy_policy_url")) this->privacyPolicyUrl = jsonData["privacy_policy_url"];
    if(jsonData.contains("owner")) this->owner = user::getUserData(jsonData["owner"]);
    if(jsonData.contains("verify_key")) this->verifyKey = jsonData["verify_key"];
    if(jsonData.contains("team") && !jsonData["team"].is_null()) this->team = team::getTeamData(jsonData["team"]);
    if(jsonData.contains("guild_id")) this->guildId = std::stol(jsonData["guild_id"].get<std::string>());
    if(jsonData.contains("primary_sku_id")) this->primarySkuId = std::stol(jsonData["primary_sku_id"].get<std::string>());
    if(jsonData.contains("slug")) this->slug = jsonData["slug"];
    if(jsonData.contains("cover_image")) this->coverImage = jsonData["cover_image"];
    if(jsonData.contains("flags")) this->flags = jsonData["flags"];
    if(jsonData.contains("tags")) {
        for(const nlohmann::basic_json<>& tag : jsonData["tags"]) {
            this->rpcOrigins.emplace_back(tag);
        }
    }

    if(jsonData.contains("install_params")) this->installParams = installParams::getInstallParamsData(jsonData["install_params"]);
    if(jsonData.contains("custom_install_url")) this->customInstallUrl = jsonData["custom_install_url"];
    if(jsonData.contains("role_connections_verification_url")) this->roleConnectionsVerificationUrl = jsonData["role_connections_verification_url"];

}
