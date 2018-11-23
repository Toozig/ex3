#include <stdio.h>
#include "Parser.h"

int main()
{
    Bullet *bullets[100];
    size_t numOfBullets = 0;
    parseInfix("17^2", bullets, &numOfBullets);
   printpostfix(bullets,numOfBullets);

}