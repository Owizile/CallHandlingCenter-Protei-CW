#include "CallCenter.hpp"

#define CDR_PATH "./CDR.txt"
#define CONFIG_PATH "../../src/configuration.json"

CallCenter::CallCenter(asio::io_service& ioService_, int port)
    : ioService(ioService_),
      acceptor(ioService, ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      queueWatcherThread(&CallCenter::watchQueueAndAssignCalls, this),
      cdr(new CDR(CDR_PATH)),
      callQueue(cdr) {

    acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
    config.initialize(CONFIG_PATH);

    for (int i = 0; i < config.operatorsNum; i++) {
        Operator* op = new Operator(ioService);
        operators.push(op);
    }

    startAccept();
}

CallCenter::~CallCenter() {
    delete cdr;

    queueWatcherFlag = false;
    if (queueWatcherThread.joinable()) {
        queueWatcherThread.join();
    }

    while (!operators.empty()) {
        Operator* currentOperator = operators.front();
        operators.pop();
        delete currentOperator;
    }
}

void CallCenter::startAccept() {
    auto socket = std::make_shared<ip::tcp::socket>(ioService);
    acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& error) {
        handleAccept(socket, error);
    });
}

void CallCenter::handleAccept(std::shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& error) {
    if (!error) {
        startRead(socket);
    }
    startAccept();
}

void CallCenter::startRead(std::shared_ptr<ip::tcp::socket> socket) {
    auto request = std::make_shared<asio::streambuf>();
    asio::async_read_until(*socket, *request, "\r\n\r\n",
        [this, socket, request](const boost::system::error_code& error, std::size_t bytesRead) {
            if (!error) {
                processRequest(socket, request);
            }
        });
}

std::string getHeaderValue(const std::string& request, const std::string& header) {
    size_t startPos = request.find(header);
    if (startPos == std::string::npos) {
        return "";
    }

    startPos += header.length() + 2; // Skip header and ': '
    size_t endPos = request.find("\n", startPos);

    return request.substr(startPos, endPos - startPos);
}

std::string getCallNumberFromHTTPRequest(std::shared_ptr<asio::streambuf> request) {
    std::istream requestStream(request.get());
    std::string requestLine;
    std::string allRequest = "";
    while (std::getline(requestStream, requestLine)) {
        allRequest += requestLine;
        allRequest += "\n";
    }

    std::string contentType = getHeaderValue(allRequest, "Content-Type");
    std::string callNumber;

    if (contentType.find("text/plain") != -1) {
        callNumber = requestLine;
    } else if (contentType.find("application/json") != -1) {
        callNumber = getHeaderValue(allRequest, "Call-Number");
    }

    return callNumber;
}

void CallCenter::processRequest(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<asio::streambuf> request) {
    try {
        std::string callNumber = getCallNumberFromHTTPRequest(request);

        Call* currentCall = new Call(callNumber);
        
        size_t holdMinDuration = config.holdMinDuration;
        size_t holdMaxDuration = config.holdMaxDuration;  
        auto getTimeout = [holdMinDuration, holdMaxDuration](){
            size_t range = holdMaxDuration - holdMinDuration + 1;
            time_t timeout = holdMinDuration + rand() % range;
            return timeout;
        };

        std::string callID = boost::uuids::to_string(currentCall->getID());
        
        std::string response = "";

        if (callQueue.isCallNumberInQueue(callNumber)) {
            currentCall->setEndTime();
            cdr->writeCall(currentCall, alreadyInQueue);
            delete currentCall;
            response += "HTTP/1.1 409 Conflict\r\n";
            response += "Content-Length: 0\r\n";
            response += "CallID: " + callID + "\r\n";
            response += "\r\n";
            startWrite(socket, response);

            BOOST_LOG_TRIVIAL(info) << "Conflict: call already in queue with call ID: " << callID
                                    << "and call number: " << callNumber;

            return;
        }

        if (callQueue.getSize() >= config.queueMaxSize) {
            currentCall->setEndTime();
            cdr->writeCall(currentCall, overload);
            delete currentCall;
            response += "HTTP/1.1 503 Service Unavailable\r\n";
            response += "Content-Length: 0\r\n";
            response += "CallID: " + callID + "\r\n";
            response += "\r\n";
            startWrite(socket, response);

            BOOST_LOG_TRIVIAL(info) << "Service Unavailable: call queue is full, terminated call ID: " 
                                    << callID << " with call number: " << callNumber;

            return;
        } else {
            callQueue.addCall(currentCall, std::time(nullptr), getTimeout());
            response += "HTTP/1.1 200 OK\r\n";
            response += "Content-Length: 0\r\n";
            response += "CallID: " + callID + "\r\n";
            response += "\r\n";
            startWrite(socket, response);

            BOOST_LOG_TRIVIAL(info) << "Call added to the queue with call ID: " << callID
                                    << "and call number: " << callNumber;

            return;
        }

    } catch (const std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what();
    }
}

void CallCenter::startWrite(std::shared_ptr<ip::tcp::socket> socket, const std::string& response) {
    asio::async_write(*socket, asio::buffer(response),
        [this, socket](const boost::system::error_code& error, std::size_t) {
            if (!error) {
                socket->close();
            }
        });
}

void CallCenter::watchQueueAndAssignCalls() {
    while (queueWatcherFlag) {
        {
            boost::unique_lock<boost::mutex> lock(operatorsMutex);

            while (!operators.empty() && callQueue.getSize() != 0) {
                Operator* currentOperator = operators.front();

                if (currentOperator->isFree()) {
                    Call* nextCall = callQueue.dequeueCall();
                    currentOperator->handleCall(config.callMinDuration, config.callMaxDuration, nextCall, cdr);
                }

                operators.pop();
                operators.push(currentOperator);  
            }
        }

        usleep(100000);
    }
}
