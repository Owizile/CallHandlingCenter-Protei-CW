#ifndef CALLCENTER_HPP
#define CALLCENTER_HPP

#include <iostream>
#include <string>
#include <queue>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <memory>
#include "CallCenterConfig.hpp"
#include "Operator.hpp"
#include "CallQueue.hpp"
#include "CDR.hpp"

namespace asio = boost::asio;
namespace ip = asio::ip;

class CallCenter {
private:
    CallCenterConfig config;
    std::queue<Operator*> operators;
    CDR* cdr;
    CallQueue callQueue;

    asio::io_service& ioService;
    ip::tcp::acceptor acceptor;

    std::mutex operatorsMutex; 
    bool queueWatcherFlag = true; 
    std::thread queueWatcherThread;

    void startAccept();
    void handleAccept(std::shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& error);
    void startRead(std::shared_ptr<ip::tcp::socket> socket);
    void processRequest(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<asio::streambuf> request);
    void startWrite(std::shared_ptr<ip::tcp::socket> socket, const std::string& response);
    void watchQueueAndAssignCalls();

public:
    CallCenter(asio::io_service& ioService_, int port);
    ~CallCenter();
};

#endif // CALLCENTER_HPP
