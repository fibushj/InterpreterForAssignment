
#include "Lexer.h"
#include <fstream>


vector<string> *Lexer::lexer(string fileName) {
    if (!ifstream(fileName)) {
        throw "Can't perform operation";
    }
    ifstream myFile;
    myFile.open(fileName);
    string line;
    vector<string> *tokens = new vector<string>;
    while (getline(myFile, line, END_LINE)) {
        tokenizeSingleLine(tokens, line);
    }

    return tokens;
}

void Lexer::tokenizeSingleLine(vector<string> *&tokens, string &line) {
    string currToken;
    char currChar;
    int index = 0;
    int lineLength = line.length();
    bool isInsideQuotation = false;
    while (index <= lineLength - 1) {
        currChar = line[index];
        if (currChar == SPACE || currChar == TAB) {
            if (currToken != "") {
                /* we split if the current character isn't preceded or
                 * followed by an operator or by Parantheses (we don't need a
                 * similar method like isNextCharOperator for skipping spaces/
                 * tabs in the opposite direction, since in that case the
                 * current token is empty, thus we won't enter the above if */
                if (!(isInsideQuotation || isOperator(line[index - 1]) ||
                      line[index - 1] == OPEN_PARENTHIS ||
                      isNextCharOperatorOrP(line, index))) {
                    tokens->push_back(currToken);
                    currToken = "";
                } else {
                    currToken += currChar;
                }
            }
            /*if the current character is comma, it indicates that it separates
             * between two expressions (since otherwise it's ambiguous) */
        } else if (currChar == COMMA) {
            if (currToken != "") {
                tokens->push_back(currToken);
                currToken = "";
            }
            index++;
            while (line[index] != MINUS) {
                index++;
            }
            currToken += MINUS;
            /* checking the following in order to prevent splitting at those
             * cases */
        } else if (currChar == ASSIGNMENT || currChar == GREATER ||
                   currChar == SMALLER || currChar == NOT) {
            if (currToken != "") {
                tokens->push_back(currToken);
            }
            currToken = currChar;
            if (line[index + 1] == '=') {
                currToken += line[index + 1];
                index++;
            }
            tokens->push_back(currToken);
            currToken = "";
            //in any other case, concatenating the current char to currToken
        } else {
            currToken += currChar;
        }
        //in order to not split a token which contains spaces/tabs
        if (currChar == QUOTATION_MARK) {
            isInsideQuotation = !isInsideQuotation;
        }
        index++;
    }
    if (currToken != "") {
        tokens->push_back(currToken);
    }
}

bool Lexer::isOperator(char c) {
    return c == PLUS || c == MINUS || c == MULT || c == DIV;
}

bool Lexer::isNextCharOperatorOrP(string &line, int index) {
    while (index < line.length() &&
           (line[index] == SPACE || line[index] == TAB)) {
        index++;
    }
    if (index == line.length() ||
        !(isOperator(line[index]) || line[index] == CLOSED_PARENTHIS)) {
        return false;
    }
    return true;
}
