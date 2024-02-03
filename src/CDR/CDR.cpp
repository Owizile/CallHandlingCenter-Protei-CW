#include "CDR.hpp"

CDR::CDR(std::string CDRPath) {
    setCDRPath(CDRPath);
}

CDR::~CDR() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

void CDR::setCDRPath(std::string path) {
    if (outputFile.is_open()) {
        outputFile.close();
    }
    BOOST_LOG_TRIVIAL(debug) << "CDR path setted: " << path << std::endl;
    outputFile.open(path, std::ios::app);
};

void CDR::writeCall(Call* call, int callStatus) {
    boost::unique_lock<boost::mutex> lock(mutex);
    switch (callStatus) {
        case OK: {
            boost::posix_time::ptime connectTime = call->getConnectTime();
            boost::posix_time::ptime endTime = call->getEndTime();

            boost::posix_time::time_duration callDuration = endTime - connectTime;

            outputFile << "1. Incoming call DT    : " << call->getStartTime() << ";\n"
                    << "2. Call ID             : " << call->getID() << ";\n"
                    << "3. Call number         : " << call->getCallNumber() << ";\n"
                    << "4. Call termination DT : " << call->getEndTime() << ";\n"
                    << "5. Call status         : OK;\n"
                    << "6. Operator answer DT  : " << call->getConnectTime() << ";\n"
                    << "7. Operator ID         : " << call->getOperatorID() << ";\n"
                    << "8. Call duration       : " << callDuration.total_seconds() << " s;\n\n";
            break;
        }

        case alreadyInQueue: {
            outputFile << "1. Incoming call DT    : " << call->getStartTime() << ";\n"
                    << "2. Call ID             : " << call->getID() << ";\n"
                    << "3. Call number         : " << call->getCallNumber() << ";\n"
                    << "4. Call termination DT : " << call->getEndTime() << ";\n"
                    << "5. Call status         : already in queue;\n"
                    << "6. Operator answer DT  : " << ";\n"
                    << "7. Operator ID         : " << ";\n"
                    << "8. Call duration       : " << ";\n\n";
            break;
        }

        case timeout: {
            outputFile << "1. Incoming call DT    : " << call->getStartTime() << ";\n"
                    << "2. Call ID             : " << call->getID() << ";\n"
                    << "3. Call number         : " << call->getCallNumber() << ";\n"
                    << "4. Call termination DT : " << call->getEndTime() << ";\n"
                    << "5. Call status         : timeout;\n"
                    << "6. Operator answer DT  : " << ";\n"
                    << "7. Operator ID         : " << ";\n"
                    << "8. Call duration       : " << ";\n\n";
            break;
        }

        case overload: {
            outputFile << "1. Incoming call DT    : " << call->getStartTime() << ";\n"
                    << "2. Call ID             : " << call->getID() << ";\n"
                    << "3. Call number         : " << call->getCallNumber() << ";\n"
                    << "4. Call termination DT : " << call->getEndTime() << ";\n"
                    << "5. Call status         : overload;\n"
                    << "6. Operator answer DT  : " << ";\n"
                    << "7. Operator ID         : " << ";\n"
                    << "8. Call duration       : " << ";\n\n";
            break;
        }

        default: {
            break;
        }
    }

    outputFile.flush();
}
