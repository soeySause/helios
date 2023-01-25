#include "discordClassses/sticker.hpp"

namespace helios {

    messageStickerItem messageStickerItem::getMessageStickerItemData(const nlohmann::json &jsonData) {
        messageStickerItem messageStickerItemData;
        messageStickerItemData.id = std::stol(jsonData["id"].get<std::string>());
        messageStickerItemData.name = jsonData["name"];
        messageStickerItemData.formatType = jsonData["format_type"];
        return messageStickerItemData;
    }

} // helios