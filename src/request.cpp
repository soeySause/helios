#include "request.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

int request::httpsResponseCode;
std::string request::httpsResponseReason;

std::string request::httpsRequest(const std::string& host, const std::string& target, const std::string& payload, const std::string& method, const std::string& authorization)
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
    http::request<http::string_body> req;
    if(method == "get") {
        req.method(http::verb::get);
    } else if(method == "post") {
        req.method(http::verb::post);
    } else if(method == "put") {
        req.method(http::verb::put);
    } else {
        throw(helios::heliosException(71, "Unknown method " + method));
    }

    req.target(target);
    req.version(11);
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);


        // Send the HTTP request to the remote host
        http::write(sock, req);
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(sock, buffer, res);

        request::httpsResponseCode = int((unsigned int)res.result_int());
        request::httpsResponseReason = res.body();

        if(httpsResponseCode != 200) {
            throw(helios::heliosException(request::httpsResponseCode, "\nEndpoint: " + target + "\nError message: " + request::httpsResponseReason));
        }

        return res.body();
}