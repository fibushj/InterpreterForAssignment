//
// Created by gadit on 12/18/18.
//
#include <string>
#include <algorithm>    // std::find
#include <vector>       //  std::vector
#include <unordered_map>
#include "Command.h"
#include "Parser.h"
#include "SimulatorCommunication.h"

using namespace std;

Parser::Parser(vector<string> *&tokens) : tokens(tokens) {}

void Parser::parse() {

    vector<string>::iterator tokensIterator = tokens->begin();
    createCommandObjects(tokensIterator);
    string currCommand;
    while (tokensIterator != tokens->end()) {
        /* if the current token doesn't exist as a key in the mapCommand it's a
         * variable (we can assume correctness of the input as we were told */
        if (!mapCommand.count(*tokensIterator)) {
            tokensIterator++;
        }
        currCommand = *tokensIterator;
        Command *c = mapCommand.at(currCommand);
        c->doCommand();
    }
    //freeing the memory allocated on the heap
    unordered_map<string, Command *>::iterator iter = mapCommand.begin();
    while (iter != mapCommand.end()) {
        delete iter->second;
        iter++;
    }
}

void Parser::createCommandObjects(vector<string>::iterator &tokensIterator) {

    mapCommand.insert(
            pair<string, Command *>("openDataServer",
                                    new OpenServerCommand(tokensIterator)));
    mapCommand.insert(
            pair<string, Command *>("connect",
                                    new ConnectCommand(tokensIterator)));

    mapCommand.insert(
            pair<string, Command *>("var",
                                    new DefineVarCommand(tokensIterator)));

    mapCommand.insert(
            pair<string, Command *>("if",
                                    new IfCommand(tokensIterator, mapCommand)));
    mapCommand.insert(
            pair<string, Command *>("while",
                                    new LoopCommand(tokensIterator,
                                                    mapCommand)));
    mapCommand.insert(
            pair<string, Command *>("print",
                                    new PrintCommand(tokensIterator)));
    mapCommand.insert(
            pair<string, Command *>("sleep",
                                    new SleepCommand(tokensIterator)));
    mapCommand.insert(
            pair<string, Command *>("=",
                                    new AssignmentCommand(tokensIterator)));
}