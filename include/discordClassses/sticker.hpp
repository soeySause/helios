#include <optional>
#include <string>
#include <nlohmann/json.hpp>

#include "user.hpp"
#ifndef HELIOS_STICKER_HPP
#define HELIOS_STICKER_HPP

namespace helios {

    class sticker {
    private:
        friend class guild;
        static sticker getStickerData(const nlohmann::json& jsonData);
    public:
        std::optional<long> id;
        std::optional<long> packId;
        std::optional<std::string> name;
        std::optional<std::string> description;
        std::optional<std::string> tags;
        std::optional<std::string> asset;
        std::optional<int> type;
        std::optional<int> formatType;
        std::optional<bool> available;
        std::optional<long> guildId;
        user user;
        std::optional<int> sortValue;

    };

    class messageStickerItem {
    private:
        friend class message;
        static messageStickerItem getMessageStickerItemData(const nlohmann::json& jsonData);
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::optional<int> formatType;
    };


} // helios

#endif //HELIOS_STICKER_HPP
