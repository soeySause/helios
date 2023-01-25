#include <optional>
#include <string>
#include <nlohmann/json.hpp>

#ifndef HELIOS_STICKER_HPP
#define HELIOS_STICKER_HPP

namespace helios {

    class sticker {

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
