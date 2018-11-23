//
// Created by toozi on 11/23/2018.
//

#ifndef EX3_PARSER_H
#define EX3_PARSER_H

#define NUMBER 0
#define OPERATOR 1
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

#endif //EX3_PARSER_H
