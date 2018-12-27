
#include "Expression.h"
#include "BinaryExpression.h"

class Minus : public BinaryExpression{
public:
    Minus(Expression* left, Expression* right): BinaryExpression(left,right){

    }
    double Calculate(){
        return left->Calculate() - right->Calculate();

    }
};
