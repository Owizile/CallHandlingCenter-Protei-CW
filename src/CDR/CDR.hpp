#ifndef CDR_HPP
#define CDR_HPP

#include <string>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#include "Call.hpp"

enum CallStatus {OK, timeout, alreadyInQueue, overload};

class CDR {
private:
    std::ofstream outputFile;
    boost::mutex mutex;
public:
    CDR(std::string CDRPath);
    ~CDR();
    void writeCall(Call* call, int callStatus);
    void setCDRPath(std::string path);
};

#endif // CDR_HPP
