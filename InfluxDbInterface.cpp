#include "InfluxDbInterface.h"
#include <InfluxDB.h>
#include <InfluxDBFactory.h>
#include <nlohmann/json.hpp>
#include <iostream>

using nlohmann::json;
using namespace std;
using namespace influxdb;

string serverAddress;

class InfluxDBException;

InfluxDbInterface::InfluxDbInterface(string url, string port, string database) {
    serverAddress = url + ":" + port + "/?db=" + database;
    //TODO check if db exists
}

void InfluxDbInterface::storeInfinibandInDatabase(
        string measurement,
        string receivedJsonString,
        string clientName,
        string clientPort
) {
    json receivedJson;
    try {
        receivedJson = json::parse(receivedJsonString);
    } catch (const nlohmann::detail::parse_error& e) {
        cerr << e.what() << endl;
        return;
    }

    try {
        auto influxdb = InfluxDBFactory::Get(serverAddress);
        influxdb->write
                (
                        influxdb::Point{measurement}
                                .addField("nodeDescription", receivedJson["nodeDescription"].get<string>())
                                .addField("nodeNumPorts", receivedJson["nodeNumPorts"].get<int>())
                                .addField("nodeGuid", receivedJson["nodeGuid"].get<string>())
                                .addField("nodeUnicastRcvPkts", receivedJson["nodeUnicastRcvPkts"].get<int>())
                                .addField("nodeUnicastXmitPkts", receivedJson["nodeUnicastXmitPkts"].get<int>())
                                .addTag("nodeDescription", receivedJson["nodeDescription"].get<string>())
                                .addTag("receivedViaPort", clientPort)
                                .addTag("receivedViaAddress", clientName)
                );
    }
    catch (runtime_error e) {
        return;
    }
}

void InfluxDbInterface::storePointInDatabase(string measurement, string field, int value) {
    auto influxdb = influxdb::InfluxDBFactory::Get(serverAddress);

    influxdb->write
            (
                    influxdb::Point{measurement}
                            .addField(field, value)
                            .addTag("host", "localhost")
            );
}
