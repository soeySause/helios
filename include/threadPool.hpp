#ifndef HELIOS_THREADPOOL_HPP
#define HELIOS_THREADPOOL_HPP
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <thread>

#include "request.hpp"
#include "ssl/root_certification.hpp"

namespace helios {
    class strand {
    private:
        friend class thread;
        std::shared_ptr<boost::asio::strand<boost::asio::io_context::executor_type>> strandPtr;
    public:
        explicit strand(const std::shared_ptr<boost::asio::io_context>& ioContext);
        ~strand();

    };
    class thread {
    private:
        friend class threadPool;
        int numRequests = 0;

        int minStrands = 1;
        int maxStrands = 50;
        int numStrands = 5;
        int strandTimeout = 120;

        std::unique_ptr<std::thread> threadPtr;
        std::shared_ptr<boost::asio::io_context> ioContext = std::make_shared<boost::asio::io_context>();
        std::vector<std::unique_ptr<strand>> strandVector;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work;
        boost::asio::ssl::context ctx;
    public:
        thread();
        ~thread();
        void createHttpRequest(const httpRequest& reqInfo);
    };

    class threadPool {
    private:
        int numThreads;
        int reqCounter = 0;
        std::vector<std::shared_ptr<helios::thread>> threads;
        std::mutex threadPoolLock;
    public:
        explicit threadPool(const int& numThreads);
        ~threadPool();
        void queueHttpRequest(const httpRequest& reqInfo);
        void resize(const int& numThreads);
    };
} // helios

#endif //HELIOS_THREADPOOL_HPP
