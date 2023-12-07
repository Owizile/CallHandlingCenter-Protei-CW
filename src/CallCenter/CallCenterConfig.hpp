#ifndef CALLCENTERCONFIG_HPP
#define CALLCENTERCONFIG_HPP

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <boost/log/trivial.hpp>

using json = nlohmann::json;

class CallCenterConfig {
private:
    std::fstream configFile;
public:
    size_t queueMaxSize;
    size_t holdMinDuration, holdMaxDuration;
    size_t callMinDuration, callMaxDuration;
    size_t operatorsNum;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CallCenterConfig,
                            queueMaxSize,
                            holdMinDuration,
                            holdMaxDuration,
                            callMinDuration,
                            callMaxDuration,
                            operatorsNum)

    ~CallCenterConfig() {
        if (configFile.is_open()) {
            configFile.close();
        }
    }

    void initialize(std::string configPath) {
        configFile.open(configPath, std::ios::in);

        if (!std::filesystem::exists(configPath)) {
            BOOST_LOG_TRIVIAL(error) << "Configuration file doesn't exist: " << configPath << std::endl;
        }

        if (!configFile.is_open()) {
            BOOST_LOG_TRIVIAL(error) << "Failed to open configuration file." << std::endl;
        }

        json configJSON;
        try {
            configJSON = json::parse(configFile);
            configJSON.get_to(*this); 
            BOOST_LOG_TRIVIAL(debug) << "Config loaded, config path is: " << configPath << std::endl;
        } catch (const json::parse_error& e) {
            BOOST_LOG_TRIVIAL(error) << "JSON parse error: " << e.what() << std::endl;
        }
    }
};

#endif // CALLCENTERCONFIG_HPP
