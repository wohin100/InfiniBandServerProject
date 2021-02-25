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

void InfluxDbInterface::checkIfDatabaseExists(string database) {
    try {
        auto influxdb = InfluxDBFactory::Get(serverAddress);
    } catch (runtime_error e) {
        cerr << "Database "+database+" doesn't exist and can't be created" << endl;
        exit(EXIT_FAILURE);
    }
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
                                .addField("nodeNumPorts", receivedJson["nodeNumPorts"].get<int>())
                                .addField("nodeGuid", receivedJson["nodeGuid"].get<int>())

                                .addField("nodeUnicastRcvPkts", receivedJson["nodeUnicastRcvPkts"].get<int>())
                                .addField("nodeUnicastXmitPkts", receivedJson["nodeUnicastXmitPkts"].get<int>())

                                .addField("nodeMulticastRcvPkts", receivedJson["nodeMulticastRcvPkts"].get<int>())
                                .addField("nodeMulticastXmitPkts", receivedJson["nodeMulticastXmitPkts"].get<int>())

                                .addField("nodeRcvPkts", receivedJson["nodeRcvPkts"].get<int>())
                                .addField("nodeRcvErrors", receivedJson["nodeRcvErrors"].get<int>())
                                .addField("nodeRcvConstraintErrors", receivedJson["nodeRcvConstraintErrors"].get<int>())
                                .addField("nodeRcvDataBytes", receivedJson["nodeRcvDataBytes"].get<int>())
                                .addField("nodeRcvRemotePhysicalErrors", receivedJson["nodeRcvRemotePhysicalErrors"].get<int>())
                                .addField("nodeRcvSwitchRelayErrors", receivedJson["nodeRcvSwitchRelayErrors"].get<int>())

                                .addField("nodeXmitWait", receivedJson["nodeXmitWait"].get<int>())
                                .addField("nodeXmitPkts", receivedJson["nodeXmitPkts"].get<int>())
                                .addField("nodeXmitDiscards", receivedJson["nodeXmitDiscards"].get<int>())
                                .addField("nodeXmitDataBytes", receivedJson["nodeXmitDataBytes"].get<int>())
                                .addField("nodeXmitConstraintErrors", receivedJson["nodeXmitConstraintErrors"].get<int>())

                                .addField("nodeExcessiveBufferOverrunErrors", receivedJson["nodeExcessiveBufferOverrunErrors"].get<int>())
                                .addField("nodeLinkDownedCounter", receivedJson["nodeLinkDownedCounter"].get<int>())
                                .addField("nodeLocalLinkIntegrityErrors", receivedJson["nodeLocalLinkIntegrityErrors"].get<int>())
                                .addField("nodeLinkRecoveryCounter", receivedJson["nodeLinkRecoveryCounter"].get<int>())

                                .addField("nodeVL15Dropped", receivedJson["nodeLinkRecoveryCounter"].get<int>())

                                .addTag("nodeDescription", receivedJson["nodeName"].get<string>())
                                .addTag("receivedViaPort", clientPort)
                                .addTag("receivedViaAddress", clientName)
                );
    }
    catch (runtime_error e) {
        cerr << "Could not write values to DB" << endl;
        cerr << e.what() << endl;

        return;
    }
}
