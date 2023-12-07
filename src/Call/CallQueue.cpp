#include "CallQueue.hpp"

CallQueue::CallQueue(CDR* cdr_) : cdr(cdr_), expirationThread(&CallQueue::removeExpiredCalls, this) {}

CallQueue::~CallQueue() {
    stopExpirationThread();
    while (!queue.empty()) {
        Call* call = dequeueCall();
        delete call;
    }
}

void CallQueue::addCall(Call* call, std::time_t timestamp, std::time_t timeout) {
    {
        boost::unique_lock<boost::mutex> lock(mutex);
        queue.push(CallQueueItem{call, timestamp, timeout});
        callNumbersInQueue.insert(call->getCallNumber());
    }
    cv.notify_one();
}

Call* CallQueue::dequeueCall() {
    boost::unique_lock<boost::mutex> lock(mutex);
    if (!queue.empty()) {
        Call* call = queue.top().call;
        queue.pop();
        callNumbersInQueue.erase(call->getCallNumber());
        return call;
    }
    return nullptr;
}

bool CallQueue::isCallNumberInQueue(std::string& callNumber) {
    boost::unique_lock<boost::mutex> lock(mutex);
    return callNumbersInQueue.find(callNumber) != callNumbersInQueue.end();
}

size_t CallQueue::getSize() {
    boost::unique_lock<boost::mutex> lock(mutex);
    return queue.size();
}

void CallQueue::removeExpiredCalls() {
    while (!stopExpirationThreadFlag) {
        boost::unique_lock<boost::mutex> lock(mutex);

        cv.wait(lock, [this] { return !queue.empty() || stopExpirationThreadFlag;; });

        std::time_t current_time = std::time(nullptr);

        while (!queue.empty() && (queue.top().timestamp + queue.top().timeout <= current_time)) {
            CallQueueItem frontCallItem = queue.top();
            Call* currentCall = frontCallItem.call;
            BOOST_LOG_TRIVIAL(info) << "Removing call from queue with number: " 
                                    << currentCall->getCallNumber()
                                    << " call ID: " << currentCall->getID()
                                    << " and timeout at: " 
                                    << boost::posix_time::from_time_t(frontCallItem.timestamp +
                                                                      frontCallItem.timeout);
            callNumbersInQueue.erase(currentCall->getCallNumber());
            currentCall->setEndTime();
            cdr->writeCall(currentCall, timeout);
            queue.pop();
            delete currentCall;
        }
    }
}

void CallQueue::stopExpirationThread() {
    stopExpirationThreadFlag = true;

    cv.notify_one();

    if (expirationThread.joinable()) {
        expirationThread.join();
    }
}
