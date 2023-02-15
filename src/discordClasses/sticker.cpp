#include "discordClasses/sticker.hpp"
#include <iostream>

namespace helios {
    messageStickerItem messageStickerItem::getMessageStickerItemData(const nlohmann::json &jsonData) {
        messageStickerItem messageStickerItemData;
        messageStickerItemData.id = std::stol(jsonData["id"].get<std::string>());
        messageStickerItemData.name = jsonData["name"];
        messageStickerItemData.formatType = jsonData["format_type"];
        return messageStickerItemData;
    }

    sticker sticker::getStickerData(const nlohmann::json &jsonData) {
        sticker stickerData;
        stickerData.id = std::stol(jsonData["id"].get<std::string>());
        if(jsonData.contains("pack_id")) { stickerData.packId = std::stol(jsonData["pack_id"].get<std::string>()); }
        stickerData.name = jsonData["name"];
        if(!jsonData["description"].is_null()) stickerData.description = jsonData["description"];
        stickerData.tags = jsonData["tags"];
        if(jsonData.contains("asset")) { stickerData.asset = jsonData["asset"]; }
        stickerData.type = jsonData["type"];
        stickerData.formatType = jsonData["format_type"];
        if(jsonData.contains("available")) { stickerData.available = jsonData["available"]; }
        if(jsonData.contains("guild_id")) { stickerData.guildId = std::stol(jsonData["guild_id"].get<std::string>()); }
        if(jsonData.contains("user")) { stickerData.user = user::getUserData("user"); }
        if(jsonData.contains("sort_value")) { stickerData.sortValue = jsonData["sort_value"]; }
        return stickerData;

    }
} // helios