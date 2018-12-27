#include <iostream>
#include <list>
#include "Maps.h"
#include "Lexer.h"
#include "Parser.h"
#include <mutex>
#include <thread>
#include "SimulatorCommunication.h"

using namespace std;


int main(int argc, char *argv[]) {
    vector<string> *tokens = Lexer::lexer(argv[1]);
    Parser parser(tokens);
    parser.parse();
    SimulatorCommunication::stop = true;
    /*the boolean indicates whether or not we entered the other thread during
     *the runtime of the program, thus indicating if we should close it at all*/
    if (SimulatorCommunication::startedReceivingData) {
        SimulatorCommunication::receiveDataThread.join();
    }
    close(SimulatorCommunication::serverSocketfd);
    close(SimulatorCommunication::clientSocketfd);

    /* freeing the memory allocated on the heap. Note: other deletions were
     * already performed in Parser */
    delete tokens;
    unordered_map<string, Number *>::iterator iter = Maps::symbolTable.begin();
    while (iter != Maps::symbolTable.end()) {
        delete iter->second;
        iter++;
    }
    return 0;
}