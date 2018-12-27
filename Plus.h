
#include "Expression.h"
#include "BinaryExpression.h"
#include <stdio.h>

class Plus : public BinaryExpression {
public:
    Plus(Expression *left, Expression *right) : BinaryExpression(left, right) {}

    double Calculate() {
        return left->Calculate() + right->Calculate();

    }
};
