/*
 * rawness.c
 * Justin Paik (jpaik03) and Alex Violet (aviole01)
 * September 5, 2025
 * CS40 Lab 1: Rawness
 *
 * This file implements functions for opening and closing files, reading the
 * header and contents of a pgm file, and converting from "plain" to "raw".
 */
#include "rawness.h"

int main(int argc, char *argv[])
{
        // testOpeningFile(argc, argv);
        (void) argc;

        readPGM(argv);

        return 0;
}

/*
 * readPGM
 * arguments: execuatble arguement array for filename
 * returns: 
 */

/********** readPGM **********
 *
 * TODO: starting to open the pnmreader. Our goal for now is to
 * identify the correct width and heigh before messing around with ints and
 * chars.
 *
 * Parameters:
 *      char *argv[]:   address of place to store # scores < limit
 *
 * Return: TODO: Nothing so far
 *
 * Expects
 *      (e.g., scores and scoresUnderLimit must not be NULL)
 * Notes:
 *      (e.g., *scoresUnderLimit is set to the number of scores under limit
 *      Will CRE if scores or scoresUnderLimit is NULL
 *      May CRE if malloc fails.)
 ******************************/
// bool allScoresUnderLimit(int *scoresUnderLimit, int limit,
//                          int scores[], int len);

void readPGM(char *argv[]) 
{
        FILE *fptr = fopen(argv[1], "rb");
        T rdr;

        /* Note: This doesn't really work when we try to pass it a bad pgm! */
        TRY
                rdr = Pnmrdr_new(fptr);
                RAISE(Pnmrdr_Badformat);
        EXCEPT(Pnmrdr_Badformat)
                ;
        END_TRY;

        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        // printReaderData(&data);

        convertP2toP5(&rdr, &data);
        
        fclose(fptr);
}

/*
 * convertP2toP5
 * bulk of the lab content: TODO: still figuring out exactly how to do 
 * everything
 */
void convertP2toP5(T *rdr, Pnmrdr_mapdata *data) 
{
        // printf("Entered convertP2toP5!\n");
        // printf("Header below:\n\n\n");
        printf("P5\n%u %u\n%u\n", data->width, data->height, data->denominator);        
        int currASCII;
        unsigned int currPos = 0;
        TRY
                while ((currASCII = Pnmrdr_get(*rdr)) != EOF) {
                        char rawASCII = currASCII;
                        printf("%c", rawASCII);
                        if (currPos < data->width - 1) {
                                printf(" ");
                                currPos++;
                        } else {
                                currPos = 0;
                                printf("\n");
                        }
                }
                RAISE(Pnmrdr_Count);
        EXCEPT(Pnmrdr_Count)
                ;
        END_TRY;
        
        printf("\n");

        Pnmrdr_free(rdr);
}

/*
 * printReaderData
 * output info for testing purposes. Won't be in final product
 */
void printReaderData(Pnmrdr_mapdata *data) 
{
        printf("FOR TESTING:\n");
        printf("\ttype: %d\n", data->type);
        printf("\twidth: %u\n", data->width);
        printf("\theight: %u\n", data->height);
        printf("\tdenominator: %u\n\n", data->denominator);
}

/*
 * testOpeningFile
 * for parts 6 - 8 we just wanted to see how to open a file in c before getting
 * to the pnmrdr stuff.
 */
void testOpeningFile(int argc, char *argv[])
{
        /* Check to see if you have enough arguments*/
        if (argc <= 1) {
                fprintf(stderr, "Error: Photo Required!\n");
                exit(EXIT_FAILURE);
        }

        /* Try to open a file */
        FILE *fptr;
        fptr = fopen(argv[1], "r");

        /* Error check to ensure file works */
        /* TODO */

        /* Check to see if file opened SHOULD FAIL */
        if (fptr == NULL) {
                fprintf(stderr, "FOR TESTING: The file did not open!\n");
                exit(EXIT_FAILURE);
        }
        else {
                // printf("FOR TESTING: The file %s was opened!\n", argv[1]);
        }

        fclose(fptr);
}