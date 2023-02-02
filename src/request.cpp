#include "request.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

int request::httpsResponseCode;
std::string request::httpsResponseReason;


std::string request::httpsRequest(const std::string& host, const std::string& target, const json& payload, const http::verb& method, const std::string& authorization, const std::string& reason)
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
    http::request<http::string_body> req{method, target, 11};

    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");


    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);
    if(!reason.empty()) req.set("X-Audit-Log-Reason", reason);
    if(method != http::verb::get) {
        req.body() = payload.dump();
        req.prepare_payload();
    }
    // Send the HTTP request to the remote host
    http::write(sock, req);
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(sock, buffer, res);

    request::httpsResponseCode = int((unsigned int)res.result_int());
    request::httpsResponseReason = res.body();

    if(httpsResponseCode == 204) return "";
    if(httpsResponseCode != 200) {
        throw (helios::heliosException(request::httpsResponseCode,
                                       "\nError code: " + std::to_string(request::httpsResponseCode) +
                                       "\nEndpoint: " + std::string(boost::beast::http::to_string(req.method())) + " " + target +
                                       "\nError message: " + request::httpsResponseReason));
    }

    return res.body();
}

std::string request::url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }
        escaped << '%' << std::setw(2) << int((unsigned char) c);
    }
    return escaped.str();
}