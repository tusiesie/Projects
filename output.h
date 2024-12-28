#ifndef _OUTPUT_H_
#define _OUTPUT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processing.h"

/* Compile-time constants */
#define MAX_WORD_LEN 150
#define MAX_BITS 3

/*
 *add your include and prototypes here
*/
void print_title(int, int*);
void print_one(char**, int, char**, Person*, int*, int);
void print_two(int, Person*, int*, int);
void print_three(int, Person*, int*, int);
void print_results(int, char[MAX_BITS][MAX_WORD_LEN], char**, char**, Person*, int*, int);

#endif
