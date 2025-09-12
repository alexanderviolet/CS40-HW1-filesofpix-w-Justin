/*
 *      restoration.h
 *      Justin and Alex, September 9, 2025
 *      filesofpix
 * 
 *      included statements and function prototypes for restoration.c
 */
#ifndef RESTORATION_H
#define RESTORATION_H

#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "except.h"
#include "atom.h"
#include "table.h"

char *allocate(unsigned n); 
FILE *testOpeningFile(int argc, char *argv[]);
void atomsAndTables(FILE *fp);
void putAtomIntoTable(int bytes, char *data, Table_T **tablepp);

#endif


// Code graveyard: TODO delete before submission
// /* Make atom */
        // const char *atom = Atom_new(data, bytes); 
        // printf("Atom representation: %s\n", atom);

        // const void *vp_atom = atom;
        // void *vp_data = data;

        // /* Make table and emplace atom! Make sure you can access atom through table! */
        // Table_T table = Table_new(5, NULL, NULL);
        // Table_put(table, vp_atom, vp_data);
        // char *from_table_get = Table_get(table, vp_atom);
        // printf("First thing in table: %s\n", from_table_get);

        // free(data);
        // int bytes2 = readaline(fp, &data);

        // const char *atom2 = Atom_new(data, bytes2);
        // printf("Atom representation of second line: %s\n", atom2);
        // const void *vp_atom2 = atom2;
        // vp_data = data;
        // char *value = Table_put(table, vp_atom2, vp_data);
        // if (value == NULL) {
        //         printf("There was not a duplicate\n");
        // } else {
        //         printf("There was a duplicate: %s\n", value);
        // }

        // Table_free(&table);