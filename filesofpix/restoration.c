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
        FILE *fp = testOpeningFile(argc, argv);

        /*
         * TODO: Getting valgrind error of depending on uninitialized values.
         * we would set this to NULL, but readaline requires an error check to
         * ensure none of the provided arguments are null. For now, we're just
         * gonna do this mumbo jumbo to silence that error. There must be a 
         * better way to do this, but idk ¯\_(ツ)_/¯
         */
        // char initialized = 'x';
        // char *data = &initialized;
        
        // int bytes = readaline(fp, &data);  
        atomsAndTables(fp);

        // while (bytes != 0) {
        //         for (int i = 0; i < bytes; i++) {
        //                 printf("%c", data[i]);
        //         }
        //         printf("\n");

        //         free(data);
        //         bytes = readaline(fp, &data);
        // }   

        // free(data);
        fclose(fp);
        
        return 0;
}

/********** testOpeningFile ********
 *
 * Try to open a file and ensure we have enough arguments to actually open it. 
 *
 * Parameters:
 *      int argc:       copy of the argument count back in main
 *      char *argv[]:   copy of the argument array to access filename
 *
 * Return: FILE pointer to file (may or may not be actually sucessful)
 *
 * Expects
 *      argc and argv to be properly defined, other error checking occurs here
 * Notes:
 *      Error checking file is open will happen in readaline as according to 
 *      the spec that should be the one throwing a checked runtime error. 
 ************************/
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

void atomsAndTables(FILE *fp) 
{
        /* Create data pointer to hold readaline string */
        char initialized = 'x';
        char *data = &initialized;

        int bytes = readaline(fp, &data);
        Table_T table = Table_new(1000, NULL, NULL);
        
        while (bytes != 0) {
                /* Store current readaline string into Atom */
                // const char *atom = Atom_new(data, bytes);

                Table_T *tablep = &table;
                putAtomIntoTable(bytes, data, &tablep);


                free(data);
                bytes = readaline(fp, &data);
        }   

        /* TODO ensure you can print what is in the table WITHIN THIS FUNCTION */

        /* Make atom */

        /* Make table and emplace atom! Make sure you can access atom through table! */
        free(data);
        Table_free(&table);
}

void putAtomIntoTable(int bytes, char *data, Table_T **tablepp) 
{
        const char *atom = Atom_new(data, bytes);

        char *nondigit_string = malloc(bytes + 1);

        if (nondigit_string == NULL) {
                fprintf(stderr, "malloc failed in putAtomIntoTable\n");
                fprintf(stderr, "TODO: make this more modular\n");
                exit(EXIT_FAILURE);
        }

        int nondigit_slot = 0;
        for (int i = 0; i < bytes; i++) {
                // printf("atom[%d]: %c ", i, atom[i]);
                if(atom[i] < '1' || atom[i] > '9') {
                        // printf("\n");
                        nondigit_string[nondigit_slot] = atom[i];
                        nondigit_slot++;
                } else {
                        // printf("<-- DO NOT INCLUDE\n");
                }
        }

        nondigit_string[nondigit_slot] = '\0';

        const char *nondigit_atom = Atom_new(nondigit_string, nondigit_slot);
        printf("key: \t%s\n", nondigit_atom);

        free(nondigit_string);

        if (Table_put(**tablepp, (void *) nondigit_atom, (void *) atom) == NULL) {
                printf("\t\tNote: we put something new in the table\n");
        } else {
                printf("\t\tNote: We have found a duplicate in the table\n");
        }

        printf("value: \t%s\n", (char *)Table_get(**tablepp, (void *) nondigit_atom));
        
}