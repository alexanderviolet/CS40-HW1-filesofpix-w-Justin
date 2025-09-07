/*
 * rawness.h
 * Justin Paik (jpaik03) and Alex Violet (aviole01)
 * September 5, 2025
 * CS40 Lab 1: Rawness
 *
 * This file contains the declarations for functions for opening and reading
 * pgm files to convert them from "plain" to "raw".
 */
#ifndef RAWNESS_H
#define RAWNESS_H
#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h> /* This is already doing the extern stuff */
#define T Pnmrdr_T

void testOpeningFile(int argc, char *argv[]);
void readPGM(char *argv[]);
void printReaderData(Pnmrdr_mapdata *data);
void convertP2toP5(T *rdr, Pnmrdr_mapdata *data);

#endif
