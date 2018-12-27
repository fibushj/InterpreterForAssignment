
#include "Expression.h"
#include "BinaryExpression.h"

class Mult : public BinaryExpression{
public:
    Mult(Expression* left, Expression* right): BinaryExpression(left,right){

    }
    double Calculate(){
        return left->Calculate() * right->Calculate();

    }
};
