#ifndef HELIOS_EMBED_HPP
#define HELIOS_EMBED_HPP
#include <optional>
#include <string>
#include <vector>

namespace helios {
    class embedThumbnail {
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int> width;
    };

    class embedVideo{
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int> width;
    };

    class embedImage{
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int>width;
    };

    class embedProvider {
        std::optional<std::string> name;
        std::optional<std::string> url;
    };

    class embedAuthor {
        std::optional<std::string> name;
        std::optional<std::string> url;
        std::optional<std::string> iconUrl;
        std::optional<std::string> proxyIconUrl;
    };

    class embedFooter {
        std::optional<std::string> text;
        std::optional<std::string> iconUrl;
        std::optional<std::string> proxyIconUrl;
    };

    class embedField {
        std::optional<std::string> name;
        std::optional<std::string>value;
        std::optional<bool> displayInline;
    };

    class embed {
        std::optional<std::string> title;
        std::optional<std::string> type;
        std::optional<std::string> description;
        std::optional<std::string> url;
        std::optional<std::string> timestamp;
        std::optional<int> color;
        embedFooter footer;
        embedImage image;
        embedThumbnail thumbnail;
        embedVideo video;
        embedProvider provider;
        embedAuthor author;
        std::vector<embedField> fields;
    };
} // helios

#endif //HELIOS_EMBED_HPP
