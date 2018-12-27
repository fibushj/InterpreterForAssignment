

#include <unordered_map>
#include <list>
#include <string>
#include <vector>

using namespace std;


#ifndef UNTITLED5_COMMAND_H
#define UNTITLED5_COMMAND_H


class Command {
public:
    virtual void doCommand() = 0;

    virtual ~Command() {}
};


class OpenServerCommand : public Command {
private:
    vector<string>::iterator &tokensIterator;
public:
    OpenServerCommand(vector<string>::iterator &tokensIterator)
            : tokensIterator(tokensIterator) {}

    void doCommand();

    void establishConnection(int portNumber, int hz);
};


class ConnectCommand : public Command {
private:
    vector<string>::iterator &tokensIterator;
public:
    ConnectCommand(vector<string>::iterator &tokensIterator)
            : tokensIterator(tokensIterator) {}

    void doCommand();

    void connectToSimulator(string ip, int portNumber);
};

class DefineVarCommand : public Command {
private:
    vector<string>::iterator &tokensIterator;
public:
    DefineVarCommand(vector<string>::iterator &tokensIterator)
            : tokensIterator(tokensIterator) {}

    void doCommand();

};

class ConditionParser : public Command {
protected:
    vector<string>::iterator &tokensIterator;
    unordered_map<string, Command *> &mapCommand;
public:
    ConditionParser(vector<string>::iterator &tokensIterator,
                    unordered_map<string, Command *> &mapCommand)
            : tokensIterator(tokensIterator), mapCommand(mapCommand) {}

    bool isConditionSatisfied();

    void skipIterator();
};

class IfCommand : public ConditionParser {

public:
    IfCommand(vector<string>::iterator &tokensIterator,
              unordered_map<string, Command *> &mapCommand)
            : ConditionParser(tokensIterator, mapCommand) {}

    void doCommand();

};


class LoopCommand : public ConditionParser {

public:
    LoopCommand(vector<string>::iterator &tokensIterator,
                unordered_map<string, Command *> &mapCommand)
            : ConditionParser(tokensIterator, mapCommand) {}

    void doCommand();

};

class PrintCommand : public Command {
private:
    vector<string>::iterator &tokensIterator;
public:
    PrintCommand(vector<string>::iterator &tokensIterator)
            : tokensIterator(tokensIterator) {}

    void doCommand();

};

class SleepCommand : public Command {
private:
    vector<string>::iterator &tokensIterator;
public:
    SleepCommand(vector<string>::iterator &tokensIterator)
            : tokensIterator(tokensIterator) {}


    void doCommand();

};

class AssignmentCommand : public Command {
private:
    vector<string>::iterator &tokensIterator;
public:
    AssignmentCommand(vector<string>::iterator &tokensIterator)
            : tokensIterator(tokensIterator) {}


    void doCommand();
};


#endif //UNTITLED5_COMMAND_H