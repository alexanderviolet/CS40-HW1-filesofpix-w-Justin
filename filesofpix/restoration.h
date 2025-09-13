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
#include "seq.h"

char *allocate(unsigned n); 
FILE *openFile(int argc, char *argv[]);
const char *findCorruptionSequence(FILE *fp, Seq_T **sequence);
char *putAtomIntoTable(char *nondigit_string, char *data, Table_T **tablepp);
char *filterDigits(int bytes, char *data);
unsigned string_length(char *string);
void writeUncorruptedData(const char *seq, FILE *fp, char *data);

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