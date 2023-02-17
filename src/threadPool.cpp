#include "threadPool.hpp"

namespace helios {
    thread::thread() : work(boost::asio::make_work_guard(*ioContext)),
    ctx(boost::asio::ssl::context::tlsv12_client) {
        load_root_certificates(ctx);
        this->ctx.set_verify_mode(boost::asio::ssl::verify_peer);

        for(int i = 0; i < numStrands; i++) {
            this->strandVector.emplace_back(std::make_unique<strand>(this->ioContext));
        }

        this->threadPtr = std::make_unique<std::thread>([this]() {
            this->ioContext->run();
        });
    }

    thread::~thread() {
        ioContext->stop();
        threadPtr->join();
    }

    void thread::createHttpRequest(const httpRequest& reqInfo) {
        std::make_shared<request>(*this->ioContext, this->ctx, this->strandVector[0]->strandPtr, reqInfo)->run();
        numRequests++;
    }

    threadPool::threadPool(const int& numThreads) {
        this->numThreads = numThreads;
        for (int i = 0; i < numThreads; i++) {
            this->threads.emplace_back(std::make_shared<helios::thread>());
        }
    }

    threadPool::~threadPool() {
        for (auto& thread : this->threads) {
            thread->ioContext->stop();
            thread->threadPtr->join();
        }
    }

    void threadPool::queueHttpRequest(const httpRequest &reqInfo) {
        std::unique_lock<std::mutex> lock(this->threadPoolLock);
        this->threads[this->reqCounter % this->numThreads]->createHttpRequest(reqInfo);
        this->reqCounter++;
    }

    void threadPool::resize(const int &newNumThreads) {
        if(newNumThreads == this->numThreads) return;
        if(newNumThreads > this->numThreads) {
            for(int i = 0; i < newNumThreads - this->numThreads; i++) {
                this->threads.emplace_back(std::make_shared<helios::thread>());
                std::cout  << "Added thread to thread pool" << std::endl;
            }
            return;
        }

        for(int i = 0; i < this->numThreads - newNumThreads; i ++) {
            this->threads.pop_back();
            std::cout  << "Removed thread from thread pool" << std::endl;
        }
    }

    strand::strand(const std::shared_ptr<boost::asio::io_context>& ioContext) {
        this->strandPtr = std::make_shared<boost::asio::strand<boost::asio::io_context::executor_type>>(ioContext->get_executor());
    }

    strand::~strand() {
        boost::asio::post(*this->strandPtr, [](){});
    }
} // helios