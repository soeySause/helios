#ifndef HELIOS_STAGEINSTANCE_HPP
#define HELIOS_STAGEINSTANCE_HPP
#include <optional>
#include <string>

namespace helios {
    class stageInstance {
        std::optional<long> id;
        std::optional<long> guildId;
        std::optional<long> channelId;
        std::optional<std::string> topic;
        std::optional<int> privacyLevel;
        std::optional<bool> discoveryDisabled;
        std::optional<long> guildScheduledEventId;
    };

} // helios

#endif //HELIOS_STAGEINSTANCE_HPP
