#include <string>
#include <csignal>
#include <unistd.h>
//#include <archive.h>
#include <syslog.h>
#include <iostream>
#include "InfluxDbInterface.h"
#include "MultiThreadServer.h"
#include "ConfigFileReader.h"

using namespace std;

char *serverAddress;
MultiThreadServer *server;
ConfigFileReader *configFileReader;

int main(int argc, char *argv[]) {
    // check params
    if (argc < 2) {
        printf("Usage: ./InfiniBandServer <configFilePath>\n");
        exit(EXIT_FAILURE);
    } else {
        configFileReader = new ConfigFileReader();
        configFileReader->read(argv[1]);
    }


    server = new MultiThreadServer
            (
                    configFileReader->getPort(),
                    configFileReader->getDbUrl(),
                    configFileReader->getDbPort(),
                    configFileReader->getDbName()
            );
    syslog(LOG_NOTICE, "InfiniBandServer started\n");
    cout << "start server" << endl;

    server->run();

    syslog(LOG_NOTICE, "InfiniBandServer stopped\n");
    closelog();
    return EXIT_SUCCESS;
}