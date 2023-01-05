#ifndef HELIOS_SESSION_HPP
#define HELIOS_SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/strand.hpp>

#include <future>
#include <iostream>
#include <thread>
#include <memory>

#include "cache.hpp"
#include "ssl/root_certification.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Report a failure
void fail(beast::error_code ec, char const* what);

// Sends a WebSocket message and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws_;
    beast::flat_buffer buffer_;
    std::string host_;
    std::string text_;
    net::steady_timer timer_;
    std::vector <std::string> queue_;
    std::vector <bool> readingQueue_;
    std::string responseString;

    //Initialized as true to ensure no messages are sent until connection is established
    bool currentlyQueued_ = true;

public:
    explicit session(net::io_context& ioc, ssl::context& ctx, const std::shared_ptr<cache>& cache);

    void run(const std::basic_string<char, std::char_traits<char>, std::allocator<char>>& host, char const* port);
    void on_resolve(beast::error_code ec, const tcp::resolver::results_type& results);
    void on_connect(beast::error_code ec, const tcp::resolver::results_type::endpoint_type& ep);
    void on_ssl_handshake(beast::error_code ec);
    void on_handshake(beast::error_code ec);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
    void on_read(beast::error_code ec, std::size_t bytes_transferred);

    bool is_socket_open();
    void enable_async_read();
    std::string getResponse();
    void closeSession();

    void asyncQueue(const std::string& payload = "");
};



#endif //HELIOS_SESSION_HPP