//
// Created by master on 14.01.21.
//

#ifndef INFINIBANDSERVERPROJECT_MULTITHREADSERVER_H
#define INFINIBANDSERVERPROJECT_MULTITHREADSERVER_H


class MultiThreadServer {

public:
    MultiThreadServer(int portToBind, std::string url, std::string port, std::string database);

    void run();

    void stop();
};


#endif //INFINIBANDSERVERPROJECT_MULTITHREADSERVER_H
