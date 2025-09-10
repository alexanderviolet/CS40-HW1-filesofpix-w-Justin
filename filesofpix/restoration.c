/*
 *      restoration.c
 *      Justin and Alex, September 9, 2025
 *      filesofpix
 * 
 *      TODO: Summary
 */
#include "restoration.h"

int main(int argc, char *argv[])
{
        (void) argc;
        (void) argv;

        void *dummyptr = testOpeningFile(argc, argv);
        (void) dummyptr;

        int bytes = readaline(NULL, NULL);

        printf("this is the number of bytes: %d\n", bytes);       

        return 0;
}

FILE *testOpeningFile(int argc, char *argv[]) 
{
        (void) argv;
        char c;

        /* Ensure we have a provided file */
        if (argc < 2) {
                fprintf(stderr, "restoration requries an image!\n");
                fprintf(stderr, "\tTODO: determine correct error message\n");
                exit(EXIT_FAILURE);
        }

        printf("This should not print!\n");
        FILE *fptr = fopen(argv[1], "r");

        /* Ensure file is actually open */
        if (fptr == NULL) {
                fprintf(stderr, "file could not be opened\n");
                fprintf(stderr, "\tTODO: should be checked runtime erorr\n");
                exit(EXIT_FAILURE);
        }

        c = fgetc(fptr);
        printf("The first character of this file is: %c\n", c);

        fclose(fptr);

        return NULL;
}