/*
 *      restoration.c
 *      Justin Paik (jpaik03), Alex Violet (aviole01)
 *      September 15, 2025
 *      filesofpix
 * 
 *      TODO: Summary
 */
#include "restoration.h"

/********** NAME ********
 *
 * DESCRIPTION
 *
 * Parameters:
 *      
 * Return: 
 *      
 * Expects
 *      
 * Notes:
 *      
 ************************/
 
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*                      EXCEPTION(S)                     */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

Except_T Invalid_Arg_Count;
Except_T File_Open_Error;
Except_T Input_File_Read_Error; // TODO: implement
Except_T Expected_Input_Not_Supplied; // TODO: implement
Except_T Memory_Allocation_Error;

int main(int argc, char *argv[])
{
        FILE *fp = openFile(argc, argv);

        /* Hanson Data Structures initialized here */
        Table_T table = Table_new(1000, NULL, NULL);
        Seq_T file_contents = Seq_new(1000);
        
        const char *corruption_str = findCorruptionStr(fp, &file_contents, &table); /* TODO: 80 chars */
        printf("IN MAIN: corruption string is %s\n", corruption_str);

        printf("first index of sequence: %s\n", (char *) Seq_get(file_contents, 0));
        printf("second index of sequence: %s\n", (char *) Seq_get(file_contents, 1));

        /* Question: why do I do this mumbo jumbo? */
        char initialized = 'x';
        char *data = &initialized;

        int bytes = readaline(fp, &data);
        const char *file_line = Atom_new(data, bytes);

        printf("Next line: %s\n", file_line);

        free(data);

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
 *
 * Return: 
 *      FILE pointer to file (given file opens successfully)
 *
 * Expects:
 *      argc and argv to be properly defined, other error checking occurs here
 * 
 * Notes:
 *      TODO: EDIT THIS SHIT 
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
 * Notes:
 *      None
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
 *      FILE *fptr:     copy of the file pointer to file (TODO: verify this is correct)
 * Return: 
 *      None
 * Expects:
 *      argc to be properly defined
 * Notes:
 *      None
 ************************/
void verifyFileOpened(FILE *fptr)
{
        if (fptr == NULL) {
                RAISE(File_Open_Error);
        }
}

/********** findCorruptionStr ********
*
* initialize hanson data structures utilized throughout the program
*
* Parameters:
*      FILE pointer fp to corrupted data
* Return:
*      None so far
* Expects
*      fp to be open and able to read
*      fp is formatted according to the spec
* Notes:
*
************************/
const char *findCorruptionStr(FILE *fp, Seq_T *seq, Table_T *table)
{ 
        /* Create data pointer to hold readaline string */
        /*
         * TODO: Getting valgrind error of depending on uninitialized values.
         * we would set this to NULL, but readaline requires an error check to
         * ensure none of the provided arguments are null. For now, we're just
         * gonna do this mumbo jumbo to silence that error. There must be a 
         * better way to do this, but idk ¯\_(ツ)_/¯
         */
        char initialized = 'x';
        char *data = &initialized;
        // int bytes = readaline(fp, &data);
        int bytes = -1;

        char *temp = NULL;
        char *temp_for_atom;

        /* Loop through the file until we find a duplicate */
        while (bytes != 0 && temp == NULL) {
                /* reset for next iteration since previous data in table */
                bytes = readaline(fp, &data);

                /* nondigit_string: key for table */
                char *nondigit_string = filterDigits(bytes, data);
                temp = putAtomIntoTable(nondigit_string, data, &table, &seq);
                
                /* Getting around the const issue to actually as atom */
                if (temp != NULL) {
                        temp_for_atom = temp;
                }

        }
        
        unsigned corruption_l = string_length(temp_for_atom);
        const char *corruption_string = Atom_new(temp_for_atom, corruption_l);
        
        free(temp_for_atom);

        /* pluck original file data from table to make freeing easier */
        void *second_line = Table_get(*table, corruption_string);
        Table_remove(*table, corruption_string);

        Seq_addhi(*seq, second_line);

        // free(data); /* TODO: YOU ARE FREEING DATA HERE REMEMBER FOR WHEN YOU MUST WRITE NEW FILE INFO TO SEQUENCE!!!!! move somewhere else later */

        return corruption_string; 
}

/********** NAME ********
 *
 * DESCRIPTION
 *
 * Parameters:
 *      
 * Return: 
 *      
 * Expects
 *      
 * Notes:
 *      
 ************************/
char *putAtomIntoTable(char *nondigit, char *data, Table_T **tablepp, Seq_T **s)
{
        /* create atom as key for table */
        unsigned nondigit_len = string_length(nondigit);
        const char *nondigit_atom = Atom_new(nondigit, nondigit_len);

        /* convert data to void poitners to pass to Table_put arguments */
        const void *nondig_vp = nondigit_atom;
        void *data_vp = data;

        /*
         * Emplace in table and check for duplicates. Table_put returns NULL if
         * a new element is successfully emplaced. However, it will return the
         * old value if a duplicate key is emplaced. Return the key for future. 
         */
        char *duplicate = Table_put(**tablepp, nondig_vp, data_vp);

        if (duplicate != NULL) {
                /* put into sequence*/
                Seq_addhi(**s, (void *) duplicate);
                return nondigit;
        }

        /* 
         * Recycle memory of nondigit character array because the atom is saved
         * inside the table by this point. We haven't found our duplicate yet,
         * so let findCorruptionStr know we have to try again by returning 
         * NULL
         */
        free(nondigit);
        return NULL;
}

/********** filterDigits ********
 *
 * create and return a string on the heap that contains a line's nondigit 
 * corruption sequence of characters.
 *
 * Parameters:
 *      bytes: number of characters in original corrupted file line
 *      data: pass by value character array of original corrupted file
 * Return: 
 *      
 * Expects
 *      readaline succeeded and returns a nonzero byte number
 *      data is not NULL
 * Notes:
 *      nondigit string will not contain a newline
 *      nondigit string will always end with '\0'
 *      nondigit string must be freed elsewhere
 ************************/
char *filterDigits(int bytes, char *data)
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
                if(data[i] < '1' || data[i] > '9') {
                        nondigit_string[nondigit_len] = data[i];
                        nondigit_len++;
                }
        }

        nondigit_string[nondigit_len] = '\0';

        return nondigit_string;
}

/******** checkMalloc ********
 *
 * Check for successful memory allocation. If memory cannot be allocated using
 * malloc, raises a CRE.
 *
 * Parameters: TODO: correct this
 *      char *nondigit_string:  copy of nondigit string
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

/********** string_length ********
 *
 * determine length of a string
 *
 * Parameters:
 *      pointer (pass by value) to string
 * Return: 
 *      unsigned integer length of string
 * Expects
 *      argument is not NULL
 *      string is properly formatted and ends with '\0'
 ************************/
unsigned string_length(char *string) {
        unsigned length = 0;
        
        while (*string++) {
                length++;
        }

        return length;
}

/********** TODO ********
 *
 * DESCRIPTION
 *
 * Parameters:
 *      
 * Return: 
 *      
 * Expects
 *      
 * Notes:
 *      
 ************************/
static void vfree(const void *key, void **value, void *cl) {
        (void) key;
        (void) cl;
        
        free(*value);
}

/********** TODO ********
 *
 * DESCRIPTION
 *
 * Parameters:
 *      
 * Return: 
 *      
 * Expects
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
        printf("length: %u\n", length);
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