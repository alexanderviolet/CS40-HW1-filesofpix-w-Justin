/*
 * Justin and Alex V
 * September 5, 2025
 * CS40: Lab 1
 */
#include "rawness.h"

// #define T Pnmrdr_T

// void tryOpeningFile(int argc, char *argv[]);

int main(int argc, char *argv[])
{
        tryOpeningFile(argc, argv);

        extern T Pnmrdr_new(FILE * fp);
        extern Pnmrdr_mapdata Pnmrdr_data(T rdr);

        return 0;
}

/*
 * tryOpeningFile
 * for parts 6 - 8 we just wanted to see how to open a file in c before getting
 * to the pnmrdr stuff.
 */
void tryOpeningFile(int argc, char *argv[])
{
        /* Check to see if you have enough arguments*/
        printf("The number of arguments %d\n", argc);
        if (argc <= 1)
        {
                fprintf(stderr, "Error: Too few arguments\n");
                exit(EXIT_FAILURE);
        }

        /* Try to open a file */
        FILE *fptr;
        fptr = fopen(argv[1], "r");

        /* Error check to ensure file works */
        /* TODO */

        /* Check to see if file opened SHOULD FAIL */
        if (fptr == NULL)
        {
                printf("The file did not open!\n");
        }
        else
        {
                printf("The file %s was opened!\n", argv[1]);
        }

        fclose(fptr);
}