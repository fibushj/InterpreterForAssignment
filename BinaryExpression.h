
#include "Expression.h"

#ifndef UNTITLED5_BINARYEXPRESSION_H
#define UNTITLED5_BINARYEXPRESSION_H

class BinaryExpression {
protected:
    Expression *right;
    Expression *left;
public:
    BinaryExpression(Expression *left, Expression *right) {
        this->right = right;
        this->left = left;
    }

};

#endif //UNTITLED5_BINARYEXPRESSION_H
