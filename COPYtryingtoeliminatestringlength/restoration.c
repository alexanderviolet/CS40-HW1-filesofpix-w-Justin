/*
 *      restoration.c
 *      Justin Paik (jpaik03), Alex Violet (aviole01)
 *      September 15, 2025
 *      filesofpix
 * 
 *      Convert corrupted P2 format PGMs into uncorrupted P5 PGMs
 */
#include "restoration.h"
 
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                      EXCEPTION(S)                     */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Except_T Invalid_Arg_Count;
Except_T File_Open_Error;
Except_T Input_File_Read_Error; // TODO: implement
Except_T Expected_Input_Not_Supplied; // TODO: implement
Except_T Memory_Allocation_Error;
Except_T No_Duplicate_Infusion_Found;

const int MAX_VAL = 255;

/********** main ********
 *
 * Driver function to run restoration program. All storage variable pointers
 * and program logic follows within this function. 
 *
 * Parameters:
 *      int argc:       copy of the argument count back in main
 *      char *argv[]:   copy of the argument array to access filename
 * Return: 
 *      0 as main function
 * Expects:
 *      user to compile program :D
 * Notes:
 *      None
 ************************/
int main(int argc, char *argv[])
{
        FILE *fp = openFile(argc, argv);

        /* Hanson Data Structures initialized here */
        Table_T table = Table_new(1000, NULL, NULL);
        Seq_T file_contents = Seq_new(1000);
        
        const char *infusion_str = findInfusion(fp, &file_contents, &table);

        storeOriginalRows(&file_contents, fp, infusion_str);

        writeRestoredFile(&file_contents);

        freeAllData(&table, &file_contents, fp);
        
        return 0;
}

/******** openFile ********
 *
 * Try to open a file by checking the number of arguments and verifying it has
 * opened. Returns a FILE pointer if successful, otherwise, terminates with a
 * Checked Runtime Error (CRE).
 *
 * Parameters:
 *      int argc:       copy of the argument count back in main
 *      char *argv[]:   copy of the argument array to access filename
 * Return: 
 *      FILE pointer to file (given file opens successfully)
 *
 * Expects:
 *      argc and argv to be properly defined, other error checking occurs here
 * 
 * Notes: 
 *      Error checking file is open will happen in readaline as according to 
 *      the spec that should be the one throwing a checked runtime error. 
 ************************/
FILE *openFile(int argc, char *argv[]) 
{
        checkArgCount(argc);

        /* Try to open argument */
        FILE *fptr = fopen(argv[1], "r");

        verifyFileOpened(fptr);

        return fptr;
}

/******** checkArgCount ********
 *
 * Checks the number of arguments. If it is invalid, raises a CRE.
 *
 * Parameters:
 *      int argc:       copy of the argument count back in main
 * Return: 
 *      None
 * Expects:
 *      argc to be properly defined
 ************************/
void checkArgCount(int argc)
{
        if (argc > 2)
        {
                RAISE(Invalid_Arg_Count);
        }
}

/******** verifyFileOpened ********
 *
 * Checks that the file pointer exists. If it is invalid, raises a CRE.
 *
 * Parameters:
 *      FILE *fptr:     copy of the file pointer to file
 * Return: 
 *      None
 * Expects:
 *      argc to be properly defined
 ************************/
void verifyFileOpened(FILE *fptr)
{
        if (fptr == NULL) {
                RAISE(File_Open_Error);
        }
}

/******** findInfusion ********
*
* Use Table to determine if a duplicate key (nondigit string) is within first
* file lines. Try to store minimal amount possible within table by searching
* for first duplicate. If there's a match, we place related values into sequence
* for restoration. 
*
* Parameters:
*      FILE pointer fp to corrupted data
*      Seq_T pointer by reference
*      Table_T pointer by reference
* Return:
*      atom representation of infusion sequence
* Expects:
*      fp to be open and able to read
*      fp is formatted according to the spec
*      Sequence and Table are initialized and not passed as NULL
* Notes:
*      Table is emplaced with atoms as keys and character arrays as values
*       
************************/
const char *findInfusion(FILE *fp, Seq_T *seq, Table_T *table)
{ 
        /* Create data pointer to hold readaline string */
        char *data = NULL;
        // int infusion_l = -1;

        /* Store character info in temp pointer */
        char *temp_for_atom = findDup(data, fp, &table, &seq);
        if (temp_for_atom == NULL) {
                fprintf(stderr, "for our own testing: no duplicate found\n");
                RAISE(No_Duplicate_Infusion_Found);
        }

        /* Make this an atom */
        unsigned infusion_l = string_length(temp_for_atom); /* TODO: eliminate string length*/
        const char *infusion_string = Atom_new(temp_for_atom, infusion_l);
        
        /* Recycle memory allocated from findDup we kick this off stack */
        free(temp_for_atom);

        /* pluck original file data from table to make freeing easier */
        void *second_line = Table_get(*table, infusion_string);
        Table_remove(*table, infusion_string);

        /* we know this file is important, so let's add to printing storage */
        Seq_addhi(*seq, second_line);

        return infusion_string; 
}

/******** findDup ********
*
* Find the duplicate infusion sequence to recognize original image lines.
*
* Parameters:
*      char pointer data to corrupted data
*      FILE pointer fp to unstored corrupted data
*      Call by reference Table_T pointer table to stored corrupted data
*      Call by reference Seq_T pointer seq to original infused data
* Return:
*      char pointer temp_for_atom to original infusion sequence
* Expects:
*      fp to be open and able to read
*      fp is formatted according to the spec
*      Sequence and Table are initialized and not passed as NULL
* Notes:
*
************************/
const char *findDup(char *data, FILE *fp, Table_T **t, Seq_T **s)
{
        /* Declare storage variables */
        const char *infusion = NULL;
        const char *filtered = NULL;
        const char *original = NULL;
        int bytes = -1;

        /* Loop through minimal amount of file until found duplicate */
        while (bytes != 0 && temp_for_atom == NULL) {
                /* reset for next iteration since previous data in table */
                bytes = readaline(fp, &data);
                original = Atom_new(data, bytes);

                /* nondigit_string: key for table */
                filtered = filterDigits(bytes, data);
                infusion = SearchTable(filtered, original, t, s);
                
                /* Getting around the const issue to actually as atom */
                if (infusion != NULL) {
                        return infusion;
                }
        }

        /* recycle memory for storage variable if unsuccessful */
        // free(filtered); /* TODO: atm cannot free atom

        /* Duplicate not found return NULL */
        return NULL;
}

/******** filterDigits ********
 *
 * create and return a string on the heap that contains a line's nondigit 
 * infusion sequence of characters.
 *
 * Parameters:
 *      bytes: number of characters in original corrupted file line
 *      data: pass by value character array of original corrupted file
 * Return: 
 *      char pointer to the injection sequence of the line
 * Expects:
 *      readaline succeeded and returns a nonzero byte number
 *      data is not NULL
 * Notes:
 *      nondigit string will not contain a newline
 *      nondigit string will always end with '\0'
 *      nondigit string must be freed elsewhere
 ************************/
const char *filterDigits(int bytes, char *data)
{
        int nondigit_len = 0;

        /* create a copy string to temporarily store nondigit chars */
        char *nondigit_string = malloc(bytes + 1);
        checkMalloc(nondigit_string);

        /* 
         * As to not skip indices in the temp string, nondigit_slot will only
         * increment when a new character is added to the temp. Looks a little
         * confusing, so maybe we can clean it up making the whole process of
         * filtering the string and copying it into an atom another function. 
         */
        for (int i = 0; i < bytes; i++) {
                if (isDigit(data[i]) == 0) {
                        nondigit_string[nondigit_len] = data[i];
                        nondigit_len++;
                }
        }

        
        nondigit_string[nondigit_len] = '\0'; /* TODO: maybe eliminate? */

        return Atom_new(nondigit_string, nondigit_len);
}

/******** SearchTable ********
 *
 * Populates the table with unique atoms that represent injection sequences.
 * When an atom repeats, Table_put() returns a char pointer to the currently
 * existing sequence, meaning it is an original line. Original lines are stored
 * in a Hanson Sequence.
 *
 * Parameters:
 *      char pointer nd to nondigit injection sequence
        char pointer data to corrupted data
        Table_T **t: pointer back to original table in main
        Seq_T **s: pointer back to original sequence in main
 * Return: 
 *      char pointer to an injection sequence
 * Expects:
 *      A duplicate is found at some point. 
 * Notes:
 *      Returns NULL for unexpected behavior of no duplicates.
 ************************/
const char *SearchTable(const char *nd, const char *og, Table_T **t, Seq_T **s)
{
        // /* create atom as key for table */
        // unsigned nondigit_len = string_length(nd);
        // const char *nondigit_atom = Atom_new(nd, nondigit_len);

        /* convert data to void pointers to pass to Table_put arguments */
        // const void *nondig_vp = nondigit_atom;

        /*
         * Emplace in table and check for duplicates. Table_put returns NULL if
         * a new element is successfully emplaced. However, it will return the
         * old value if a duplicate key is emplaced. Return the key for future. 
         */
        const char *duplicate = Table_put(**t, (void *)nd, (void *)og);

        if (duplicate != NULL) {
                /* put into sequence*/
                Seq_addhi(**s, (void *) duplicate);
                return nd;
        }

        /* 
         * Recycle memory of nondigit character array because the atom is saved
         * inside the table by this point. We haven't found our duplicate yet,
         * so let findInfusion know we have to try again by returning 
         * NULL
         */
        free(nd);
        return NULL;
}

/******** storeOriginalRows ********
 *
 * Populates a Hanson Sequence with the original rows of the files.
 *
 * Parameters:
 *      Sequence pointer seq to original rows data structure
        FILE pointer fp to corrupted data
        const char pointer infusion to original infusion seqeunce
 * Return: 
 *      None
 * Expects:
 *      
 * Notes:
 *      
 ************************/
void storeOriginalRows(Seq_T *seq, FILE *fp, const char *infusion) {
        char *data = NULL;
        char *nondigits = NULL;

        int bytes = readaline(fp, &data);
        while(bytes != 0 && *data != EOF) {
                nondigits = filterDigits(bytes, data);
                int nondigit_length = string_length(nondigits);
                const char *nondigit_atom = Atom_new(nondigits, nondigit_length); /* TODO 80 chars*/

                if (infusion == nondigit_atom) {
                        Seq_addhi(*seq, data);
                } else {
                        free(data);
                }

                bytes = readaline(fp, &data);
                free(nondigits);
        }

        free(data);
}

/******** writeRestoredFile ********
 *
 * Output the converted file in P5 format, including the header and raster.
 *
 * Parameters:
 *      Sequence pointer seq to original rows
 * Return:
 *      None
 * Expects:
 *      
 * Notes:
 *      Outputs restored file header and raster in P5 format.
 ************************/
void writeRestoredFile(Seq_T *seq)
{
        /* TODO: in future, you will be passing an atom, so be sure to record the length necessary for this for loop */
        printf("P5\n");

        char *first_line = (char *) Seq_get(*seq, 0);

        int length = string_length(first_line);
        int bool_printed = 1; /* TODO: 0 is false, 1 is true */
        int width = 0;

        for (int i = 0; i < length + 1; i++)
        {
                if (isDigit(first_line[i])) {
                        bool_printed = 0;
                } else if (bool_printed == 0) {
                        width++;
                        bool_printed = 1;
                }
        } 
        printf("%d %d\n%d\n", width, Seq_length(*seq), MAX_VAL);

        for (int i = 0; i < Seq_length(*seq); i++) {
                printLineInASCII((char *) Seq_get(*seq, i));
        }
}

/******** printLineInASCII ********
 *
 * Output the converted file in P5 format, including the header and raster.
 *
 * Parameters:
 *      char pointer line to current original line being output
 * Return:
 *      None
 * Expects:
 *      
 * Notes:
 *      Uses bool_printed status to determine whether to print a character or
 *      not.
 ************************/
void printLineInASCII(char *line)
{
        int length = string_length(line);
        int number = 0;
        int bool_printed = 1; /* TODO: 0 is false, 1 is true */

        int width = 0;
        for (int i = 0; i < length + 1; i++)
        {
                if (isDigit(line[i])) {
                        bool_printed = 0;
                        number = number * 10 + line[i] - '0';
                } else if (bool_printed == 0) {
                        printf("%c", (char) number);
                        width++;
                        bool_printed = 1;
                        number = 0;
                }
        }        
}

/******** freeAllData ********
 *
 * Free memory used by Hanson data structures.
 *
 * Parameters:
 *      Table pointer table to injection sequences
 *      Sequence pointer seq to original rows
        FILE pointer fp to corrupted data
 * Return:
 *      None
 * Expects:
 *      
 * Notes:
 *      
 ************************/
void freeAllData(Table_T *table, Seq_T *seq, FILE *fp) 
{
        /* Map through table */
        Table_map(*table, vfree, NULL);

        /* Free memory in sequence */
        int length = Seq_length(*seq);
        for (int i = 0; i < length; i++)
        {
                free(Seq_get(*seq, i));
        }

        /* Hansons data structures should be empty, so free their memory */
        Table_free(table);
        Seq_free(seq);

        /* Close file */
        fclose(fp);
}

/******** checkMalloc ********
 *
 * Check for successful memory allocation. If memory cannot be allocated using
 * malloc, raises a CRE.
 *
 * Parameters:
 *      char pointer nondigit_string to copy of nondigit chars
 * Return: 
 *      None
 * Expects:
 *      nondigit_string to be properly defined
 * Notes:
 *      None
 ************************/
void checkMalloc(char *nondigit_string)
{
        if (nondigit_string == NULL) {
                RAISE(Memory_Allocation_Error);
        }
}

/******** isDigit ********
 *
 * Returns 1 if a char is a digit. Else returns 0.
 *
 * Parameters:
 *      char c: a char to check
 * Return: 
 *      boolean representation of the function description
 * Expects:
 *      c is a valid char
 * Notes:
 *      None
 ************************/
unsigned isDigit(char c)
{
        if (c >= '0' && c <= '9') {
                return 1;
        }
        return 0;
}

/******** string_length ********
 *
 * determine length of a string
 *
 * Parameters:
 *      pointer (pass by value) to string
 * Return: 
 *      unsigned integer length of string
 * Expects:
 *      argument is not NULL
 *      string is properly formatted and ends with '\0'
 ************************/
unsigned string_length(char *string) {
        unsigned length = 0; /* TODO */
        
        while (*string++) {
                length++;
        }

        return length;
}

/******** vfree ********
 *
 * Deallocate memory used by the entries and table.
 *
 * Parameters:
 *      const void pointer key, void pointer cl: Unused variables necessary for
 *                                               compilation
 * Return: 
 *      None
 * Expects:
 *      None
 ************************/
static void vfree(const void *key, void **value, void *cl) {
        (void) key;
        (void) cl;
        
        free(*value);
}