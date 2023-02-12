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
#include <future>
#include <fstream>
#include <memory>
#include <chrono>
#include <thread>

#include "rateLimit.hpp"
#include "heliosException.hpp"
#include "discordClasses/channel/attachment.hpp"
#include "ssl/root_certification.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

class request : public std::enable_shared_from_this<request> {
private:
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::string_body> req_;
    http::response<http::string_body> res_;
    const httpRequest& reqInfo_;

public:
    explicit request(net::io_context& ioc, ssl::context& ctx, const httpRequest& reqInfo);
    void run();
    void on_resolve(beast::error_code ec, const tcp::resolver::results_type& results);
    void on_connect(beast::error_code ec, const tcp::resolver::results_type::endpoint_type&);
    void on_handshake(beast::error_code ec);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_shutdown(beast::error_code ec);
    void fail(beast::error_code ec, char const* what);

    static void sendHttpRequest(const httpRequest& reqInfo);
    static std::string urlEncode(const std::string &value);
    static std::string generateRandomString(const std::size_t& length);
};
#endif //HELIOS_REQUEST_HPP
