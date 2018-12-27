//
// Created by Jonathan on 25/12/2018.
//

#ifndef INTERPRETER_SIMULATORCOMMUNICATION_H
#define INTERPRETER_SIMULATORCOMMUNICATION_H

#ifdef _WIN32

#include "winsock2.h"

typedef int socklen_t;
#else
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <string>
#include <mutex>
#include <thread>

#define BUFFER_SIZE 1024

using namespace std;

class SimulatorCommunication {
public:
    static int serverSocketfd;
    static int clientSocketfd;
    static bool startedReceivingData;
    static bool stop;

    static thread receiveDataThread;
    static mutex m;

    static void receiveDataFromSimulator(int hz);

    static void updateRawValuesMap(string message);

    static void sendSetCommand(string message);
};


#endif //INTERPRETER_SIMULATORCOMMUNICATION_H
