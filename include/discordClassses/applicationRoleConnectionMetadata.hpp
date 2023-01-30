#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include "request.hpp"
#include "../token.hpp"
#ifndef HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP
#define HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP

namespace helios {
    class applicationRoleConnectionMetadata {
    private:
        friend class applicationRoleConnectionMetadataOptions;
        static applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecordData(const nlohmann::json& jsonData);
    public:
        std::optional<int> type;
        std::optional<std::string> key;
        std::optional<std::string> name;
        std::optional<std::string> nameLocalizations;
        std::optional<std::string> description;
        std::optional<std::string> descriptionLocalizations;
    };

    class applicationRoleConnectionMetadataOptions {
    public:
        [[maybe_unused]] static applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecords(const long& applicationId);
        [[maybe_unused]] static applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecords(const std::string& applicationId);
        [[maybe_unused]] static applicationRoleConnectionMetadata updateApplicationRoleConnectionMetadataRecords(const long& applicationId);
        [[maybe_unused]] static applicationRoleConnectionMetadata updateApplicationRoleConnectionMetadataRecords(const std::string& applicationId);
    };
}

#endif //HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP
