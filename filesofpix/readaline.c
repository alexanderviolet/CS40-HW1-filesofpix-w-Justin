/*
 *      readaline.c
 *      Justin and Alex, September 9, 2025
 *      filesofpix
 * 
 *      TODO: Summary
 */

#include "readaline.h"
#include <stdio.h>
#include <stdlib.h>

/********** readaline ********
 *
 * TODO: copy from spec
 *
 * Parameters:
 *      FILE *inputfd:  pointer to corrupted file from restoration
 *      char **datapp:  pass by reference pointer which will be set to the first
 *                      character of a line in the file
 *
 * Return: number of bytes in read line
 *
 * Expects
 *      inputfd and datapp must not be NULL
 * Notes:
 *      Will raise Checked Runtime Error if:
 *              1. Either or both of the supplied arguments is NULL
 *              2. An error is encountered reading from the file
 *              3. Memory allocation fails
 ************************/
size_t readaline(FILE *inputfd, char **datapp) {
        if (inputfd == NULL || *datapp == NULL) {
                fprintf(stderr, "TODO: terminate with a checked runtime error");
                fprintf(stderr, "Supplied arguments in readaline are NULL\n");
                exit(EXIT_FAILURE);
        }

        *datapp = malloc(1000);

        if(*datapp == NULL) {
                fprintf(stderr, "TODO: terminate with a checked runtime error");
                fprintf(stderr, " for \"3. Memory allocation fails\"\n");
                fclose(inputfd);
                exit(EXIT_FAILURE);
        }

        /* TODO: add a check to see if we're going over 1000 characters. */

        int i = 0;
        **datapp = fgetc(inputfd);
        while(*(*datapp + i) != '\n' && *(*datapp + i) != EOF) {
                i++;
                *(*datapp + i) = fgetc(inputfd);
        }
        
        /* Note: we are adding a null character to the end for printing! */
        *(*datapp + i) = '\0';

        return i;
}