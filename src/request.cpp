#include "request.hpp"
#include <chrono>
#include <thread>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

int request::httpsResponseCode;
std::string request::httpsResponseReason;


std::string request::httpsRequest(const std::string& host, const std::string& target, const json& payload, const http::verb& method, const std::shared_ptr<rateLimitStruct>& rateLimit, const std::string& authorization, const std::string& reason) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rateLimit->rateLimitGlobal->countHttpsReq()));

    typedef beast::ssl_stream<beast::tcp_stream> ssl_socket;

    ssl::context ctx(ssl::context::sslv23);
    load_root_certificates(ctx);
    ctx.set_default_verify_paths();

    net::io_context io_service;
    ssl_socket sock(io_service, ctx);
    tcp::resolver resolver(io_service);

    auto const results = resolver.resolve(host, "https");
    beast::get_lowest_layer(sock).connect(results);

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

std::string request::urlEncode(const std::string &value) {
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

std::string request::attachmentHttpsRequest(const std::string &host, const std::string &target, const json &jsonPayload, const std::vector<helios::attachment>& attachments, const http::verb &method, const std::string &authorization, const std::string &reason) {
    typedef beast::ssl_stream<beast::tcp_stream> ssl_socket;

    ssl::context ctx(ssl::context::sslv23);
    load_root_certificates(ctx);
    ctx.set_default_verify_paths();

    net::io_context io_service;
    ssl_socket sock(io_service, ctx);
    tcp::resolver resolver(io_service);

    auto const results = resolver.resolve(host, "https");
    beast::get_lowest_layer(sock).connect(results);

    sock.set_verify_mode(ssl::verify_peer);
    sock.set_verify_callback(ssl::rfc2818_verification(host));
    sock.handshake(ssl_socket::client);

    // ... read and write as normal ...
    http::request<http::string_body> req{method, target, 11};
    const std::string boundary = request::generateRandomString(10);

    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "multipart/form-data; boundary=" + boundary);


    if(!authorization.empty()) req.set(http::field::authorization, "Bot " + authorization);
    if(!reason.empty()) req.set("X-Audit-Log-Reason", reason);

    std::string body;
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"payload_json\"\r\n";
    body += "Content-Type: application/json\r\n\r\n";
    body += jsonPayload.dump();

    int i = 0;
    for(auto& attachment : attachments) {
        assert(attachment.filename.has_value());
        assert(attachment.contentType.has_value());
        assert(attachment.pathToFile.has_value());

        body +=  "\r\n--" + boundary + "\r\n";
        body += "Content-Disposition: form-data; name=\"files[" + std::to_string(i) + "]\"; filename=\"" + attachment.filename.value() + "\"\r\n";
        body += "Content-Type: " + attachment.contentType.value() + "\r\n\r\n";


        std::ifstream file(attachment.pathToFile.value(), std::ios::binary | std::ios::in);
        std::vector<char> file_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        body += std::string(file_data.begin(), file_data.end()) + "\r\n";
        body += "--" + boundary;
        i++;
    }

    body += "--";
    req.body() = body;
    req.prepare_payload();

    // Send the HTTP request to the remote host
    http::write(sock, req);
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(sock, buffer, res);

    request::httpsResponseCode = int((unsigned int)res.result_int());
    request::httpsResponseReason = res.body();

    if(httpsResponseCode != 200) {
        std::cout << req.body() << std::endl;
        throw (helios::heliosException(request::httpsResponseCode,
                                       "\nError code: " + std::to_string(request::httpsResponseCode) +
                                       "\nEndpoint: " + std::string(boost::beast::http::to_string(req.method())) + " " + target +
                                       "\nError message: " + request::httpsResponseReason));
    }

    return res.body();

}

std::string request::generateRandomString(const std::size_t& length)
{
    static constexpr char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<std::size_t> distribution(0, sizeof(characters) - 2);

    std::string result(length, ' ');
    for (std::size_t i = 0; i < length; ++i)
        result[i] = characters[distribution(generator)];

    return result;
}
