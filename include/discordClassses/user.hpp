#ifndef HELIOS_USER_HPP
#define HELIOS_USER_HPP
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
namespace helios {
    class user {
    private:
        friend class teamMember;
        friend class application;
        static user getUserData(const nlohmann::json& jsonData);
    public:
        bool operator==(const user& userToCompare) const;
        explicit operator bool() const;
        std::optional<long> id;
        std::optional<std::string> username;
        std::optional<std::string> discriminator;
        std::optional<std::string> avatar;
        std::optional<bool> bot;
        std::optional<bool> system;
        std::optional<bool> mfaEnabled;
        std::optional<std::string> banner;
        std::optional<int> accentColor;
        std::optional<std::string> locale;
        std::optional<bool> verified;
        std::optional<std::string> email;
        std::optional<int> flags;
        std::optional<int> premiumType;
        std::optional<int> publicFlags;
    };
}

#endif //HELIOS_USER_HPP
