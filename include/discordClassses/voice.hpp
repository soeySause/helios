#ifndef HELIOS_VOICE_HPP
#define HELIOS_VOICE_HPP
#include <optional>
#include <string>
#include <nlohmann/json.hpp>

#include "guild/guildMember.hpp"

namespace helios {
    class voiceState {
    private:
        friend class eventData;
        static voiceState getVoiceStateData(const nlohmann::json& jsonData);
    public:
        std::optional<long> guildId;
        std::optional<long> channelId;
        std::optional<long> userId;
        guildMember member;
        std::optional<std::string> session_id;
        std::optional<bool> deaf;
        std::optional<bool> mute;
        std::optional<bool> selfDeaf;
        std::optional<bool> selfMute;
        std::optional<bool> selfStream;
        std::optional<bool> selfVideo;
        std::optional<bool> suppress;
        std::optional<std::string> requestToSpeakTimestamp;
    };

    class voiceRegion {
        std::optional<std::string> id;
        std::optional<std::string> name;
        std::optional<bool> optimal;
        std::optional<bool> deprecated;
        std::optional<bool> custom;
    };

}

#endif //HELIOS_VOICE_HPP
