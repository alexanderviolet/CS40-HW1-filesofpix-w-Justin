/*
 *      readaline.c
 *      Justin Paik (jpaik03), Alex Violet (aviole01)
 *      September 15, 2025
 *      filesofpix
 * 
 *      TODO: Summary
 */

#include "readaline.h"
#include "except.h"
#include <stdio.h>
#include <stdlib.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                      EXCEPTION(S)                     */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Except_T Invalid_Argument;
Except_T Readaline_Input_File_Read_Error; // TODO: implement
Except_T Readaline_Memory_Allocation_Error;
/* TODO: find out why multiple definitions of exceptions among files won't compile */

/********** readaline ********
 *
 * TODO: copy from spec
 *
 * Parameters:
 *      FILE *inputfd:  pointer to corrupted file from restoration
 *      char **datapp:  pass by reference pointer which will be set to the first
 *                      character of a line in the file
 *
 * Return:
 *      Number of bytes in read line
 *
 * Expects:
 *      inputfd and datapp must not be NULL
 * 
 * Notes:
 *      Will raise Checked Runtime Error if:
 *              1. Either or both of the supplied arguments is NULL
 *              2. An error is encountered reading from the file
 *              3. Memory allocation fails
 ************************/
size_t readaline(FILE *inputfd, char **datapp) {
        if (inputfd == NULL || *datapp == NULL) {
                RAISE(Invalid_Argument);
        }

        *datapp = malloc(1000);

        if(*datapp == NULL) {
                RAISE(Readaline_Memory_Allocation_Error);
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