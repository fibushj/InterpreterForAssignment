
#include "ShuntingYard.h"
#include "Number.h"
#include "Maps.h"
#include "Plus.h"
#include "Minus.h"
#include "Mult.h"
#include "Div.h"
#include <bits/stdc++.h>
#include "DefineCollection.h"
#include "SimulatorCommunication.h"

// Function that finds precedence of operators
int ShuntingYard::precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

// Function that returns value of expression
double ShuntingYard::evaluate(string tokens) {
    int count = 0;
    int x = 0;
    string var;
    // a stack to store doubles values.
    stack<double> values;
    // a stack to store operators.
    stack<char> ops;
    int i = 0;
    while (i < tokens.length()) {
        if (tokens[i] == UNDERLINE || isalpha(tokens[i])) {
            var = "";
            while (i < tokens.length() &&
                   (isalpha(tokens[i]) || tokens[i] == UNDERLINE ||
                    isdigit(tokens[i]))) {
                var += tokens[i];
                i++;
            }
            double value = currentValueOfVar(var);
            values.push(value);
            continue;
        }
        // Current token is a whitespace or tab, thus continue
        if (tokens[i] == ' ' || tokens[i] == '\t') {
            i++;
            continue;
        }
            // Current token is an opening brace, push it to 'ops'
        else if (tokens[i] == '(') {
            ops.push(tokens[i]);
        } else if (isdigit(tokens[i])) {
            double val = 0;
            int isFrac = 0;
            double fracion = 1;
            // if there are some digits in the number
            while ((i < tokens.length()) &&
                   ((isdigit(tokens[i])) || (tokens[i] == '.'))) {
                if (tokens[i] == '.') {
                    isFrac = 1;
                } else if (isFrac) {
                    fracion = fracion / 10;
                    val = val + (fracion * (tokens[i] - '0'));
                } else {
                    val = (val * 10) + (tokens[i] - '0');
                }
                i++;
            }
            i--;
            values.push(val);
        }

            // Closing brace encountered
        else if (tokens[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOperator(val1, val2, op));
            }

            // pop opening brace.
            ops.pop();
        }

            // Current token is an operator.
        else {
            for (int j = 0; j < tokens.length(); j++) {
                if (tokens[j] == '-' || tokens[j] == '+' || tokens[j] == '*' ||
                    tokens[j] == '/') {
                    count++;
                }
            }
            while ((!ops.empty() && precedence(ops.top())
                                    >= precedence(tokens[i])) ||
                   (tokens[i] == MINUS && isCurrMinusUnary(tokens, i - 1))) {
                if (isCurrMinusUnary(tokens, i - 1)) {
                    values.push(0);
                    break;
                }
                double val1 = 0, val2 = 0;

                char op = ops.top();
                ops.pop();

                val2 = values.top();
                values.pop();

                if (op != '-' || !values.empty()) {
                    val1 = values.top();
                    values.pop();
                }

                values.push(applyOperator(val1, val2, op));
            }
            // Push current token to 'ops'.
            ops.push(tokens[i]);
        }
        i++;
    }

    while (!ops.empty()) {
        if (values.size() < 2) {
            return 0;
        }
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOperator(val1, val2, op));
    }
    double result = values.top();
    values.pop();
    // return the top of 'values' that contains the result
    return result;
}

// Function that performs the +,=,*,/ operations
double ShuntingYard::applyOperator(double a, double b, char op) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            throw "no operation to evaluate";
    }

}


bool ShuntingYard::isCurrMinusUnary(string &str, int index) {
    while (index >= 0 && (str[index] == SPACE || str[index] == TAB)) {
        index--;
    }
    if (index < 0) {
        return true;
    }
    if (str[index] == PLUS || str[index] == MINUS || str[index] == MULT ||
        str[index] == DIV || str[index] == '(') {
        return true;
    }
    return false;
}

double ShuntingYard::currentValueOfVar(string var) {
    double currValue;
    //if the variable isn't bound to a path
    if (Maps::boundVars.count(var)) {
        SimulatorCommunication::m.lock();
        string &pathInSimulator = Maps::boundVars.at(var);
        currValue = Maps::rawValues[pathInSimulator];
        SimulatorCommunication::m.unlock();
    } else {
        currValue = Maps::symbolTable[var]->Calculate();
    }
    return currValue;
}