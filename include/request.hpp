#ifndef HELIOS_REQUEST_HPP
#define HELIOS_REQUEST_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <random>
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

#include "rateLimit.hpp"
#include "heliosException.hpp"
#include "discordClasses/channel/attachment.hpp"
#include "ssl/root_certification.hpp"

class request {
private:
    static int httpsResponseCode;
    static std::string httpsResponseReason;
 public:
    static std::string httpsRequest(const std::string& host, const std::string& target, const nlohmann::json& payload, const boost::beast::http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization = "", const std::string& reason = "");
    static std::string attachmentHttpsRequest(const std::string& host, const std::string& target, const nlohmann::json& jsonPayload, const std::vector<helios::attachment>& attachments, const boost::beast::http::verb& method, const std::string& authorization = "", const std::string& reason = "");
    static std::string urlEncode(const std::string &value);
    static std::string generateRandomString(const std::size_t& length);
};
#endif //HELIOS_REQUEST_HPP
