#include <string>

class InfinibandInterface {
public:
    InfinibandInterface(std::string url, std::string port, std::string database);

    void storePointInDatabase(std::string measurement, std::string field, int value);
};
