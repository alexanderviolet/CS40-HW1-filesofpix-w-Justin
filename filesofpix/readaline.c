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
 * Return true if all scores are under a given limit and return number of
 * scores under the limit via reference parameter
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
        (void) inputfd;
        (void) datapp;

        printf("You are in readaline.c!\n");

        return 0;
}