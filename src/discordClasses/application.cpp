#include "discordClassses/application.hpp"

helios::installParams helios::installParams::getInstallParamsData(const nlohmann::json &jsonData) {
    installParams installParamsData;
    for(const nlohmann::basic_json<>& scope : jsonData["scopes"]) {
        installParamsData.scopes.emplace_back(scope);
    }
    installParamsData.permissions = jsonData["permissions"];
    return installParamsData;
}

helios::application helios::application::getApplicationData(const nlohmann::json &jsonData) {
    application applicationData;
    applicationData.id = std::stol(jsonData["id"].get<std::string>());
    if(jsonData.contains("name")) applicationData.name = jsonData["name"];
    if(jsonData.contains("icon") && !jsonData["icon"].is_null()) applicationData.icon = jsonData["icon"];
    if(jsonData.contains("description")) applicationData.description = jsonData["description"];
    if(jsonData.contains("rpc_origins")) {
        for(const nlohmann::basic_json<>& rpcOrigin : jsonData["rpc_origins"]) {
            applicationData.rpcOrigins.emplace_back(rpcOrigin);
        }
    }

    if(jsonData.contains("bot_public")) applicationData.botPublic = jsonData["bot_public"];
    if(jsonData.contains("bot_require_code_grant"))applicationData.botRequireCodeGrant = jsonData["bot_require_code_grant"];
    if(jsonData.contains("terms_of_service_url")) applicationData.termsOfServiceUrl = jsonData["terms_of_service_url"];
    if(jsonData.contains("privacy_policy_url")) applicationData.privacyPolicyUrl = jsonData["privacy_policy_url"];
    if(jsonData.contains("owner")) applicationData.owner = user::getUserData(jsonData["owner"]);
    if(jsonData.contains("verify_key")) applicationData.verifyKey = jsonData["verify_key"];
    if(jsonData.contains("team") && !jsonData["team"].is_null()) applicationData.team = team::getTeamData(jsonData["team"]);
    if(jsonData.contains("guild_id")) applicationData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
    if(jsonData.contains("primary_sku_id")) applicationData.primarySkuId = std::stol(jsonData["primary_sku_id"].get<std::string>());
    if(jsonData.contains("slug")) applicationData.slug = jsonData["slug"];
    if(jsonData.contains("cover_image")) applicationData.coverImage = jsonData["cover_image"];
    if(jsonData.contains("flags")) applicationData.flags = jsonData["flags"];
    if(jsonData.contains("tags")) {
        for(const nlohmann::basic_json<>& tag : jsonData["tags"]) {
            applicationData.rpcOrigins.emplace_back(tag);
        }
    }

    if(jsonData.contains("install_params")) applicationData.installParams = installParams::getInstallParamsData(jsonData["install_params"]);
    if(jsonData.contains("custom_install_url")) applicationData.customInstallUrl = jsonData["custom_install_url"];
    if(jsonData.contains("role_connections_verification_url")) applicationData.roleConnectionsVerificationUrl = jsonData["role_connections_verification_url"];
    return applicationData;
}
