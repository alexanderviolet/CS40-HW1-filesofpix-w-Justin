/*
 * Justin and Alex messing around trying to get all ASCII (we are programmers)
 */

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
        int c = 'A';
        for (int i = 0; i < 26; i++)
        {
                printf("%d ", c);
                c++;
        }
        printf("\n");
        c = 'a';
        for (int i = 0; i < 26; i++)
        {
                printf("%d ", c);
                c++;
        }
        printf("\n");
        
}