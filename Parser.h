
#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H


#include <string>
#include <vector>
#include "Maps.h"
#include "Command.h"
using namespace std;

class Parser {
private:
    vector<string> *&tokens;
    unordered_map<string, Command *> mapCommand;
public:
    Parser(vector<string> *&tokens);

    void parse();

    void createCommandObjects(vector<string>::iterator &tokensIterator);

};

#endif //INTERPRETER_PARSER_H
