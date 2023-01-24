#ifndef HELIOS_ATTACHMENT_HPP
#define HELIOS_ATTACHMENT_HPP
#include <optional>
#include <string>

namespace helios {
    class attachment {
        std::optional<long> id;
        std::optional<std::string> fileName;
        std::optional<std::string> description;
        std::optional<std::string> contentType;
        std::optional<int> size;
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int> width;
        std::optional<bool> ephemeral;
    };
} // helios

#endif //HELIOS_ATTACHMENT_HPP
