
#include <string>

#ifndef UNTITLED5_NUMBER_H
#define UNTITLED5_NUMBER_H

#include "Expression.h"

class Number : public Expression {
private:
    double num;
public:
    Number(double num) : num(num) {}

    double Calculate() {
        return num;
    }

    void set(double num) {
        this->num = num;
    }
};

#endif //UNTITLED5_NUMBER_H
