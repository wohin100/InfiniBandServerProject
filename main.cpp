#include <iostream>
#include <string>
#include <csignal>
#include "InfluxDbInterface.h"
#include "MultiThreadServer.h"

using namespace std;
int port = 80;
char* serverAddress;
MultiThreadServer* server;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        std::cout << "Server stopped" << std::endl;
        server->stop();
    }
}

int main(int argc, char *argv[])
{
    // check params
    if(argc < 3) {
        printf("Usage: ./InfiniBandServer <serverAddress> <serverPort>\n");
        exit(EXIT_FAILURE);
    }else{
        serverAddress = argv[1];
        port = stoi(argv[2]);
    }

    // start server
    string dbUrl = "http://localhost";
    string dbPort = "8086";
    string dbName = "infiniband";
    server = new MultiThreadServer(port, dbUrl, dbPort, dbName);
    server->run();

    return 0;
}