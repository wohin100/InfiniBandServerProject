#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <csignal>
#include <InfluxDB.h>
#include <InfluxDBFactory.h>

using namespace std;
int port = 80;
char* serverAddress;

bool isRunning = true;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 1000; ++i) {
        // Provide complete URI
        auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086/?db=infiniband");

        influxdb->write(influxdb::Point{"test"}
                                .addField("value", i)
                                .addTag("host", "localhost")
        );
        printf("Done\n");
    }


    // check params
    if(argc < 3) {
        printf("Usage: ./InfiniBandServer <serverAddress> <port>\n");
        exit(EXIT_FAILURE);
    }else{
        serverAddress = argv[1];
        port = stoi(argv[2]);
    }

    // socket creation
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Socket can't be created" << endl;
        return -1;
    }

    // set address informations to server info struct
    sockaddr_in serverSocketAddressInformation;
    serverSocketAddressInformation.sin_family = AF_INET;
    serverSocketAddressInformation.sin_port = htons(port);
    serverSocketAddressInformation.sin_addr.s_addr = INADDR_ANY;
    // make it binary
    //inet_pton(AF_INET, serverAddress, &serverSocketAddressInformation.sin_addr);
    // bind it to socket
    bind(serverSocket, (sockaddr*)&serverSocketAddressInformation, sizeof(serverSocketAddressInformation));

    // let the socket listen for connections
    int numberOfAcceptedConnections = SOMAXCONN;
    listen(serverSocket, numberOfAcceptedConnections);

    // run till stop signal arrives
    cout << "Waiting for input..." << endl;
    while(isRunning) {
        cout << "..." << endl;

        // set address informations to client info struct
        sockaddr_in clientSocketAddressInformation;
        socklen_t clientSocketSize = sizeof(clientSocketAddressInformation);

        // create client socket
        int clientSocket = accept(serverSocket, (sockaddr *) &clientSocketAddressInformation, &clientSocketSize);

        // we will save some infos about connecting client
        char clientName[NI_MAXHOST];
        char clientPort[NI_MAXSERV];
        memset(clientName, 0, NI_MAXHOST);
        memset(clientPort, 0, NI_MAXSERV);

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
        cout << clientName << " connected using port " << clientPort << endl;

        // buffer received data
        char buffer[4096];

        while (true) {
            // reset buffer
            memset(buffer, 0, 4096);

            // receive data
            int bytesReceived = recv(clientSocket, buffer, 4096, 0);

            // some went wrong
            if (bytesReceived == -1) {
                cerr << "Error during data receiving" << endl;
                break;
            }

            // data receiving finished
            if (bytesReceived == 0) {
                cout << "Data transfer completed" << endl;
                break;
            }

            // TODO first simply print data => in future we will parse and write it to DB
            cout << string(buffer, 0, bytesReceived) << endl;

            // Send client success message
            string successMessage = "Data transmitted";
            send(clientSocket, successMessage.c_str(), successMessage.size() + 1, 0);
        }
        close(clientSocket);
    }

    // close server socket
    close(serverSocket);
    return 0;
}