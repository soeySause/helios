#include "request.hpp"

// Report a failure
void request::fail(beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

// Performs an HTTP GET and prints the response
request::request(net::io_context &ioc, ssl::context &ctx, const httpRequest& reqInfo)
: resolver_(ioc)
, reqInfo_(reqInfo)
, stream_(ioc, ctx) {}


void request::run() {
    if(! SSL_set_tlsext_host_name(stream_.native_handle(), this->reqInfo_.host.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        std::cerr << ec.message() << "\n";
        return;
    }

    this->req_.version(this->reqInfo_.version);
    this->req_.target(this->reqInfo_.target);
    this->req_.set(http::field::host, this->reqInfo_.host);
    this->req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    this->req_.method(this->reqInfo_.method);
    this->req_.set(http::field::content_type, "application/json");

    if (!this->reqInfo_.authorization.empty()) req_.set(http::field::authorization, "Bot " + this->reqInfo_.authorization);
    if (!this->reqInfo_.reason.empty()) this->req_.set("X-Audit-Log-Reason", this->reqInfo_.reason);
    if (this->reqInfo_.method != http::verb::get) {
        this->req_.body() = this->reqInfo_.payload.dump();
        this->req_.prepare_payload();
    }

    resolver_.async_resolve(this->reqInfo_.host, this->reqInfo_.port,
            beast::bind_front_handler(
                    &request::on_resolve,
                    shared_from_this()));
}

void request::on_resolve(beast::error_code ec, const tcp::resolver::results_type& results) {
    if(ec)
        return fail(ec, "resolve");

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(stream_).async_connect( results,
            beast::bind_front_handler(
                    &request::on_connect,
                    shared_from_this()));
}

void request::on_connect(beast::error_code ec, const tcp::resolver::results_type::endpoint_type&) {
    if(ec)
        return fail(ec, "connect");

    stream_.async_handshake(
            ssl::stream_base::client,
            beast::bind_front_handler(
                    &request::on_handshake,
                    shared_from_this()));
}

void request::on_handshake(beast::error_code ec) {
    if(ec)
        return fail(ec, "handshake");

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    http::async_write(stream_, req_,
                      beast::bind_front_handler(
                              &request::on_write,
                              shared_from_this()));
}

void request::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    http::async_read(stream_, buffer_, res_,
                     beast::bind_front_handler(
                             &request::on_read,
                             shared_from_this()));
}

void request::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "read");

    unsigned int httpsResponseCode = res_.result_int();
    switch (httpsResponseCode) {
        case (204):
        case (200): {
            this->reqInfo_.onResponse(res_);
            break;
        };
        default : {
            throw (helios::heliosException((int)httpsResponseCode,
                                           "\nError code: " + std::to_string(httpsResponseCode) +
                                           "\nEndpoint: " + std::string(boost::beast::http::to_string(req_.method())) + " " + this->reqInfo_.target +
                                           "\nError message: " + res_.body()));
        }
    }
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    stream_.async_shutdown(
            beast::bind_front_handler(
                    &request::on_shutdown,
                    shared_from_this()));
}

void request::on_shutdown(beast::error_code ec) {
    // Rationale:
    // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
    if(ec == net::error::eof) ec = {};

    if(ec)
        return fail(ec, "shutdown");
}

void request::sendHttpRequest(const httpRequest& reqInfo) {
    net::io_context ioc;
    ssl::context ctx{ssl::context::tlsv12_client};
    load_root_certificates(ctx);
    ctx.set_verify_mode(ssl::verify_peer);
    std::make_shared<request>(ioc, ctx, reqInfo)->run();
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

/*std::string request::attachmentHttpsRequest(const std::string &host, const std::string &target, const json &jsonPayload, const std::vector<helios::attachment>& attachments, const http::verb &method, const std::string &authorization, const std::string &reason) {
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
*/
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

