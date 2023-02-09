#ifndef HELIOS_APPLICATIONCOMMANDS_HPP
#define HELIOS_APPLICATIONCOMMANDS_HPP
#include <optional>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace helios {

    class guildApplicationCommandPermissions {

    };

    class applicationCommandPermissions {

    };

    class applicationCommandOptionChoice {
    private:
        friend class applicationCommandOption;
        static applicationCommandOptionChoice getApplicationCommandOptionChoiceData(const json& jsonData);
    public:
        std::optional<std::string> name;
        std::map<std::string, std::string> nameLocalizations;
        std::optional<std::string> value;
    };

    class applicationCommandOption {
    private:
        friend class applicationCommand;
        static applicationCommandOption getApplicationCommandOptionData(const json& jsonData);
    public:
        std::optional<int> type;
        std::optional<std::string> name;
        std::map<std::string, std::string> nameLocalizations;
        std::optional<std::string> description;
        std::map<std::string, std::string> descriptionLocalizations;
        std::optional<bool> required;
        std::vector<applicationCommandOptionChoice> choices;
        std::unique_ptr<applicationCommandOption> options;
        std::vector<int> channelTypes;
        std::optional<std::string> minValue;
        std::optional<std::string> maxValue;
        std::optional<int> minLength;
        std::optional<int> maxLength;
        std::optional<bool> autocomplete;

    };
    class applicationCommand {
    private:
        applicationCommand getApplicationCommandData(const json& jsonData);
    public:
	    std::optional<long> id;
        std::optional<int> type;
        std::optional<long> applicationId;
        std::optional<long> guildId;
        std::optional<std::string> name;
        std::map<std::string, std::string> nameLocalizations;
        std::optional<std::string> description;
        std::map<std::string, std::string> descriptionLocalizations;
        std::vector<applicationCommandOption> options;
        std::optional<std::string> defaultMemberPermissions;
        std::optional<bool> dmPermissions;
        std::optional<bool> nsfw;
        std::optional<long> version;
    };

} // helios

#endif //HELIOS_APPLICATIONCOMMANDS_HPP
