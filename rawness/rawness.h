/*
 * Justin and Alex V
 * September 5, 2025
 * CS40: Lab 1
 */
#ifndef RAWNESS_H
#define RAWNESS_H
#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h> /* This is already doing the extern stuff */
#include <setjmp.h>
#define T Pnmrdr_T

void tryOpeningFile(int argc, char *argv[]);

#endif
