//
// Created by toozi on 11/23/2018.
//

#include "Parser.h"
#include <stdio.h>
#include <mem.h>
#include <ctype.h>
#include "stack.h"

#define GROW_FACTOR 10

size_t ParseNumber(const char *infix, Bullet *const *bullets, size_t *numOfBullets, size_t i);

size_t *leftParenthesisFunc(Bullet **bullets, size_t *numOfBullets, const Stack *stack);

Bullet *createBullet(int data, int type)
{
    Bullet *bullet = (Bullet *) malloc(sizeof(Bullet));
    if (bullet == NULL)
    {
        fprintf(stderr, "ERROR memory alloc problem");
        return NULL;
    }
    bullet->data = data;
    bullet->type = type;
    return bullet;

}

Bullet *createOperator(char operator)
{
    int data;
    switch (operator)
    {
        case '+':
            data = ADDITION;
            break;
        case '-':
            data = MINUS;
            break;
        case '/':
            data = DIVISION;
            break;
        case '^':
            data = POWER;
            break;
        case '*':
            data = MULTIPLICATION;
            break;
        default:
            fprintf(stderr, "ERROR Invalid input");
            return NULL;
    }
    return createBullet(data, OPERATOR);
}

int ParseInfix(char *infix, Bullet **bullets, size_t *numOfBullets)
{
    Stack *stack = stackAlloc(sizeof(Bullet));
    if (stack == NULL)
    {
        fprintf(stderr, "ERROR memory alloc problem");
        return 1;
    }
    size_t i;
    for (i = 0; i < strlen(infix); i++)
    {
        if (infix[i] == ' ')
        {
            continue;
        }
        else if (isdigit(infix[i]))
        {
            i = ParseNumber(infix, bullets, numOfBullets, i);
        }
        else if (infix[i] == '(')
        {
            Bullet *bullet = createBullet(0, RIGHT_PARENTHESIS);
            if (bullet == NULL)
            {
                return 1;
            }
            push(stack, bullet);
        }
        else if ( infix[i] == ')')
        {
            numOfBullets = leftParenthesisFunc(bullets, numOfBullets, stack);
        }
        else
        {
            Bullet *bullet = createOperator(infix[i]);
            if( bullet == NULL)
            {
                return 0;
            }
            if (isEmptyStack(stack))
            {
                push(stack,bullet);
            }
            Bullet *topElement =  stack->_top->_data;
            if(topElement->type == LEFT_PARENTHESIS)
            {
                push(stack,bullet);
            } else
            {
                while (!isEmptyStack(stack) && topElement->type != '(' ) //todo another condition that i did not understand
            }

        }



    }
}

size_t *leftParenthesisFunc(Bullet **bullets, size_t *numOfBullets, const Stack *stack)
{
    while(isEmptyStack(stack))
            {
                Bullet *bullet = pop(stack);
                if(bullet->data == '(')
                {
                    free(bullet);
                    break;
                }
                bullets[*numOfBullets] = bullet;
                numOfBullets++;
            }
    return numOfBullets;
}

size_t ParseNumber(const char *infix, Bullet *const *bullets, size_t *numOfBullets, size_t i)
{
    int num;
    sscanf(&infix[i], "%d", &num);
    int add;
    while (isdigit(infix[i + 1]))
    {
        i++;
        sscanf(&infix[i], "%d", &add);
        num *= GROW_FACTOR;
        num += add;
    }
    Bullet *bullet = createBullet(num, NUMBER);
    if (bullet == NULL)
    {

    }
    bullets[*numOfBullets] = bullet;
    numOfBullets++;
    return i;
}
