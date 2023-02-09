#ifndef HELIOS_INTERACTION_HPP
#define HELIOS_INTERACTION_HPP
#include <optional>
#include <string>
#include <nlohmann/json.hpp>

#include "../user.hpp"
#include "../guild/guildMember.hpp"

namespace helios {

    class interaction {

    };

    class messageInteraction {
    private:
        friend class message;
        static messageInteraction getMessageInteractionData(const nlohmann::json& jsonData);
    public:
        std::optional<long> id;
        std::optional<int> type;
        std::optional<std::string> name;
        user user;
        guildMember member;
    };

} // helios

#endif //HELIOS_INTERACTION_HPP
