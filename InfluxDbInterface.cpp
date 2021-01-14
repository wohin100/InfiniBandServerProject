#include "InfluxDbInterface.h"
#include <InfluxDB.h>
#include <InfluxDBFactory.h>
#include <nlohmann/json.hpp>

using nlohmann::json;
using namespace std;

string serverAddress;
InfluxDbInterface::InfluxDbInterface(string url, string port, string database) {
    serverAddress = url+":"+port+"/?db="+database;
}

void InfluxDbInterface::storeInfinibandInDatabase(std::string measurement, std::string receivedJsonString, std::string clientName,
                                                  std::string clientPort)
{
    auto receivedJson = json::parse(receivedJsonString);

    auto influxdb = influxdb::InfluxDBFactory::Get(serverAddress);
    influxdb->write(influxdb::Point{measurement}
                            .addField("node", receivedJson["node"].get<string>())
                            .addField("transmitted", receivedJson["transmitted"].get<int>())
                            .addField("received", receivedJson["received"].get<int>())
                            .addField("MulticastRcvPkts", receivedJson["MulticastRcvPkts"].get<int>())
                            .addTag("node", receivedJson["node"].get<string>())
                            .addTag("receivedViaPort", clientPort)
                            .addTag("receivedViaAddress", clientName)
    );
}

void InfluxDbInterface::storePointInDatabase(string measurement, string field, int value)
{
    auto influxdb = influxdb::InfluxDBFactory::Get(serverAddress);

    influxdb->write(influxdb::Point{measurement}
                            .addField(field, value)
                            .addTag("host", "localhost")
    );
}
