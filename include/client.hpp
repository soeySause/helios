#ifndef HELIOS_CLIENT_HPP
#define HELIOS_CLIENT_HPP

#include <string>
#include <iostream>
#include <memory>
#include <boost/asio/strand.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "cache.hpp"
#include "ssl/root_certification.hpp"
#include "session.hpp"
#include "discordApiRequest.hpp"

namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>

namespace helios {
    class client {
    private:
        net::io_context ioContext;
        ssl::context sslContext{ssl::context::tlsv12_client};
        std::shared_ptr<cache> cache_;
        std::shared_ptr<session> p;
        std::mutex global_stream_lock;
    public:
        explicit client();
        //void heartbeatCycle();
        //void sendHeartbeat();
        //void sendWsMessage(const string& message, bool synchronous = false);
        //void endWsConnection();
        void run(const std::string& token);
    };

} // helios

#endif //HELIOS_CLIENT_HPP
