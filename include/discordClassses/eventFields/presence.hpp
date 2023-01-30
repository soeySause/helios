#include <optional>
#include <string>
#include "../user.hpp"
#ifndef HELIOS_PRESENCE_HPP
#define HELIOS_PRESENCE_HPP
namespace helios {
    class presenceUpdate {
        user user;
        std::optional<long> guildId;
        std::optional<std::string> status;
    };
}

#endif //HELIOS_PRESENCE_HPP
