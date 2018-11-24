//
// Created by toozi on 11/23/2018.
//

#include <stdio.h>
#ifndef EX3_PARSER_H
#define EX3_PARSER_H
#define MAX_CHAR_IN_LINE 100

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


int parseInfix(char *infix, Bullet *bullets[MAX_CHAR_IN_LINE], size_t *numOfBullets);
void printInfo(Bullet **bullets, size_t numOfBullets, char * infix);

#endif //EX3_PARSER_H
