//
// Created by toozi on 11/23/2018.
//

#include "Parser.h"
#include <stdio.h>
#include <ctype.h>
#include <mem.h>
#include "stack.h"

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
int ParseNumber(const char *infix, Bullet *bullets[MAX_CHAR_IN_LINE], const size_t numOfBullets, size_t *i)
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
        switch ( (int) operator->data)
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
void addOperator(Bullet *bullets[MAX_CHAR_IN_LINE], size_t *numOfBullets, Stack *stack, Bullet *bullet)
{
    if (isEmptyStack(stack))
    {
        push(stack, bullet);
        return;
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
            *numOfBullets += 1;
            topElement = isEmptyStack(stack)? 0: stack->_top->_data;
        }
        push(stack, bullet);
    }
}

/**
 * This function preform the part of the algorithm if a left parenthesis is met
 */
int leftParenthesisFunc(Bullet *bullets[MAX_CHAR_IN_LINE], size_t *numOfBullets, Stack *stack)
{
    Bullet *bullet = pop(stack);
    while (bullet->type != LEFT_PARENTHESIS)
    {
        bullets[*numOfBullets] = bullet;
        *numOfBullets += 1;
        if (isEmptyStack(stack))
        {
            fprintf(stderr, "ERROR Input format error");
            return 1;
        }
        bullet = pop(stack);
    }
    return 0;
}


/**
 * This function turns the infix into postfix
 * @param infix the infix string
 * @param bullets the postfix array
 * @param numOfBullets the number of elements in the array
 * @return 0 if all did right, 1 otherwise
 */
int parseInfix(char *infix, Bullet *bullets[MAX_CHAR_IN_LINE], size_t *numOfBullets)
{
    Stack *stack = stackAlloc(sizeof(Bullet));
    if (stack == NULL)
    {
        fprintf(stderr, "ERROR memory alloc problem");
        return 1;
    }
    size_t i;
    Bullet *bullet;
    for (i = 0; i < strlen(infix) -1; i++)
    {
        if (isdigit(infix[i]))
        {
            if (ParseNumber(infix, bullets, *numOfBullets, &i))
            {
                return 1;
            }
            *numOfBullets += 1;
            continue;
        }
        switch (infix[i])
        {
            case ' ':
                continue;
            case '(':
                bullet = createBullet(0, LEFT_PARENTHESIS);
                if (bullet == NULL)
                {
                    return 1;
                }
                push(stack, bullet);
                break;
            case ')':
                if(leftParenthesisFunc(bullets, numOfBullets, stack))
                {
                    return 1;
                }
                break;
            default:
                bullet = createOperator(infix[i]);
                if (bullet == NULL)
                {
                    return 1;
                }
                addOperator(bullets, numOfBullets, stack, bullet);
        }
    }
    while (!isEmptyStack(stack))
    {
        bullet = pop(stack);
        if (bullet->type == LEFT_PARENTHESIS)
        {
            fprintf(stderr, "ERROR input format error");
            return 1;
        }
        bullets[*numOfBullets] = bullet;
        *numOfBullets += 1;
    }
    freeStack(&stack);
    return 0;
}

/**
 * This function prtins postfix calculation.
 * @param bullets
 * @param numOfBullets
 */
void printInfo(Bullet **bullets, size_t numOfBullets, char * infix)
{
    size_t i;
    printf("%s", "Infix: ");
    for(i =  0; i < strlen(infix) ; i++)
    {
        if(infix[i] != ' ')
        {
            printf(" %c", infix[i] );
        }
    }
    printf("%s", "Postfix: ");
    for (i = 0; i < numOfBullets; ++i)
    {
        if (bullets[i]->type == NUMBER)
        {
            printf("%d ", (int) bullets[i]->data);
        }
        else
        {
            switch((int) bullets[i]->data)
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
    printf("\n");
}