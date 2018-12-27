
#include "Number.h"
#include "Command.h"
#include "iostream"
#include "ShuntingYard.h"
#include "DefineCollection.h"
#include "Maps.h"
#include "SimulatorCommunication.h"
#include <cstring>

void OpenServerCommand::doCommand() {
    tokensIterator++;
    ShuntingYard s;
    int portNumber = (int) s.evaluate(*tokensIterator);
    tokensIterator++;
    int hz = (int) s.evaluate(*tokensIterator);
    establishConnection(portNumber, hz);
    tokensIterator++;
}

void OpenServerCommand::establishConnection(int portNumber, int hz) {
    SimulatorCommunication::startedReceivingData = false;
    SimulatorCommunication::stop = false;
    int sockfd, newsockfd, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNumber);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 1);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
                       (socklen_t *) &clilen);
    close(sockfd);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }
    SimulatorCommunication::serverSocketfd = newsockfd;
    //   SimulatorCommunication::receiveDataFromSimulator(hz);
    SimulatorCommunication::receiveDataThread = thread(
            SimulatorCommunication::receiveDataFromSimulator, hz);
    while (!SimulatorCommunication::startedReceivingData) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void ConnectCommand::doCommand() {
    tokensIterator++;
    ShuntingYard s;
    string ip = *tokensIterator;
    tokensIterator++;
    int portNumber = (int) s.evaluate(*tokensIterator);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    connectToSimulator(ip, portNumber);
    tokensIterator++;
}

void ConnectCommand::connectToSimulator(string ip, int portNumber) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(ip.c_str());

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr,
           server->h_length);
    serv_addr.sin_port = htons(portNumber);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <
        0) {
        perror("ERROR connecting");
        exit(1);
    }
    SimulatorCommunication::clientSocketfd = sockfd;
}

bool ConditionParser::isConditionSatisfied() {
    tokensIterator++;
    ShuntingYard s;
    int val1 = s.evaluate(*tokensIterator);
    tokensIterator++;
    string op = (*tokensIterator);
    tokensIterator++;
    int val2 = s.evaluate(*tokensIterator);
    tokensIterator++;
    if (op == "==") {
        return val1 == val2;
    }
    if (op == "!=") {
        return val1 != val2;
    }
    if (op == ">") {
        return val1 > val2;
    }
    if (op == ">=") {
        return val1 >= val2;
    }
    if (op == "<") {
        return val1 < val2;
    }
    if (op == "<=") {
        return val1 <= val2;
    }
    return false;
}

void IfCommand::doCommand() {
    if (isConditionSatisfied()) {
        /*it should be after summoning isConiditionSatisfied, but ensuring for
        safety */
        if (*tokensIterator == OPEN_BRACKET) {
            tokensIterator++;
        }
        while (*tokensIterator != CLOSED_BRACKET) {
            /* if the current token doesn't exist as a key in the mapCommand*/
            if (!mapCommand.count(*tokensIterator)) {
                tokensIterator++;
            }
            string currCommand = *tokensIterator;
            Command *c = mapCommand.at(currCommand);
            c->doCommand();
        }
        tokensIterator++;
    } else {
        skipIterator();
    }
}

void LoopCommand::doCommand() {
    /* an auxiliary iterator that stores the start of the condition, so at the
     * end of each iteration we'll jump back */
    vector<string>::iterator startIterator = tokensIterator;
    while (isConditionSatisfied()) {
        if (*tokensIterator == OPEN_BRACKET) {
            tokensIterator++;
        }
        while (*tokensIterator != CLOSED_BRACKET) {
            /* if the current token doesn't exist as a key in the mapCommand*/
            if (!mapCommand.count(*tokensIterator)) {
                tokensIterator++;
            }
            string currCommand = *tokensIterator;
            Command *c = mapCommand.at(currCommand);
            c->doCommand();
        }
        tokensIterator = startIterator;
    }
    skipIterator();
}

void ConditionParser::skipIterator() {
    int countOpenBrackets = 0;
    int countClosedBrackets = 0;
    while (!(*tokensIterator == CLOSED_BRACKET &&
             countOpenBrackets == countClosedBrackets + 1)) {
        if (*tokensIterator == OPEN_BRACKET) {
            countOpenBrackets++;
        } else if (*tokensIterator == CLOSED_BRACKET) {
            countClosedBrackets++;
        }
        tokensIterator++;
    }
    tokensIterator++;
}

void PrintCommand::doCommand() {
    tokensIterator++;
    string message;
    if ((*tokensIterator)[0] == QUOTATION_MARK) {
        message = (*tokensIterator).substr(1,
                                           (*tokensIterator).length() -
                                           2);
    } else {
        ShuntingYard s;
        message = to_string(s.evaluate(*tokensIterator));
    }
    cout << message << endl;
    tokensIterator++;
}

void SleepCommand::doCommand() {
    tokensIterator++;
    ShuntingYard s;
    unsigned int milliseconds = s.evaluate(*tokensIterator);
    tokensIterator++;
    // std::chrono::duration<double> duration(seconds);
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void DefineVarCommand::doCommand() {
    tokensIterator++;
    Maps::symbolTable.insert(
            pair<string, Number *>(*tokensIterator, new Number(0)));
    tokensIterator++;
}

void AssignmentCommand::doCommand() {
    string var = *(tokensIterator - 1);
    tokensIterator++;
    if (*tokensIterator == "bind") {
        tokensIterator++;
        /* if "bind" is followed by a path (indicated by the first character in
         * the current string being quotation mark) */
        if ((*tokensIterator)[0] == QUOTATION_MARK) {
            string path = (*tokensIterator).substr(1,
                                                   (*tokensIterator).length() -
                                                   2);
            Maps::boundVars.insert(pair<string, string>(var, path));
        }
            /* in this case, "bind" is followed by a variable, thus the original
             * variable (var) should be bound to the following one (bindToVar)*/
        else {
            string bindToVar = *tokensIterator;
            if (Maps::boundVars.count(bindToVar)) {
                Maps::boundVars[var] = Maps::boundVars[bindToVar];
            } else {
                Maps::symbolTable[var] = Maps::symbolTable[bindToVar];
            }
        }

    }
        //if we shouldn't perform a bind
    else {
        ShuntingYard s;
        double value = s.evaluate(*tokensIterator);
        //if it's an itialization of the variable
        if (*(tokensIterator - 3) == "var") {
            Maps::symbolTable[var]->set(value);
        }
            //if it's an update of the variable
        else {
            //if it's bound to a path
            if (Maps::boundVars.count(var)) {
                string path = Maps::boundVars[var];
                string message =
                        "set " + path + " " + to_string(value) + "\r\n";
                SimulatorCommunication::sendSetCommand(message);
                SimulatorCommunication::m.lock();
                Maps::rawValues[path] = value;
                SimulatorCommunication::m.unlock();
                /* otherwise, whether it's bound locally or not to a variable
                 * (that's neither it nor other variables it may be bound to is
                 * bind to a path), updating the value of the pointed
                 * corresponding Number */
            } else {
                Maps::symbolTable[var]->set(value);
            }
        }
    }
    tokensIterator++;
}


