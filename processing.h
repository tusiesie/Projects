#ifndef _PROCESSING_H_
#define _PROCESSING_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_handling.h"

/* Compile-time constants */
#define MAX_DATE_FIELDS 3

/*
 *add your include and prototypes here
*/

/* Set include to all include */
void set_include(int*, int);

/* Get number of people included in output */
int get_viable(int*, int);

/* Get the age of individual based on birthday and current date */
int get_age(int[MAX_DATE_FIELDS], int[MAX_DATE_FIELDS]);

/* Get current day */
void get_cur_time(int[MAX_DATE_FIELDS]);

/* Count the number of questions */
int count_line(char*);

/* Get number scores based on answers */
int get_direct(char*, char**);
int get_reverse(char*, char**);

#endif
