
#ifndef UNTITLED5_SHUNTINGYARD_H
#define UNTITLED5_SHUNTINGYARD_H

#include "Expression.h"

#include <string>

using namespace std;

class ShuntingYard {
public:
    int precedence(char op);

    double applyOperator(double a, double b, char op);

    double evaluate(string tokens);

    bool isCurrMinusUnary(string &str, int index);

    double currentValueOfVar(string var);

};

#endif //UNTITLED5_SHUNTINGYARD_H