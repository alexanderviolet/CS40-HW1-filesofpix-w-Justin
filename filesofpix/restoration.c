/*
 *      restoration.c
 *      Justin and Alex, September 9, 2025
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
int main(int argc, char *argv[])
{
        FILE *fp = openFile(argc, argv);

        Seq_T file_contents = Seq_new(1000);
        Seq_T *contents_p = &file_contents;
        const char *corruption_seq = findCorruptionSequence(fp, &contents_p);
        printf("IN MAIN: corruption sequence is %s\n", corruption_seq);

        printf("asdfIN MAIN: trying to print first two indices of sequence:");
        char *index_0 = Seq_get(file_contents, 0);
        char *index_1 = Seq_get(file_contents, 1);
        printf("IN MAIN: index 0: %s", index_0);
        printf("IN MAIN: index 1: %s", index_1);

        Seq_free(&file_contents);

        /* Write uncorrupted data now */

        fclose(fp);
        
        return 0;
}

/********** openFile ********
 *
 * Try to open a file and ensure we have enough arguments to actually open it. 
 *
 * Parameters:
 *      int argc:       copy of the argument count back in main
 *      char *argv[]:   copy of the argument array to access filename
 *
 * Return: 
 *      FILE pointer to file (may or may not be actually sucessful)
 *
 * Expects
 *      argc and argv to be properly defined, other error checking occurs here
 * Notes:
 *      Error checking file is open will happen in readaline as according to 
 *      the spec that should be the one throwing a checked runtime error. 
 ************************/
FILE *openFile(int argc, char *argv[]) 
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

/********** findCorruptionSequence ********
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
const char *findCorruptionSequence(FILE *fp, Seq_T **sequence) 
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
        int bytes = readaline(fp, &data);

        /* 
         * Create table HERE for sole purpose of determining original file line
         * corruption. Once we find the correct sequence of characters, you
         * shouldn't need the table anymore and we can safely free after 
         * printing the only other duplicate key in table. 
         */
        Table_T table = Table_new(1000, NULL, NULL);

        char *temp = NULL;
        char *temp_for_atom;

        /* Loop through the file until we find a duplicate */
        while (bytes != 0 && temp == NULL) {
                /* create a table pointer to pass by reference */
                Table_T *tablep = &table;

                /* nondigit_string: key for table */
                char *nondigit_string = filterDigits(bytes, data);
                temp = putAtomIntoTable(nondigit_string, data, &tablep);
                
                /* Getting around the const issue to actually as atom */
                if (temp != NULL) {
                        temp_for_atom = temp;
                        printf("current data string once match found: %s\n", data);
                }

                /* TODO: Must put into sequence here */
                Seq_put(**sequence, 0, (void *) data);
                

                /* reset for next iteration since previous data in table */
                free(data);
                bytes = readaline(fp, &data);
        }
        
        unsigned corruption_l = string_length(temp_for_atom);
        const char *corruption_string = Atom_new(temp_for_atom, corruption_l);
        /* Maybe put this elsewhere */
        free(temp_for_atom);
        free(data);

        /* 
         * TODO: before we free table, you must write to stdout the current 
         * duplicate emplaced in table 
         */
        void *second_line = Table_get(table, corruption_string);
        Seq_put(**sequence, 1, second_line);
        Table_free(&table);

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
char *putAtomIntoTable(char *nondigit_string, char *data, Table_T **tablepp)
{
        /* create atom as key for table */
        unsigned nondigit_len = string_length(nondigit_string);
        const char *nondigit_atom = Atom_new(nondigit_string, nondigit_len);

        /* convert data to void poitners to pass to Table_put arguments */
        const void *nondig_vp = nondigit_atom;
        void *data_vp = data;

        /*
         * Emplace in table and check for duplicates. Table_put returns NULL if
         * a new element is successfully emplaced. However, it will return the
         * old value if a duplicate key is emplaced. Return the key for future. 
         */
        if (Table_put(**tablepp, nondig_vp, data_vp) != NULL) {
                return nondigit_string;
        }

        /* 
         * Recycle memory of nondigit character array because the atom is saved
         * inside the table by this point. We haven't found our duplicate yet,
         * so let findCorruptionSequence know we have to try again by returning 
         * NULL
         */
        free(nondigit_string);
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

        if (nondigit_string == NULL) {
                fprintf(stderr, "malloc failed in putAtomIntoTable\n");
                fprintf(stderr, "TODO: make this more modular\n");
                exit(EXIT_FAILURE);
        }

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