#include <optional>
#include <string>
#include "../emoji.hpp"
#ifndef HELIOS_ACTIVITY_HPP
#define HELIOS_ACTIVITY_HPP
namespace helios {
    class activityTimestamps {

    };

    class activityParty {

    };

    class activityAssets {

    };

    class activitySecrets {

    };

    class activityButtons {

    };

    class activity {
        std::optional<std::string> name;
        std::optional<int> type;
        std::optional<std::string> url;
        std::optional<int> createdAt;
        activityTimestamps timestamps;
        std::optional<long> applicationId;
        std::optional<std::string> details;
        std::optional<std::string> state;
        emoji emoji;
        activityParty party;
        activityAssets assets;
        activitySecrets secrets;
        std::optional<bool> instance;
        std::optional<int> flags;
        activityButtons buttons;
    };
}


#endif //HELIOS_ACTIVITY_HPP
