#include "discordClassses/user.hpp"

helios::user helios::user::getUserData(const nlohmann::json& jsonData) {
    user userData;
    if(jsonData.contains("id")) {userData.id = std::stol(jsonData["id"].get<std::string>()); }
    if(jsonData.contains("username")) {userData.username = jsonData["username"]; }
    if(jsonData.contains("discriminator")) {userData.discriminator = jsonData["discriminator"]; }
    if(jsonData.contains("avatar") && !jsonData["avatar"].is_null()) {userData.avatar = jsonData["avatar"]; }
    if(jsonData.contains("bot")) {userData.bot = jsonData["bot"]; }
    if(jsonData.contains("system")) {userData.system = jsonData["system"]; }
    if(jsonData.contains("mfa_enabled")) {userData.mfaEnabled = jsonData["mfa_enabled"]; }
    if(jsonData.contains("banner") && !jsonData["banner"].is_null()) {userData.banner = jsonData["banner"]; }
    if(jsonData.contains("accent_color") && !jsonData["accent_color"].is_null()) {userData.accentColor = jsonData["accent_color"]; }
    if(jsonData.contains("locale")) {userData.locale = jsonData["locale"]; }
    if(jsonData.contains("verified")) {userData.verified = jsonData["verified"]; }
    if(jsonData.contains("email") && !jsonData["email"].is_null()) {userData.email = jsonData["email"]; }
    if(jsonData.contains("flags")) {userData.flags = jsonData["flags"]; }
    if(jsonData.contains("premium_type")) {userData.premiumType = jsonData["premium_type"]; }
    if(jsonData.contains("avatar") && !jsonData["avatar"].is_null()) {userData.avatar = jsonData["avatar"]; }
    if(jsonData.contains("public_flags")) {userData.publicFlags = jsonData["public_flags"]; }
    return userData;
}

bool helios::user::operator==(const user &userToCompare) const {
    return this->id == userToCompare.id;
}

helios::user::operator bool() const {
    return this->id.has_value();
}
