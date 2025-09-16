/*
 *      readaline.c
 *      Justin Paik (jpaik03), Alex Violet (aviole01)
 *      September 15, 2025
 *      filesofpix
 * 
 *      Process file line information and give restoration implemntation a 
 *      pointer to data storage array and the number of bytes stored
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


/* Helper function prototype */
void reserveMore(int *vsize, char **datapp);

/********** readaline ********
 *
 * primary function to process file information into a character array for
 * restoration. 
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
        /* arbitrarily initialize first size to be 1000 */
        int vsize = 1000;

        /* Check valid arguments, raise CRE if invalid */
        if (inputfd == NULL || datapp == NULL) {
                RAISE(Invalid_Argument);
        }

        /* Allocate memory and raise CRE if unsuccessful */
        *datapp = malloc(vsize);
        if(*datapp == NULL) {
                RAISE(Readaline_Memory_Allocation_Error);
        }


        int i = 0;
        **datapp = fgetc(inputfd);
        while(*(*datapp + i) != '\n' && *(*datapp + i) != EOF) {
                i++;
                /* If reached max size, then reserve more memory */
                if (i == vsize) {
                        reserveMore(&vsize, datapp);
                }

                /* continue storing characters from file */
                *(*datapp + i) = fgetc(inputfd);
        }
        
        /* Note: we are adding a null character to the end for printing! */
        *(*datapp + i) = '\0';

        /* return the length of the iteration for the number of bytes */
        return i;
}

/********** reserveMore ********
 *
 * reallocate more memory when we reach max buffer size
 *
 * Parameters: 
 *      pointer to vsize integer to update new size
 *      datapp pointer to reset to buffer array
 * Return: 
 *      none
 * Expects
 *      datapp is not NULL
 * Notes:
 *      This is pretty much exactly like ArrayList expansion function.
 ************************/
void reserveMore(int *vsize, char **datapp) {        
        /* Allocate more memory for larger input size */
        char *buffer = malloc(*vsize * 2);
        if (buffer == NULL) {
                RAISE(Readaline_Memory_Allocation_Error);
        }

        /* Copy values into buffer array */
        for (int i = 0; i < *vsize; i++) {
                buffer[i] = *(*datapp + i);
        }

        /* Recycle memory of smaller array */
        free(*datapp);

        /* Update datapp's pointer value to the first address of buffer array */
        *datapp = buffer;
        
        /* size needs to represent the current size of new array */
        *vsize *= 2; 
}