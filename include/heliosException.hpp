#ifndef HELIOS_HELIOSEXCEPTION_HPP
#define HELIOS_HELIOSEXCEPTION_HPP
#include <iostream>
#include <string>
#include <utility>

namespace helios {
    class heliosException  : public std::exception {
    private:
        int errorCode;
        std::string errorReason;
    public:
        heliosException(const int &errorCode, std::string reason) : errorCode(errorCode), errorReason(std::move(reason)) {};

        [[nodiscard]] int error_code() const { return errorCode; }
        [[nodiscard]] const char *what() const noexcept override { return errorReason.c_str(); }
    };
}

#endif //HELIOS_HELIOSEXCEPTION_HPP
