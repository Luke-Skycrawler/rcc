#include <stdio.h>
int main()
{
    printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
           10 << 2,
           10 >> 2,
           10 % 7,
           10 ^ 7,
           10 | 7,
           10 || 7,
           10 & 7,
           7 <= 7,
           7 != 7,
           7 == 7,
           7 > 7,
           7 < 7
           );
}