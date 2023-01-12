#include "session.hpp"

// Report a failure
void fail(beast::error_code ec, char const* what)
{
    if(std::to_string(*what) == "connect") {
        exit(1);
    }
    std::cerr << what << ": " << ec.message() << "\n";
}

// Resolver and socket require an io_context
session::session(net::io_context& ioc, ssl::context& ctx, const std::shared_ptr<cache>& cache)
        : resolver_(net::make_strand(ioc))
        , timer_(ioc)
        , ws_(net::make_strand(ioc), ctx){};

// Start the asynchronous operation
void session::run(
        const std::basic_string<char, std::char_traits<char>, std::allocator<char>>& host,
        char const* port)
{
    // Save these for later
    host_ = host;

    // Look up the domain name
    resolver_.async_resolve(
            host,
            port,
            beast::bind_front_handler(
                    &session::on_resolve,
                    shared_from_this()));
}

void session::on_resolve(
        beast::error_code ec,
        const tcp::resolver::results_type& results)
{
    if(ec)
        return fail(ec, "resolve");

    // Set a timeout on the operation
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(ws_).async_connect(
            results,
            beast::bind_front_handler(
                    &session::on_connect,
                    shared_from_this()));

}

void session::on_connect(beast::error_code ec, const tcp::resolver::results_type::endpoint_type& ep)
{
    if(ec)
        return fail(ec, "connect");

    // Set a timeout on the operation
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(! SSL_set_tlsext_host_name(
            ws_.next_layer().native_handle(),
            host_.c_str()))
    {
        ec = beast::error_code(static_cast<int>(::ERR_get_error()),
                               net::error::get_ssl_category());
        return fail(ec, "connect");
    }

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    host_ += ':' + std::to_string(ep.port());

    // Perform the SSL handshake
    ws_.next_layer().async_handshake(
            ssl::stream_base::client,
            beast::bind_front_handler(
                    &session::on_ssl_handshake,
                    shared_from_this()));

}

void session::on_ssl_handshake(beast::error_code ec)
{
    if(ec)
        return fail(ec, "ssl_handshake");

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(ws_).expires_never();

    // Set suggested timeout settings for the websocket
    ws_.set_option(
            websocket::stream_base::timeout::suggested(
                    beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-async-ssl");
            }));

    // Perform the websocket handshake
    ws_.async_handshake(host_, "/",
                        beast::bind_front_handler(
                                &session::on_handshake,
                                shared_from_this()));

}

void session::on_handshake(beast::error_code ec)
{
    if(ec)
        return fail(ec, "handshake");

    // Send the message
    ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                    &session::on_read,
                    shared_from_this()));

}

void session::on_write(
        beast::error_code ec,
        std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    if(ec) return fail(ec, "write");
    queue_.erase(queue_.begin());

}

void session::on_read(
        beast::error_code ec,
        std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "read");

    responseString = beast::buffers_to_string(buffer_.data());
    buffer_.consume(buffer_.size());

    currentlyQueued_ = false;
    asyncQueue();
}

void session::onClose(beast::error_code ec) {
    if(ec) return fail(ec, "close");
};

void session::asyncCloseSession(const websocket::close_code& closeCode)
{
    if(is_socket_open()) ws_.async_close(closeCode, beast::bind_front_handler(&session::onClose, shared_from_this()));
    buffer_.consume(buffer_.size());

    reinterpret_cast<boost::asio::io_context &>(ws_.get_executor().context()).stop();
}


std::uint16_t session::getCloseCode() {
    return ws_.reason().code;
}

beast::static_string<123, char> session::getCloseReason() {
    return ws_.reason().reason;
}


bool session::is_socket_open()
{
    return ws_.is_open();
}

void session::enable_async_read() {
    ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                    &session::on_read,
                    shared_from_this()));
}

void session::asyncQueue(const std::string& payload) {
    //If something to write, add to back of queue
    if(!payload.empty()) {
        queue_.emplace_back(payload);
    }

    if(queue_.empty()) return;

    // If there is something to write, write it.
    if(!currentlyQueued_) {
        currentlyQueued_ = true;

        ws_.async_write(
                net::buffer(queue_.at(0)),
                beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this()));

    }
}

std::string session::getResponse()
{
    return responseString;
}
