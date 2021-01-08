#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <csignal>
#include <InfluxDB.h>
#include <InfluxDBFactory.h>
#include "InfinibandInterface.h"
#include "Server.h"

using namespace std;
int port = 80;
char* serverAddress;
Server* server;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        server->stop();
    }
}

int main(int argc, char *argv[])
{
    //auto* influxdbInterface = new InfinibandInterface("http://localhost", "8086", "infiniband");
    //influxdbInterface->storePointInDatabase("test", "value", 10000);

    /*
    for (int i = 0; i < 1000; ++i) {
        // Provide complete URI
        auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086/?db=infiniband");

        influxdb->write(influxdb::Point{"test"}
                                .addField("value", i)
                                .addTag("host", "localhost")
        );
        printf("Done\n");
    }
    */

    // check params
    if(argc < 3) {
        printf("Usage: ./InfiniBandServer <serverAddress> <port>\n");
        exit(EXIT_FAILURE);
    }else{
        serverAddress = argv[1];
        port = stoi(argv[2]);
    }

    server = new Server(port);


    return 0;
}