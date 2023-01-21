#ifndef HELIOS_APPLICATION_HPP
#define HELIOS_APPLICATION_HPP
#include <optional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "team.hpp"
#include "user.hpp"

namespace helios {
    class installParams {
    private:
        friend class application;
        static installParams getInstallParamsData(const nlohmann::json& jsonData);
    public:
        std::vector<std::string> scopes;
        std::optional<std::string> permissions;
    };

    class application {
    private:
        explicit application(const nlohmann::json& jsonData);
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<std::string> icon;
        std::optional<std::string> description;
        std::vector<std::string> rpcOrigins;
        std::optional<bool> botPublic;
        std::optional<bool> botRequireCodeGrant;
        std::optional<std::string> termsOfServiceUrl;
        std::optional<std::string> privacyPolicyUrl;
        user owner;
        std::optional<std::string> verifyKey;
        team team;
        std::optional<long> guildId;
        std::optional<long> primarySkuId;
        std::optional<std::string> slug;
        std::optional<std::string> coverImage;
        std::optional<int> flags;
        std::vector<std::string> tags;
        installParams installParams;
        std::optional<std::string> customInstallUrl;
        std::optional<std::string> roleConnectionsVerificationUrl;
    };
}

#endif //HELIOS_APPLICATION_HPP
