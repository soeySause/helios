#include "discordClasses/channel/embed.hpp"

namespace helios {
    embedThumbnail embedThumbnail::getEmbedThumbnailData(const nlohmann::json &jsonData) {
        embedThumbnail embedThumbnailData;
        embedThumbnailData.url = jsonData["url"];
        if(jsonData.contains("proxy_url")) embedThumbnailData.proxyUrl = jsonData["proxy_url"];
        if(jsonData.contains("height")) embedThumbnailData.height = jsonData["height"];
        if(jsonData.contains("width")) embedThumbnailData.width = jsonData["width"];
        return embedThumbnailData;
    }

    embedThumbnail::embedThumbnail(const std::string& url, const std::string& proxyUrl, const int& height, const int& width) {
        this->url = url;
        this->proxyUrl = proxyUrl;
        this->height = height;
        this->width = width;
    }

    embedVideo embedVideo::getEmbedVideoData(const nlohmann::json &jsonData) {
        embedVideo embedVideoData;
        if(jsonData.contains("url")) embedVideoData.url = jsonData["url"];
        if(jsonData.contains("proxy_url")) embedVideoData.proxyUrl = jsonData["proxy_url"];
        if(jsonData.contains("height")) embedVideoData.height = jsonData["height"];
        if(jsonData.contains("width")) embedVideoData.width = jsonData["width"];
        return embedVideoData;
    }

    embedVideo::embedVideo(const std::string& url, const std::string& proxyUrl, const int& height, const int& width) {
        this->url = url;
        this->proxyUrl = proxyUrl;
        this->height = height;
        this->width = width;
    }

    embedImage embedImage::getEmbedImageData(const nlohmann::json &jsonData) {
        embedImage embedImageData;
        embedImageData.url = jsonData["url"];
        if(jsonData.contains("proxy_url")) embedImageData.proxyUrl = jsonData["proxy_url"];
        if(jsonData.contains("height")) embedImageData.height = jsonData["height"];
        if(jsonData.contains("width")) embedImageData.width = jsonData["width"];
        return embedImageData;
    }

    embedImage::embedImage(const std::string& url, const std::string& proxyUrl, const int& height, const int& width) {
        this->url = url;
        this->proxyUrl = proxyUrl;
        this->height = height;
        this->width = width;
    }

    embedProvider embedProvider::getEmbedProviderData(const nlohmann::json &jsonData) {
        embedProvider embedProviderData;
        if(jsonData.contains("url")) embedProviderData.url = jsonData["url"];
        if(jsonData.contains("name")) embedProviderData.name = jsonData["name"];
        return embedProviderData;
    }

    embedProvider::embedProvider(const std::string &name, const std::string &url) {
        this->url = url;
        this->name = name;
    }

    embedAuthor embedAuthor::getEmbedAuthorData(const nlohmann::json &jsonData) {
        embedAuthor embedAuthorData;
        embedAuthorData.name = jsonData["name"];
        if(jsonData.contains("url")) embedAuthorData.url = jsonData["url"];
        if(jsonData.contains("icon_url")) embedAuthorData.iconUrl = jsonData["icon_url"];
        if(jsonData.contains("proxy_icon_url")) embedAuthorData.proxyIconUrl = jsonData["proxy_icon_url"];
        return embedAuthorData;
    }

    embedAuthor::embedAuthor(const std::string &name, const std::string &url, const std::string& iconUrl, const std::string& proxyIconUrl) {
        this->name = name;
        this->url = url;
        this->iconUrl = iconUrl;
        this->proxyIconUrl = proxyIconUrl;
    }

    embedFooter embedFooter::getEmbedFooterData(const nlohmann::json &jsonData) {
        embedFooter embedFooterData;
        embedFooterData.text = jsonData["text"];
        if(jsonData.contains("icon_url")) embedFooterData.iconUrl = jsonData["icon_url"];
        if(jsonData.contains("proxy_icon_url")) embedFooterData.proxyIconUrl = jsonData["proxy_icon_url"];
        return embedFooterData;
    }

    embedFooter::embedFooter(const std::string &text, const std::string &iconUrl, const std::string &proxyIconUrl) {
        this->text = text;
        this->iconUrl = iconUrl;
        this->proxyIconUrl = proxyIconUrl;
    }

    embedField embedField::getEmbedFieldData(const nlohmann::json &jsonData) {
        embedField embedFieldData;
        embedFieldData.name = jsonData["name"];
        embedFieldData.value = jsonData["value"];
        if(jsonData.contains("inline")) embedFieldData.inline_ = jsonData["inline"];
        return embedFieldData;
    }

    embedField::embedField(const std::string &name, const std::string &value, const bool &displayInline) {
        this->name = name;
        this->value = value;
        this->inline_ = displayInline;
    }

    embed embed::getEmbedData(const nlohmann::json &jsonData) {
        embed embedData;
        if(jsonData.contains("title")) embedData.title = jsonData["title"];
        if(jsonData.contains("type")) embedData.type = jsonData["type"];
        if(jsonData.contains("description")) embedData.description = jsonData["description"];
        if(jsonData.contains("url")) embedData.url = jsonData["url"];
        if(jsonData.contains("timestamp")) embedData.timestamp = jsonData["timestamp"];
        if(jsonData.contains("color")) embedData.color = jsonData["color"];
        if(jsonData.contains("footer")) embedData.footer = embedFooter::getEmbedFooterData(jsonData["footer"]);
        if(jsonData.contains("image")) embedData.image = embedImage::getEmbedImageData(jsonData["image"]);
        if(jsonData.contains("thumbnail")) embedData.thumbnail = embedThumbnail::getEmbedThumbnailData(jsonData["thumbnail"]);
        if(jsonData.contains("video")) embedData.video = embedVideo::getEmbedVideoData(jsonData["video"]);
        if(jsonData.contains("provider")) embedData.provider = embedProvider::getEmbedProviderData(jsonData["provider"]);
        if(jsonData.contains("author")) embedData.author = embedAuthor::getEmbedAuthorData(jsonData["author"]);
        if(jsonData.contains("fields")) {
            for(const nlohmann::basic_json<>& field : jsonData["fields"]) {
                embedData.fields.emplace_back(embedField::getEmbedFieldData(field));
            }
        }
        return embedData;
    }

    embed::embed(const std::string &title, const std::string &description, const std::string &url,
                 const std::string &timestamp, const int &color) {
        this->title = title;
        this->description = description;
        this->url = url;
        this->timestamp = timestamp;
        this->color = color;
    }

} // helios