
#ifndef UNTITLED5_LEXER_H
#define UNTITLED5_LEXER_H


#include <vector>
#include <string>

#include "DefineCollection.h"

using namespace std;

class Lexer {
public:
    static vector<string> *lexer(string fileName);

    static void tokenizeSingleLine(vector<string> *&tokens, string &line);

    static bool isOperator(char c);

    /* an auxiliary function that returns whether or not the next character
     * is operator or a parenthesis (ignoring spaces and tabs) */
    static bool isNextCharOperatorOrP(string &line, int index);
};


#endif //UNTITLED5_LEXER_H
