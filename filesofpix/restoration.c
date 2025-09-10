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
        FILE *dummyptr = testOpeningFile(argc, argv);

        char *data;

        int bytes = readaline(dummyptr, &data);

        printf("this is the number of bytes: %d\n", bytes);       

        fclose(dummyptr);
        
        printf("Trying to print in restoration:\n");
        printf("*data: %c\n", *data);
        
        
        free(data);
        
        return 0;
}

FILE *testOpeningFile(int argc, char *argv[]) 
{
        /* Ensure we have a provided file */
        if (argc < 2) {
                fprintf(stderr, "restoration requries an image!\n");
                fprintf(stderr, "\tTODO: determine correct error message\n");
                exit(EXIT_FAILURE);
        }

        FILE *fptr = fopen(argv[1], "r");

        /* Ensure file is actually open */
        if (fptr == NULL) {
                fprintf(stderr, "file could not be opened\n");
                fprintf(stderr, "\tTODO: should be checked runtime erorr\n");
                exit(EXIT_FAILURE);
        }

        return fptr;
}