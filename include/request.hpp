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
#include "ssl/root_certification.hpp"

class request
{
private:
    static int httpsResponseCode;
    static std::string httpsResponseReason;
 public:
    static std::string httpsRequest(const std::string& host, const std::string& target, const nlohmann::json& payload, const boost::beast::http::verb& method, const std::string& authorization = "", const std::string& reason = "");
    static std::string url_encode(const std::string &value);

    };
#endif //HELIOS_REQUEST_HPP
