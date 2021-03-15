#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <netdb.h>
#include <cstring>

#include "MultiThreadServer.h"
#include "InfluxDbInterface.h"

using namespace std;

int serverPort;
bool threadServerIsRunning;
char readingBuffer[4096];
string databaseName;
InfluxDbInterface *influxDbService;

//#define DEBUG "dummy"

typedef struct {
    int socket;
    struct sockaddr clientAddress;
    int addressLength;
    char* clientName;
    char* clientPort;
} clientConnection;

MultiThreadServer::MultiThreadServer(int portToBind, string dbUrl, string dbPort, string database) {
    serverPort = portToBind;
    threadServerIsRunning = true;
    influxDbService = new InfluxDbInterface(dbUrl, dbPort, database);
    databaseName = database;
}

void *processClientCall(void *connectionStruct) {
    // reset buffer
    memset(readingBuffer, 0, 4096);

    clientConnection *connection;

    // kill the thread if no client connection was given
    if (!connectionStruct) {
        pthread_exit(0);
    }
    connection = (clientConnection *) connectionStruct;

    // receive data
    int bytesReceived = recv(connection->socket, readingBuffer, 4096, 0);

    // some went wrong
    if (bytesReceived == -1) {
        cerr << "Error during data receiving" << endl;
    }

    // data receiving finished
    if (bytesReceived == 0) {
        cout << "Data transfer completed" << endl;
    }

    influxDbService->storeInfinibandInDatabase(databaseName, readingBuffer, connection->clientName, connection->clientPort);

    // Send client success message
    string successMessage = "Data transmitted";
    send(connection->socket, successMessage.c_str(), successMessage.size() + 1, 0);

    // close socket and kill thread
    close(connection->socket);
    free(connection);
    pthread_exit(0);
}

void MultiThreadServer::run() {
    clientConnection *connection;
    pthread_t thread;

    // socket creation
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Socket can't be created" << endl;
        exit(EXIT_FAILURE);
    }

    // set address informations to server info struct
    sockaddr_in serverSocketAddressInformation{};
    serverSocketAddressInformation.sin_family = AF_INET;
    serverSocketAddressInformation.sin_port = htons(serverPort);
    serverSocketAddressInformation.sin_addr.s_addr = INADDR_ANY;
    // make it binary
    //inet_pton(AF_INET, serverAddress, &serverSocketAddressInformation.sin_addr);
    // bind it to socket
    bind(serverSocket, (sockaddr *) &serverSocketAddressInformation, sizeof(serverSocketAddressInformation));

    // let the socket listen for connections
    int numberOfAcceptedConnections = SOMAXCONN;
    listen(serverSocket, numberOfAcceptedConnections);

    // we will save some infos about connecting client
    char clientName[NI_MAXHOST];
    char clientPort[NI_MAXSERV];
    memset(clientName, 0, NI_MAXHOST);
    memset(clientPort, 0, NI_MAXSERV);

    // wait for connections
    while (threadServerIsRunning) {
        // set address informations to client info struct
        sockaddr_in clientSocketAddressInformation;
        socklen_t clientSocketSize = sizeof(clientSocketAddressInformation);

        // accept new connections and malloc space for new struct
        connection = (clientConnection *) malloc(sizeof(clientConnection));
        connection->socket = accept(serverSocket, (sockaddr *) &clientSocketAddressInformation, &clientSocketSize);

        // write infos to vars
        getnameinfo(
                (sockaddr *) &clientSocketAddressInformation,
                sizeof(clientSocketAddressInformation),
                clientName,
                NI_MAXHOST,
                clientPort,
                NI_MAXSERV,
                0
        );
        connection->clientName = clientName;
        connection->clientPort = clientPort;

        // connection failed => free struct
        if (connection->socket <= 0) {
            free(connection);
        }
        // connection was successfully created
        else {
            // start a new thread and listen instantly for new connections
            pthread_create(&thread, 0, processClientCall, (void *) connection);
            pthread_detach(thread);
        }
    }
}

void MultiThreadServer::stop() {
    threadServerIsRunning = false;
    std::cout << "Server stopped" << std::endl;
}


