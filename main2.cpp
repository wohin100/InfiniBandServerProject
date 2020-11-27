#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serverSocket, clientSocket, port, allowedConnections;
    socklen_t clientAddressLength;
    char buffer[256];
    struct sockaddr_in serverAddress, clientAddress;
    int executionResult;
    if (argc < 2) {
        fprintf(stderr,"Usage: ./InfiniBandServer <port>\n");
        exit(1);
    }
    else{
        port = atoi(argv[1]);
    }

    // create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        error("Socket can't be created");

    // initialize server address
    bzero((char *) &serverAddress, sizeof(serverAddress));

    // bind server to localhost
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    executionResult = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (executionResult < 0){
        error("ERROR on binding");
    }

    // let server listen for requests
    allowedConnections = 24;
    listen(serverSocket, allowedConnections);

    // create a clientSocket
    clientAddressLength = sizeof(clientAddress);

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            error("Can't interact with client");
        }

        // clear buffer
        bzero(buffer, 2048);

        // read data
        executionResult = read(clientSocket, buffer, 2047);
        if (executionResult < 0) {
            error("Can't read data");
        }

        // test output
        printf("Received data: %s\n", buffer);

        // return success
        executionResult = write(clientSocket, "Success", 7);
        if (executionResult < 0) {
            error("Return failed");
        }
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}