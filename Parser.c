//
// Created by toozi on 11/23/2018.
//

#include "Parser.h"
#include <stdio.h>
#include <mem.h>
#include <ctype.h>
#include "stack.h"

#define GROW_FACTOR 10

size_t ParseNumber(const char *infix, Bullet **bullets, size_t *numOfBullets, size_t i);

size_t *leftParenthesisFunc(Bullet **bullets, size_t *numOfBullets, Stack *stack);

size_t *addOperator(Bullet **bullets, size_t *numOfBullets, Stack *stack, Bullet *bullet);

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

size_t ParseNumber(const char *infix, Bullet **bullets, size_t *numOfBullets, size_t i)
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

    return i;
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
        if (isdigit(infix[i]))
        {
            i = ParseNumber(infix, bullets, numOfBullets, i);
            numOfBullets++;
        } else if (infix[i] == ' ')
        {
            continue;
        } else if (infix[i] == '(')
        {
            Bullet *bullet = createBullet(0, RIGHT_PARENTHESIS);
            if (bullet == NULL)
            {
                return 1;
            }
            push(stack, bullet);
        } else if (infix[i] == ')')
        {
            numOfBullets = leftParenthesisFunc(bullets, numOfBullets, stack);
        } else
        {
            Bullet *bullet = createOperator(infix[i]);
            if (bullet == NULL)
            {
                return 0;
            }
            numOfBullets = addOperator(bullets, numOfBullets, stack, bullet);
        }
    }
    while (!isEmptyStack(stack))
    {
        Bullet *bullet = pop(stack);
        bullets[*numOfBullets] = bullet;
        numOfBullets++;
    }
}

/**
 * This function checks if the precedence of the operator is bigger to the precedence of thetop elemnt
 * @param operator the operator
 * @param top the top element of the stack
 * @return  0 if the operator precedence is bigger than the top element. 1 otherwise
 */
int precedenceIsGreater(Bullet *operator, Bullet *top)
{
    if (operator->type == top->type)
    {
       switch (operator->data)
       {
           case MULTIPLICATION:
           case DIVISION:
               if(top->data == MINUS || top->data == ADDITION)
               {
                   return 0;
               }
               break;
           case POWER:
               if(top->data != POWER)
               {
                   return 0;
               }
           default:
               break;
       }
    }
    return 1;
}

size_t *addOperator(Bullet **bullets, size_t *numOfBullets, Stack *stack, Bullet *bullet)
{
    if (isEmptyStack(stack))
    {
        push(stack, bullet);
    }
    Bullet *topElement = stack->_top->_data;
    if (topElement->type == LEFT_PARENTHESIS)
    {
        push(stack, bullet);
    } else
    {
        while (!isEmptyStack(stack) &&
               topElement->type != '(' && precedenceIsGreater(bullet, topElement)) //todo check if this function work
        {
            Bullet *poped = pop(stack);
            bullets[*numOfBullets] = poped;
            numOfBullets++;
            topElement = stack->_top->_data;
        }
        push(stack, bullet);
    }
    return numOfBullets;
}

/**
 * This function preform the part of the algorithm if a left parenthesis is met
 */
size_t *leftParenthesisFunc(Bullet **bullets, size_t *numOfBullets, Stack *stack)
{
    while (isEmptyStack(stack))
    {
        Bullet *bullet = pop(stack);
        if (bullet->data == '(')
        {
            free(bullet);
            break;
        }
        bullets[*numOfBullets] = bullet;
        numOfBullets++;
    }
    return numOfBullets;
}

/**
 * this function parse a digit char in the given input
 */
