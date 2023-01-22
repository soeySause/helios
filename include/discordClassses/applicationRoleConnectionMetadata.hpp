#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include "request.hpp"

#ifndef HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP
#define HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP

namespace helios {
    class applicationRoleConnectionMetadata {
    public:
        static applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecordData(const nlohmann::json& jsonData);
        std::optional<int> type;
        std::optional<std::string> key;
        std::optional<std::string> name;
        std::optional<std::string> nameLocalizations;
        std::optional<std::string> description;
        std::optional<std::string> descriptionLocalizations;
    };

    class applicationRoleConnectionMetadataOptions {
    private:
        friend class client;
        std::string token;
    public:
        [[maybe_unused]] applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecords(const long& applicationId);
        [[maybe_unused]] applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecords(const std::string& applicationId);
        [[maybe_unused]] applicationRoleConnectionMetadata updateApplicationRoleConnectionMetadataRecords(const long& applicationId);
        [[maybe_unused]] applicationRoleConnectionMetadata updateApplicationRoleConnectionMetadataRecords(const std::string& applicationId);
    };
}

#endif //HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP
