
#include "SimulatorCommunication.h"
#include "DefineCollection.h"
#include "Maps.h"
#include <iostream>
#include <cstring>

using namespace std;

int SimulatorCommunication::serverSocketfd;
int SimulatorCommunication::clientSocketfd;
bool SimulatorCommunication::stop;
bool SimulatorCommunication::startedReceivingData;
mutex SimulatorCommunication::m;
thread SimulatorCommunication::receiveDataThread;

//This function will be run in a separate thread
void SimulatorCommunication::receiveDataFromSimulator(int hz) {
    char buffer[BUFFER_SIZE];
    string message;
    std::chrono::duration<double> elapsed_seconds;
    //the total time to sleep based on the desired frequencey
    std::chrono::duration<double> totalTimeToSleep(1.0 / hz);
    int sizeRead;
    while (!stop) {
        auto start = std::chrono::system_clock::now();
        memset((char *) &buffer, 0, sizeof(buffer));
        sizeRead = read(serverSocketfd, buffer, BUFFER_SIZE);
        if (sizeRead < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        if (sizeRead == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        } else {
            m.lock();
            message = std::string(buffer);
            updateRawValuesMap(message);
            startedReceivingData = true;
            auto end = std::chrono::system_clock::now();
            elapsed_seconds = totalTimeToSleep - (end - start);
            m.unlock();
            std::this_thread::sleep_for(totalTimeToSleep);
        }
    }
}

void SimulatorCommunication::updateRawValuesMap(string message) {
    int index = 0;
    //indicates the number of values encountered so far (out of the 23)
    int countValues = 0;
    string currValue, currPath;
    while (message[index] != END_LINE) {
        if (message[index] != COMMA) {
            currValue += message[index];
        } else {
            currPath = Maps::allPaths.at(countValues);
            Maps::rawValues[currPath] = std::stod(currValue);
            currValue = "";
            countValues++;
        }
        index++;
    }
    //treating the value preceding the \n
    currPath = Maps::allPaths.at(countValues);
    Maps::rawValues[currPath] = std::stod(currValue);
}

//This function will be run in the main thread
void SimulatorCommunication::sendSetCommand(string str) {
    const char *message = str.c_str();
    int n=write(clientSocketfd, message, strlen(message));
    if (n<0) {
        perror("ERROR writing to socket");
        exit(1);
    }
}
