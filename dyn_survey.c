#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "emalloc.h"
#include "dyn_survey.h"

/*
 *Compile-time constants
 */

#define MAX_LINE_LENGTH 3000
#define MAX_WORD_LEN 150

#define MAX_OPTIONS 6
#define MAX_BITS 3

#define MAX_DATE_FIELDS 3
#define YEAR 0
#define MONTH 1
#define DAY 2

#define MAX_C 8
#define MAX_I 10
#define MAX_G 10
#define MAX_U 6
#define MAX_P 4

/* Allocate memory for a 2D array */
void allocate_2D(char ***data, int rows, int cols) {

	*data = (char**)emalloc(sizeof(char*) * rows); // Allocate memory for data

	for (int i = 0; i<rows; i++)
	{
		(*data)[i] = (char *)emalloc(sizeof(char) * cols); // Allocate memory for each component
	}
}

/* Set default to include the person to 1*/
void set_include(int *include, int size) {

	for (int i = 0; i<size; i++)
	{
		include[i] = 1;
	}
}

/* Get the number of people that are included */
int get_viable(int *include, int respondents) {

	int count = 0;

	for (int i = 0; i<respondents; i++)
	{
		if (include[i] == 1)
		{
			count++;
		}
	}

	return count;
}

/* Save the student major information */
void save_major(Person *students, char *line, int count) {

	int len = strlen(line) + 1;
	students[count].major = (char *)emalloc(sizeof(char) * len); // Allocate memory
	strncpy(students[count].major, line, len);
}


/* Save the student Canadian information*/
void save_canadian(Person *students, char *line, int count) {

	int len = strlen(line) + 1;
	students[count].canadian = (char *)emalloc(sizeof(char) * len); // Allocate memory
	strncpy(students[count].canadian, line, len);
}

/* Save the student birth date*/
void save_age(Person *students, char *line, int count) {

	int len = strlen(line)+1;
	students[count].birthday = (char *)emalloc(sizeof(char) * len); // Allocate memory
	strncpy(students[count].birthday, line, len);

}

/* Compare birthdate and current date to get age */
int get_age(int birthday[MAX_DATE_FIELDS], int date[MAX_DATE_FIELDS]) {

	int age = -1; // Initialize person has not had birthday yet

	if (birthday[MONTH] < date[MONTH]) {
		age++;

	} else if (birthday[MONTH] == date[MONTH] && birthday[DAY] <= date[DAY]) {
		age++;
	}

	age += (date[YEAR] - birthday[YEAR]); // Get the difference of year (age)

	return age;
}

/* Convert YYYY-MM-DD to int array */
void get_date(char line[MAX_WORD_LEN], int date[MAX_DATE_FIELDS]) {

	char *token;
	int count = 0;

	token = strtok(line, "-\n\r"); // Get the year

	while (token)
	{
		date[count] = atoi(token);
		count++;
		token = strtok(NULL, "-\n\r"); // Get the month and day
	}
}

/* Getting current date */
void get_cur_time(int date[MAX_DATE_FIELDS]) {

	time_t time_cur;
	struct tm *now;
	char temp[MAX_WORD_LEN];

	time( &time_cur );
	now = localtime( &time_cur );
	strftime(temp, MAX_WORD_LEN, "%F", now);

	get_date(temp, date);


}

/* Count the number of questions */
int count_line(char *line) {

	int len = strlen(line) + 1;
	int count = 0;
	char *token;

	char *copy = (char *)emalloc(sizeof(char *) * len); // Allocate memory for copy
	strncpy(copy, line, len);

	token = strtok(copy, ";,\n\r");

	while (token)
	{
		count++; // Increament count each iteration until token is NULL
		token = strtok(NULL, ";,\n\r");
	}

	free(copy); // Free copy

	return count;
}

/* Tokenizes line and stores in dynamic array */
void tokenize_line_dyn(char *line, char **words, int size) {

	char *token;

	token = strtok(line, ";,\n\r"); // Get first token

	for (int i = 0; i<size; i++)
	{
		strncpy(words[i], token, strlen(token)+1);
		token = strtok(NULL, ";,\n\r");
	}
}

/* Tokenizes each line by splitting words and stores then in a string array */
void tokenize_line(char *line, char words[][MAX_WORD_LEN]) {

	int num_words = 0;
	char *token;

	/* Get the first token from the line */
	token = strtok(line, ";,\n\r");

	while (token)
	{
		strncpy(words[num_words], token, MAX_WORD_LEN);
		num_words++;

		/* Get the next token from line or reach end of line */
		token = strtok(NULL, ";,\n\r");
	}
}

/* Get answer for direct answer*/
int get_direct(char *answer, char **options) {

	if (strcmp(answer, options[0]) == 0) {
		return 1;
	} else if (strcmp(answer, options[1]) == 0) {
		return 2;
	} else if (strcmp(answer, options[2]) == 0) {
		return 3;
	} else if (strcmp(answer, options[3]) == 0) {
		return 4;
	} else if (strcmp(answer, options[4]) == 0) {
		return 5;
	} else {
		return 6;
	}
}

/* Get the score for reverse answer */
int get_reverse(char *answer, char **options) {

	if (strcmp(answer, options[0]) == 0) {
		return 6;
	} else if (strcmp(answer, options[1]) == 0) {
		return 5;
	} else if (strcmp(answer, options[2]) == 0) {
		return 4;
	} else if (strcmp(answer, options[3]) == 0) {
		return 3;
	} else if (strcmp(answer, options[4]) == 0) {
		return 2;
	} else {
		return 1;
	}
}

/* Filter respondents based on major */
void filter_major(char *line, int *include, Person *students, int size) {

	for (int i = 0; i<size; i++)
	{
		if ((include[i] == 1) && (strcmp(line, students[i].major) != 0))
		{
			include[i] = 0; // Change person to: do not include
		}
	}
}

/* Filter respondents based on Canadian */
void filter_canadian(char *line, int *include, Person *students, int size) {

	for (int i = 0; i<size; i++)
	{
		if (include[i] == 1 && strcmp(line, students[i].canadian) != 0)
		{
			include[i] = 0; // Change person to: do not include
		}
	}
}

/* Filter respondents based on age */
void filter_age(int low, int high, int *include, Person *students, int size, int date[MAX_DATE_FIELDS]) {

	int birth_int[MAX_DATE_FIELDS];
	int age = 0;
	char birth_char[MAX_WORD_LEN];

	for (int i = 0; i<size; i++)
	{
		strncpy(birth_char, students[i].birthday, MAX_WORD_LEN); // Copy person's birthday to birth_char
		get_date(birth_char, birth_int); // Change birth_char to int array
		age = get_age(birth_int, date); // Get age of person

		if (include[i] == 1 && (age<low || age>high))
		{
			include[i] = 0; // Change person to: do not include
		}
	}
}

/* Print the title title and number of respondents */
void print_title(int respondents, int *include) {

	int total = get_viable(include, respondents);

	printf(	"Examining Science and Engineering Students' Attitudes Towards Computer Science\n"
			"SURVEY RESPONSE STATISTICS\n\n"
			"NUMBER OF RESPONDENTS: %d\n\n", total);
}

/* Print first bit */
void print_one(char **questions, int respondents, char **types, Person *students, int *include, int total_questions) {

	/* Initialize variables of variabless: fully disagree, disagree, partially disagree, partially agree, agree, fully agree*/
	float fd = 0, d = 0, pd = 0, pa = 0, a = 0, fa = 0;

	int total = get_viable(include, respondents);

	/* Print subtitle */
	printf("FOR EACH QUESTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n\n");

	for (int question_num = 0; question_num<total_questions; question_num++) // Loop for each question
	{
		fd = d = pd = pa = a = fa = 0; // Reset each catrgory

		if (strcmp(types[question_num], "Direct") == 0)
		{
			for (int person = 0; person<respondents; person++) // Loop for each person's response
			{
				if (include[person] == 0)
				{
					continue;
				}

				switch (students[person].responses->answers[question_num])
				{
					case 1:
						fd++;
						break;
					case 2:
						d++;
						break;
					case 3:
						pd++;
						break;
					case 4:
						pa++;
						break;
					case 5:
						a++;
						break;
					case 6:
						fa++;
						break;
				}
			}
		} else {
			for (int person = 0; person<respondents; person++)
			{
				if (include[person] == 0)
				{
					continue;
				}

				switch (students[person].responses->answers[question_num])
				{
					case 1:
						fa++;
						break;
					case 2:
						a++;
						break;
					case 3:
						pa++;
						break;
					case 4:
						pd++;
						break;
					case 5:
						d++;
						break;
					case 6:
						fd++;
						break;
				}
			}
		}

		/* Get respondent percentages if respondents > 0*/
		if (total != 0)
		{
			fd *= (100 / total);
			d *= (100 / total);
			pd *= (100 / total);
			pa *= (100 / total);
			a *= (100 / total);
			fa *= (100 / total);
		}

		/* Printing results*/
		printf("%s\n", questions[question_num]);
		printf(	"%.2f: fully disagree\n"
			"%.2f: disagree\n"
			"%.2f: partially disagree\n"
			"%.2f: partially agree\n"
			"%.2f: agree\n"
			"%.2f: fully agree\n\n", fd, d, pd, pa, a, fa);
	}
}

/* Print second bit */
void print_two(int respondents, Person *students, int *include, int total_questions) {

	/* Initialize each variable for each category of question*/
	double c = 0, i = 0, g = 0, u = 0, p = 0;

	/* Print subtitle*/
	printf("SCORES FOR ALL THE RESPONDENTS\n\n");

	for (int person = 0; person<respondents; person++) // Loop for each person
	{
		if (include[person] == 0)
		{
			continue;
		}

		c = i = g = u = p = 0;
		for (int question_num = 0; question_num<total_questions; question_num++)
		{
			if (question_num < MAX_C) // Get scores from category C
			{
				c += students[person].responses->answers[question_num];

			} else if (question_num < MAX_C+MAX_I) { // Get scores from category I

				i += students[person].responses->answers[question_num];

			} else if (question_num < MAX_C+MAX_I+MAX_G) { // Get scores from catorgory G

				g += students[person].responses->answers[question_num];

			} else if (question_num < MAX_C+MAX_I+MAX_G+MAX_U) { // Get scores from category U

				u += students[person].responses->answers[question_num];

			} else { // Get scores from category p

				p += students[person].responses->answers[question_num];

			}
		}

		/* Get the averages for each category*/
		c /= MAX_C;
		i /= MAX_I;
		g /= MAX_G;
		u /= MAX_U;
		p /= MAX_P;

		/* Print results*/
		printf("C:%.2f,I:%.2f,G:%.2f,U:%.2f,P:%.2f\n", c, i, g, u, p);
	}

	/* Formatting */
	printf("\n");
}

/* Print third bit */
void print_three(int respondents, Person *students, int *include, int total_questions) {

	/* Initialize variables for each category */
	double c = 0, i = 0, g = 0, u = 0, p = 0;

	int total = get_viable(include, respondents);

	for (int person = 0; person<respondents; person++) // Loop for each person
	{

		if (include[person] == 0)
		{
			continue;
		}

		for (int question_num = 0; question_num<total_questions; question_num++) // Loop for each question
		{
			if (question_num < MAX_C) // Get scores for category C
			{
				c += students[person].responses->answers[question_num];

			} else if (question_num < MAX_C+MAX_I) { // Get scores for category I

				i += students[person].responses->answers[question_num];

			} else if (question_num < MAX_C+MAX_I+MAX_G) { // Get scores for category G

				g += students[person].responses->answers[question_num];

			} else if (question_num < MAX_C+MAX_I+MAX_G+MAX_U) { // Get scores for category U

				u += students[person].responses->answers[question_num];

			} else { // Get scores for category P

				p += students[person].responses->answers[question_num];

			}
		}
	}

	/* Get total average if respondents > 0*/
	if (total != 0)
	{
		c /= (MAX_C*total);
		i /= (MAX_I*total);
		g /= (MAX_G*total);
		u /= (MAX_U*total);
		p /= (MAX_P*total);
	}

	/* Print subt title and results*/
	printf("AVERAGE SCORES PER RESPONDENT\n\n");
	printf("C:%.2f,I:%.2f,G:%.2f,U:%.2f,P:%.2f\n", c, i, g, u, p);

}

/* Print the final results */
void print_results(int respondents, char bits[MAX_BITS][MAX_WORD_LEN], char **questions, char **types, Person *students, int *include, int total_questions) {

	/* Print main title*/
	print_title(respondents, include);

	if (strcmp(bits[0], "1") == 0)
	{

		print_one(questions, respondents, types, students, include, total_questions); // Print percent frequency for each question
	}

	if (strcmp(bits[1], "1") == 0)
	{

		print_two(respondents, students, include, total_questions); // Print score for all respondents
	}

	if (strcmp(bits[2], "1") == 0)
	{

		print_three(respondents, students, include, total_questions); // Print average scores for categories
	}
}


int main() {
	/* Variable Declarations */
	int respondents = 0, chunk = 0, count = 0, question_num = 0, score = 0, total_num_questions = 0, low = 0, high = 0;
	int  date[MAX_DATE_FIELDS];
	int *include;
	char bits[MAX_BITS][MAX_WORD_LEN];
	char *token, **questions, **types, **options;
	Person *students;

	/* Save the current date */
	char *line = (char *)emalloc(sizeof(char) * MAX_LINE_LENGTH); // Allocate space for line
	get_cur_time(date); // Get current date

	/* Reading the Input*/
	while (fgets(line, sizeof(char) * MAX_LINE_LENGTH, stdin))
	{
		if (line[0] != '#')
		{
			if (chunk>4 && line[0] == '0')
			{
				/* Filter for major */
				token = strtok(line, ",\n\r"); // Read first number
				token = strtok(NULL, ",\n\r"); // Get major
				filter_major(token, include, students, respondents); // Filter

			} else if (chunk>4 && line[0] == '1') {

				/* Filter for Canadian */
				token = strtok(line, ",\n\r"); // Read first number
				token = strtok(NULL, ",\n\r"); // Get "yes" or "no"
				filter_canadian(token, include, students, respondents); // Filter

			} else if (chunk>4 && line[0] == '2') {

				/* Filter for age */
				token = strtok(line, ",\n\r"); // Read first number

				token = strtok(NULL, ",\n\r"); // Get lower bound
				low = atoi(token);

				token = strtok(NULL, ",\n\r"); // Get upper bound
				high = atoi(token);

				filter_age(low, high, include, students, respondents, date); // Filter

			} else if (chunk>4) // Reading responses
			{

				/* Save data for major */
				token = strtok(line, ", \n\r");
				save_major(students, token, count);

				/* Save data for Canadian */
				token = strtok(NULL, ",\n\r");
				save_canadian(students, token, count);

				/* Save data for age */
				token = strtok(NULL, ",\n\r");
				save_age(students, token, count);

				/* Get next token */
				token = strtok(NULL, ",\n\r");
				question_num = 0;

				/* Loop for each token */
				while (token)
				{

					if (strcmp(types[question_num], "Direct") == 0) // Question is direct type
					{

						score = get_direct(token, options);

					} else { // Question is reverse type

						score = get_reverse(token, options);

					}

					students[count].responses->answers[question_num] = score; // Record score
					question_num++; // Increment question number
					token = strtok(NULL, ",\n\r"); //Get next token
				}

				count++; // Plus one respondent

			} else if (chunk == 0) { // Tokenize the test bits

				tokenize_line(line, bits);
				chunk++;

			} else if (chunk == 1) { // Tokenize the questions

				total_num_questions = count_line(line);

				allocate_2D(&questions, total_num_questions, MAX_WORD_LEN+1); // Allocate memory

				tokenize_line_dyn(line, questions, total_num_questions); // Store data for questions
				chunk++;

			} else  if (chunk == 2) { // Tokeize the question types

				allocate_2D(&types, total_num_questions, MAX_WORD_LEN+1); //Allocat memory

				tokenize_line_dyn(line, types, total_num_questions); // Store data for question types
				chunk++;

			} else if (chunk == 3) { // Tokenize answer types

				allocate_2D(&options, MAX_OPTIONS, MAX_WORD_LEN+1); // Allocate memory

				tokenize_line_dyn(line, options, MAX_OPTIONS); // Store data for answer types
				chunk++;

			} else { // Get the number of resonses

				respondents = atoi(line); // Get number of respondents

				students = (Person *)emalloc(sizeof(Person) * respondents); // Allocate memory for student struct
				for (int i = 0; i<respondents; i++) // Allocate memory for student answers
				{
					students[i].responses = (Responses *)emalloc(sizeof(Responses));
					students[i].responses->answers = (int *)emalloc(sizeof(int) * total_num_questions);
				}

				include = (int *)emalloc(sizeof(int) * respondents); // Allocate memory for int array to include or exclude
				set_include(include, respondents); // Set include to all 1s

				chunk++;
			}
		}
	}


	/* Outputing the results */
	print_results(respondents, bits, questions, types, students, include, total_num_questions);

	/* Free allocated memory */
	for (int i = 0; i<respondents; i++)
	{
		free(students[i].responses->answers);
		free(students[i].responses);
		free(students[i].major);
		free(students[i].canadian);
		free(students[i].birthday);
	}
	free(students);

	for (int i = 0; i<total_num_questions; i++)
	{
		free(questions[i]);
		free(types[i]);
	}
	free(questions);
	free(types);

	for (int i = 0; i<MAX_OPTIONS; i++)
	{
		free(options[i]);
	}
	free(options);

	free(line);
	free(include);

	return 0;

}
