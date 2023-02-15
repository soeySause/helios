#ifndef HELIOS_EMBED_HPP
#define HELIOS_EMBED_HPP
#include <optional>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace helios {
    class embedThumbnail {
    private:
        friend class embed;
        static embedThumbnail getEmbedThumbnailData(const nlohmann::json& jsonData);
    public:
        explicit embedThumbnail(const std::string& url, const std::string& proxyUrl = "", const int& height = -1, const int& width = -1);
        embedThumbnail() = default;
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int> width;
    };

    class embedVideo{
    private:
        friend class embed;
        static embedVideo getEmbedVideoData(const nlohmann::json& jsonData);
    public:
        explicit embedVideo(const std::string& url, const std::string& proxyUrl = "", const int& height = -1, const int& width = -1);
        embedVideo() = default;
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int> width;
    };

    class embedImage{
    private:
        friend class embed;
        static embedImage getEmbedImageData(const nlohmann::json& jsonData);
    public:
        explicit embedImage(const std::string& url, const std::string& proxyUrl = "", const int& height = -1, const int& width = -1);
        embedImage() = default;
        std::optional<std::string> url;
        std::optional<std::string> proxyUrl;
        std::optional<int> height;
        std::optional<int>width;
    };

    class embedProvider {
    private:
        friend class embed;
        static embedProvider getEmbedProviderData(const nlohmann::json& jsonData);
    public:
        explicit embedProvider(const std::string& name = "", const std::string& url = "");
        std::optional<std::string> name;
        std::optional<std::string> url;
    };

    class embedAuthor {
    private:
        friend class embed;
        static embedAuthor getEmbedAuthorData(const nlohmann::json& jsonData);
    public:
        explicit embedAuthor(const std::string& name, const std::string& url = "", const std::string& iconUrl = "", const std::string& proxyIconUrl = "");
        embedAuthor() = default;
        std::optional<std::string> name;
        std::optional<std::string> url;
        std::optional<std::string> iconUrl;
        std::optional<std::string> proxyIconUrl;
    };

    class embedFooter {
    private:
        friend class embed;
        static embedFooter getEmbedFooterData(const nlohmann::json& jsonData);
    public:
        explicit embedFooter(const std::string& text, const std::string& iconUrl = "", const std::string& proxyIconUrl = "");
        embedFooter() = default;
        std::optional<std::string> text;
        std::optional<std::string> iconUrl;
        std::optional<std::string> proxyIconUrl;
    };

    class embedField {
    private:
        friend class embed;
        static embedField getEmbedFieldData(const nlohmann::json& jsonData);
    public:
        explicit embedField(const std::string& name, const std::string& value, const bool& displayInline = false);
        embedField() = default;
        std::optional<std::string> name;
        std::optional<std::string>value;
        std::optional<bool> inline_;
    };

    class embed {
    private:
        friend class message;
        static embed getEmbedData(const nlohmann::json& jsonData);
    public:
        explicit embed(const std::string& title = "", const std::string& description = "", const std::string& url= "", const std::string& timestamp = "", const int& color = 0);
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
