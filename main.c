#include <stdio.h>
#include <mem.h>
#include <malloc.h>
#include "Parser.h"
#include "postfixEvaluation.h"

int main()
{
    char  infix[MAX_CHAR_IN_LINE + 1];
    Bullet *bullets[MAX_CHAR_IN_LINE];
    size_t numOfBullets;
    while (fgets(infix, MAX_CHAR_IN_LINE + 1, stdin) != NULL)
    {
        numOfBullets = 0;
        if (parseInfix(infix, bullets, &numOfBullets))
        {
            return 1;
        }
        printInfo(bullets, numOfBullets, infix);
        evaluatePostFix(bullets, numOfBullets);
        for (int i = 0; i < numOfBullets; ++i)
        {
            free(bullets[i]);
        }
    }

}