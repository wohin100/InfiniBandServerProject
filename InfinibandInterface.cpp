#include "InfinibandInterface.h"
#include <InfluxDB.h>
#include <InfluxDBFactory.h>
using namespace std;

string serverAddress;
InfinibandInterface::InfinibandInterface(string url, string port, string database) {
    serverAddress = url+":"+port+"/?db="+database;
}

void InfinibandInterface::storePointInDatabase(string measurement, string field, int value)
{
    auto influxdb = influxdb::InfluxDBFactory::Get(serverAddress);

    influxdb->write(influxdb::Point{measurement}
                            .addField(field, value)
                            .addTag("host", "localhost")
    );
}
