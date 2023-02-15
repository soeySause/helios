#ifndef HELIOS_ATTACHMENT_HPP
#define HELIOS_ATTACHMENT_HPP
#include <optional>
#include <string>
#include <nlohmann/json.hpp>
namespace helios {
    class attachment {
        friend class message;
        static attachment getAttachmentData(const nlohmann::json& jsonData);
    public:
        attachment() = default;
        std::optional<int> id;
        std::optional<std::string> filename;
        std::optional<std::string> description;
        std::optional<std::string> contentType;
        std::optional<int> size;
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int> width;
        std::optional<bool> ephemeral;
        std::optional<std::string> pathToFile;
    };
} // helios

#endif //HELIOS_ATTACHMENT_HPP
