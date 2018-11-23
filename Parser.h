//
// Created by toozi on 11/23/2018.
//

#include <stdio.h>
#ifndef EX3_PARSER_H
#define EX3_PARSER_H

// bullet's type
#define NUMBER 0
#define OPERATOR 1
#define LEFT_PARENTHESIS 2

// math operator
#define  ADDITION 0
#define MINUS 1
#define MULTIPLICATION 2
#define DIVISION 3
#define POWER 4




typedef struct Bullet
{
    int type;
    int data;
} Bullet;


int parseInfix(char *infix, Bullet *bullets[100], size_t *numOfBullets);
void printpostfix(Bullet *bullets[100], size_t numOfBullets);

#endif //EX3_PARSER_H
