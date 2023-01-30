#ifndef HELIOS_EMOJI_HPP
#define HELIOS_EMOJI_HPP
#include <string>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

#include "user.hpp"

namespace helios {
    class emoji {
    private:
        friend class guild;
        static emoji getEmojiData(const nlohmann::json& jsonData);
    public:
        std::optional<long> id;
        std::optional<std::string> name;
        std::vector<long> roles;
        user user;
        std::optional<bool> require_colons;
        std::optional<bool> managed;
        std::optional<bool> animated;
        std::optional<bool> available;

    };
}

#endif //HELIOS_EMOJI_HPP
