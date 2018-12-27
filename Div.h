//
// Created by gadit on 12/19/18.
//

#include "Expression.h"
#include "BinaryExpression.h"
class Div : public BinaryExpression{
public:
    Div(Expression* left, Expression* right): BinaryExpression(left,right){

    }
    double Calculate(){
        return left->Calculate() / right->Calculate();
    }
};