//
// Created by toozi on 11/23/2018.
//

#include "Parser.h"
#include <stdio.h>
#include <mem.h>
#include <ctype.h>
#include "stack.h"

#define GROW_FACTOR 10

/**
 * This function create the struct which is the format for the postfix array
 * @param data  - the data of the struct
 * @param type  what kind of bullet this is- number/ operator etc..
 * @return
 */
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

/**
 * This function create Operator struct for the postfix array
 * @param operator  the char representing the math operator
 * @return  struct to be added to the stack / postfix array
 */
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

/**
 *  this function adds number to the postfix array.
 */
int ParseNumber(const char *infix, Bullet *bullets[100], const size_t numOfBullets, size_t *i)
{
    int num;
    sscanf(&infix[*i], "%d", &num);
    while ( *i + 1 < strlen(infix) && isdigit(infix[*i + 1]))
    {
        *i += 1;
    }
    Bullet *bullet = createBullet(num, NUMBER);
    if (bullet == NULL)
    {
        return 1;
    }
    bullets[numOfBullets] = bullet;

    return 0;
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
                if (top->data == MINUS || top->data == ADDITION)
                {
                    return 0;
                }
                break;
            case POWER:
                if (top->data != POWER)
                {
                    return 0;
                }
            default:
                break;
        }
    }
    return 1;
}

/**
 * this function adds operator to the postfix array
 */
size_t *addOperator(Bullet *bullets[100], size_t *numOfBullets, Stack *stack, Bullet *bullet)
{
    if (isEmptyStack(stack))
    {
        push(stack, bullet);
        return numOfBullets;
    }
    Bullet *topElement = stack->_top->_data;
    if (topElement->type == LEFT_PARENTHESIS)
    {
        push(stack, bullet);
    } else
    {
        while (!isEmptyStack(stack) &&
               topElement->type != LEFT_PARENTHESIS &&
               precedenceIsGreater(bullet, topElement)) //todo check if this function work
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
size_t *leftParenthesisFunc(Bullet *bullets[100], size_t *numOfBullets, Stack *stack)
{
    while (!isEmptyStack(stack))
    {
        Bullet *bullet = pop(stack);
        if (bullet->type == LEFT_PARENTHESIS)
        {
            free(bullet);
            break;
        }
        bullets[*numOfBullets] = bullet;
        *numOfBullets += 1;
    }
    return numOfBullets;
}


/**
 * This function turns the infix into postfix
 * @param infix the infix string
 * @param bullets the postfix array
 * @param numOfBullets the number of elements in the array
 * @return 0 if all did right, 1 otherwise
 */
int parseInfix(char *infix, Bullet *bullets[100], size_t *numOfBullets)
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
            if (ParseNumber(infix, bullets, *numOfBullets, &i))
            {
                return 1;
            }
            *numOfBullets += 1;
        } else if (infix[i] == ' ')
        {
            continue;
        } else if (infix[i] == '(')
        {
            Bullet *bullet = createBullet(0, LEFT_PARENTHESIS);
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
                return 1;
            }
            numOfBullets = addOperator(bullets, numOfBullets, stack, bullet);
        }
    }
    while (!isEmptyStack(stack))
    {
        Bullet *bullet = pop(stack);
        if (bullet->type == LEFT_PARENTHESIS)
        {
            fprintf(stderr, "ERROR input format error");
            return 1;
        }
        bullets[*numOfBullets] = bullet;
        *numOfBullets += 1;
    }
    free(stack);
    return 0;
}

void printpostfix(Bullet* bullets[100], size_t numOfBullets)
{
    size_t i;
    for (i = 0; i < numOfBullets; ++i)
    {
        if (bullets[i]->type == NUMBER)
        {
            printf("%d ", bullets[i]->data);
        }
        else
        {
            switch(bullets[i]->data)
            {
                case MINUS:
                    printf("%s ", "- ");
                    break;
                case ADDITION:
                    printf("%s ", "+ ");
                    break;
                case MULTIPLICATION:
                    printf("%s ", "* ");
                    break;
                case POWER:
                    printf("%s ", "^ ");
                    break;
                default:
                    printf("%s ", "/ ");
                    break;
            }
        }

    }
}