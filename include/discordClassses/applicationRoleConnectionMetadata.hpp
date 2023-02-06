#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include "request.hpp"
#ifndef HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP
#define HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP \

namespace helios {
    class applicationRoleConnectionMetadata {
    private:
        friend class client;
        static applicationRoleConnectionMetadata getApplicationRoleConnectionMetadataRecordData(const nlohmann::json& jsonData);
    public:
        std::optional<int> type;
        std::optional<std::string> key;
        std::optional<std::string> name;
        std::optional<std::string> nameLocalizations;
        std::optional<std::string> description;
        std::optional<std::string> descriptionLocalizations;
    };
}

#endif //HELIOS_APPLICATIONROLECONNECTIONMETADATA_HPP
