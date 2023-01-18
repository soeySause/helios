#ifndef HELIOS_REQUEST_HPP
#define HELIOS_REQUEST_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "heliosException.hpp"
#include "cache.hpp"
#include "ssl/root_certification.hpp"

 class request
{
private:
    static int httpsResponseCode;
    static std::string httpsResponseReason;
 public:
    [[nodiscard]] static std::string getRequest(const std::string& host, const std::string& target, const std::string& authorization = "");
    static int postRequest(const std::string& host, const std::string& target, const std::string& payload, const std::string& authorization = "");
    static int patchRequest(const std::string& host, const std::string& target, const std::string& payload, const std::string& authorization = "");
    static int deleteRequest(const std::string &host, const std::string &target, const std::string &payload, const std::string& authorization = "");
};

#endif //HELIOS_REQUEST_HPP
