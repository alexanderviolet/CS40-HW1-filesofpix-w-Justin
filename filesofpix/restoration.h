/*
 *      restoration.h
 *      Justin Paik (jpaik03), Alex Violet (aviole01)
 *      September 15, 2025
 *      filesofpix
 *
 *      Includes statements and function prototypes for restoration.c
 */
#ifndef RESTORATION_H
#define RESTORATION_H

#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "except.h"
#include "atom.h"
#include "table.h"
#include "seq.h"

char *allocate(unsigned n); 
FILE *openFile(int argc, char *argv[]);
void checkArgCount(int argc);
void verifyFileOpened(FILE *fptr);
const char *findInfusion(FILE *fp, Seq_T *seq, Table_T *table);
char *findDuplicate(char *data, FILE *fp, Table_T **table, Seq_T **seq);
char *putAtomIntoTable(char *nondigit, char *data, Table_T **tablepp, Seq_T **s);
char *filterDigits(int bytes, char *data);
void checkMalloc(char *nondigit_string);
unsigned string_length(char *string);
void writeRestoredFile(char *corrupted_string);
void freeAllData(Table_T *table, Seq_T *seq, FILE *fp);
static void vfree(const void *key, void **value, void *cl);

#endif

/* Function Contract Template: (TODO: delete before submission)*/

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