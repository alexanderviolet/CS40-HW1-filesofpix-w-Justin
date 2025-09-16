#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
        (void) argc;
        (void) argv;

        char c;

        for (int i = 0; i < 255; i++)
        {
                c = i;
                printf("65%c", c);
        }
        printf("\n");

        for (int i = 0; i < 255; i++)
        {
                c = i;
                printf("65%c", c);
        }
        printf("\n");
        

        return 0;
}