#ifndef CALLQUEUE_HPP
#define CALLQUEUE_HPP

#include <iostream>
#include <queue>
#include <unordered_set>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/log/trivial.hpp>
#include "Call.hpp"
#include "CDR.hpp"

class CallQueue {
public:
    CallQueue(CDR* cdr_);
    ~CallQueue();

    void addCall(Call* call, std::time_t timestamp, std::time_t timeout);
    Call* dequeueCall();
    size_t getSize();
    bool isCallNumberInQueue(std::string& callNumber);

private:
    struct CallQueueItem {
        std::time_t timestamp;
        std::time_t timeout;
        Call* call;

        CallQueueItem(Call* call_, std::time_t timestamp_, std::time_t timeout_)
            : call(call_), timestamp(timestamp_), timeout(timeout_) {}

        // Comparison function for priority_queue
        bool operator<(const CallQueueItem& other) const {
            return timestamp + timeout < other.timestamp + other.timeout;
        }
    };

    std::priority_queue<CallQueueItem> queue;  // Use priority_queue
    boost::mutex mutex;
    boost::condition_variable cv;
    boost::thread expirationThread;
    bool stopExpirationThreadFlag = false;
    std::unordered_set<std::string> callNumbersInQueue;
    CDR* cdr;

    void removeExpiredCalls();
    void stopExpirationThread();
};

#endif // CALLQUEUE_HPP
