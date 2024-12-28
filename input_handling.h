#ifndef _INPUT_HANDLING_H_
#define _INPUT_HANDLING_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile-time constants */
#define MAX_WORD_LEN 150
#define MAX_DATE_FIELDS 3

/* Typedef for the responses */
typedef struct responses {
        int *answers; // Array of individual's answers
} Responses;

/* Typedef for each person */
typedef struct person {
        char *major;
        char *birthday;
        char *canadian;
        Responses *responses; //Pointer to responses
} Person;

/*
 *add your include and prototypes here
*/

/* Saves the data of major, canadian and birhtdate into student struct */
void save_major(Person*, char*, int);
void save_canadian(Person*, char*, int);
void save_age(Person*, char*, int);

/* Convert date from char to int */
void get_date(char[MAX_WORD_LEN], int[MAX_DATE_FIELDS]);

/* Tokenization functions */
void tokenize_line_dyn(char*, char**, int);
void tokenize_line(char*, char[][MAX_WORD_LEN]);

/* Filter by major, canadian and age to include to ignore */
void filter_major(char*, int*, Person*, int);
void filter_candian(char*, int*, Person*, int);
void filter_age(int, int, int*, Person*, int, int[MAX_DATE_FIELDS]);

#endif
