#ifndef _DYN_SURVEY_H_
#define _DYN_SURVEY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "output.h"

/* Compile-time constants */
#define MAX_WORD_LEN 150
#define MAX_BITS 3
#define MAX_DATE_FIELDS 3

/*
 *Function prototypes
 */
void allocate_2D(char***, int, int);
int count_line(char*);

#endif
