//
// Created by toozi on 11/24/2018.
//
#include <stdlib.h>
#include "postfixEvaluation.h"
#include "Parser.h"
#include "stack.h"
#include <math.h>


void calculate(Bullet *a, Bullet *b, int operator)
{

    int data;
    switch (operator)
    {
        case MINUS:
            data =  b->data - a->data;
            break;
        case ADDITION:
            data = b->data + a->data;
            break;
        case MULTIPLICATION:
            data  = b->data * a->data;
            break;
        case DIVISION:
            data = b->data / a->data;
            break;
        case POWER:
            data = (int) pow(b->data, a->data);
            break;
        default:
            return;
    }
    a->data = data;
}

int evaluatePostFix(Bullet* const bullets[MAX_CHAR_IN_LINE], size_t const numOfBullets)
{
    Stack *stack = stackAlloc(sizeof(Bullet*));
    size_t i;
    Bullet *a;
    Bullet *b;
    for ( i = 0; i < numOfBullets ; ++i)
    {
        switch (bullets[i]->type)
        {
            case NUMBER:
                push(stack,(bullets[i]));
                break;
            case OPERATOR:
                a = pop(stack);
                b = pop(stack);
                if ( a == NULL || b == NULL)
                {
                    return 1;
                }
                calculate(a, b,(int) bullets[i]->data);
                push(stack, a);
                break;
        }
    }
    a = pop(stack);
    printf("%lf", a->data );
    return 0;
}
