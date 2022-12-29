#include "request.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

int request::httpsResponseCode;

int request::getRequest(const std::string& host, const std::string& target, const std::shared_ptr<cache>& cache_, const std::string& authorization, std::vector<std::string> whatToCache)
{
    typedef beast::ssl_stream<beast::tcp_stream> ssl_socket;

    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv23);
    load_root_certificates(ctx);

    ctx.set_default_verify_paths();

    // Open a socket and connect it to the remote host.
    net::io_context io_service;
    ssl_socket sock(io_service, ctx);
    tcp::resolver resolver(io_service);

    auto const results = resolver.resolve(host, "https");
    beast::get_lowest_layer(sock).connect(results);

    // Perform SSL handshake and verify the remote host's
    // certificate.
    sock.set_verify_mode(ssl::verify_peer);
    sock.set_verify_callback(ssl::rfc2818_verification(host));
    sock.handshake(ssl_socket::client);

    // ... read and write as normal ...
    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);

    try {

        // Send the HTTP request to the remote host
        http::write(sock, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(sock, buffer, res);

        const json jsonResponse = json::parse(res.body());

        // For if you need to cache a non json message
        if(whatToCache[0] == "*") cache_->put(whatToCache[1], jsonResponse.dump());

        // Loop through keys[] and caches the json response
        for (const auto& key : whatToCache) {
            if(jsonResponse.contains(key)) {
                std::string keyValue = jsonResponse.at(key).dump();

                // Check to make sure string is not double-quoted
                if(keyValue.at(0) == '"') keyValue = keyValue.substr(1, keyValue.length() - 2);

                cache_->put(key, keyValue);
            } else {
                std::cerr << "could not find key " + key + " in response " + res.body() << std::endl;
            }
        }

        request::httpsResponseCode = int((unsigned int)res.result_int());
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }
    beast::error_code ec;
    sock.shutdown(ec);
    return request::httpsResponseCode;
}

int request::postRequest(const std::string& host, const std::string& target, const std::string& payload, const std::string& authorization)
{
    typedef beast::ssl_stream<beast::tcp_stream> ssl_socket;

    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv23);
    load_root_certificates(ctx);

    ctx.set_default_verify_paths();

    // Open a socket and connect it to the remote host.
    net::io_context io_service;
    ssl_socket sock(io_service, ctx);
    tcp::resolver resolver(io_service);

    auto const results = resolver.resolve(host, "https");
    beast::get_lowest_layer(sock).connect(results);

    // Perform SSL handshake and verify the remote host's
    // certificate.
    sock.set_verify_mode(ssl::verify_peer);
    sock.set_verify_callback(ssl::rfc2818_verification(host));
    sock.handshake(ssl_socket::client);

    // ... read and write as normal ...
    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::post, target, 11};
    req.set(http::field::host,host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::connection, "close");

    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);

    req.body() = payload;
    req.prepare_payload();

    try {
        // Send the HTTP request to the remote host
        http::write(sock, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(sock, buffer, res);
        request::httpsResponseCode = int((unsigned int)res.result_int());
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }

    beast::error_code ec;
    sock.shutdown(ec);
    return request::httpsResponseCode;
}

int request::patchRequest(const std::string& host, const std::string& target, const std::string& payload, const std::string& authorization)
{
    typedef beast::ssl_stream<beast::tcp_stream> ssl_socket;

    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv23);
    load_root_certificates(ctx);

    ctx.set_default_verify_paths();

    // Open a socket and connect it to the remote host.
    net::io_context io_service;
    ssl_socket sock(io_service, ctx);
    tcp::resolver resolver(io_service);

    auto const results = resolver.resolve(host, "https");
    beast::get_lowest_layer(sock).connect(results);

    // Perform SSL handshake and verify the remote host's
    // certificate.
    sock.set_verify_mode(ssl::verify_peer);
    sock.set_verify_callback(ssl::rfc2818_verification(host));
    sock.handshake(ssl_socket::client);

    // ... read and write as normal ...
    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::patch, target, 11};
    req.set(http::field::host,host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::connection, "close");

    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);

    req.body() = payload;
    req.prepare_payload();

    try {
        // Send the HTTP request to the remote host
        http::write(sock, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(sock, buffer, res);
        request::httpsResponseCode = int((unsigned int)res.result_int());
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }

    beast::error_code ec;
    sock.shutdown(ec);
    return request::httpsResponseCode;
}

int request::deleteRequest(const std::string& host, const std::string& target, const std::string& payload, const std::string& authorization)
{
    typedef beast::ssl_stream<beast::tcp_stream> ssl_socket;

    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv23);
    load_root_certificates(ctx);

    ctx.set_default_verify_paths();

    // Open a socket and connect it to the remote host.
    net::io_context io_service;
    ssl_socket sock(io_service, ctx);
    tcp::resolver resolver(io_service);

    auto const results = resolver.resolve(host, "https");
    beast::get_lowest_layer(sock).connect(results);

    // Perform SSL handshake and verify the remote host's
    // certificate.
    sock.set_verify_mode(ssl::verify_peer);
    sock.set_verify_callback(ssl::rfc2818_verification(host));
    sock.handshake(ssl_socket::client);

    // ... read and write as normal ...
    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::delete_, target, 11};
    req.set(http::field::host,host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::connection, "close");

    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);

    req.body() = payload;
    req.prepare_payload();

    try {
        // Send the HTTP request to the remote host
        http::write(sock, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(sock, buffer, res);
        request::httpsResponseCode = int((unsigned int)res.result_int());
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
    }

    beast::error_code ec;
    sock.shutdown(ec);
    return request::httpsResponseCode;
}
