#include "discordClassses/voice.hpp"

helios::voiceState helios::voiceState::getVoiceStateData(const nlohmann::json& jsonData) {
    voiceState voiceStateData;
    if(jsonData.contains("guild_id")) voiceStateData.guildId = std::stol(jsonData["guild_id"].get<std::string>());
    if(!jsonData["channel_id"].is_null()) voiceStateData.channelId = std::stol(jsonData["channel_id"].get<std::string>());
    voiceStateData.userId = std::stol(jsonData["user_id"].get<std::string>());
    if(jsonData.contains("member")) voiceStateData.member = guildMember::getGuildMemberData(jsonData["member"]);
    voiceStateData.session_id = jsonData["session_id"];
    voiceStateData.deaf = jsonData["deaf"];
    voiceStateData.mute = jsonData["mute"];
    voiceStateData.selfDeaf = jsonData["self_deaf"];
    voiceStateData.selfMute = jsonData["self_mute"];
    if(jsonData.contains("self_stream")) voiceStateData.selfStream = jsonData["self_stream"];
    voiceStateData.selfVideo = jsonData["self_video"];
    voiceStateData.suppress = jsonData["suppress"];
    if(!jsonData["request_to_speak_timestamp"].is_null()) voiceStateData.requestToSpeakTimestamp = jsonData["request_to_speak_timestamp"];
    return voiceStateData;

}
