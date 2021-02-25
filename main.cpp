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

typedef void (*sighandler_t)(int);

static sighandler_t

handle_signal(int sig_nr, sighandler_t signalHandler) {
    struct sigaction newSignal, oldSignal;
    newSignal.sa_handler = signalHandler;
    sigemptyset(&newSignal.sa_mask);
    newSignal.sa_flags = SA_RESTART;
    if (sigaction(sig_nr, &newSignal, &oldSignal) < 0) {
        return SIG_ERR;
    }
    return oldSignal.sa_handler;
}
/*
static void start_daemon(const char *log_name, int facility) {

    int fileDescriptor;
    pid_t pid;

    if ((pid = fork()) != 0) {
        exit(EXIT_FAILURE);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }
    handle_signal(SIGHUP, SIG_IGN);

    if ((pid = fork()) != 0) {
        exit(EXIT_FAILURE);
    }

    chdir("/");

    umask(0);

    for (fileDescriptor = sysconf(_SC_OPEN_MAX); fileDescriptor > 0; fileDescriptor--) {
        close(fileDescriptor);
    }
    openlog(log_name, LOG_PID | LOG_CONS | LOG_NDELAY, facility);
}
*/
int main(int argc, char *argv[]) {
    // check params
    if (argc < 2) {
        printf("Usage: ./InfiniBandServer <configFilePath>\n");
        exit(EXIT_FAILURE);
    } else {
        configFileReader = new ConfigFileReader();
        configFileReader->read(argv[1]);
    }

    //start_daemon ("infinibandServer", LOG_LOCAL0);

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