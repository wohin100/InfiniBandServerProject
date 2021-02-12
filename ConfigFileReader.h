#include <string>
using namespace std;
class ConfigFileReader {

public:
    ConfigFileReader();

    void read(string path);

    int getPort();

    string getDbUrl();

    string getDbPort();

    string getDbName();
};
