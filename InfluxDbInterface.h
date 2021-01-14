#include <string>
#include <nlohmann/json.hpp>

class InfluxDbInterface {
public:
    InfluxDbInterface(std::string url, std::string port, std::string database);

    void storePointInDatabase(std::string measurement, std::string field, int value);

    void
    storeInfinibandInDatabase(std::string measurement, std::string receivedJsonString, std::string clientName,
                              std::string clientPort);
};
