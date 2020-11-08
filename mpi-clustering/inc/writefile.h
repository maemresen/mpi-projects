#include "glo.h"

// system libraries
#include <stdio.h>
#include <stdlib.h>

// helper libraries
#include "filter.h"

// constant definitions
#define COL_SIZE 10 // number of columns on output

/*
    Store calculated frequency values on char1 and char2.
    Required to save ascii values of characters after sorting the table.
*/
typedef struct bigram{
    char char1;
    char char2;
    int freq;
 } bigram_t;

void flatArray(int valid_chars, int *frequency_table_ptr, bigram_t *bigram_table_ptr);
void sort(int valid_chars, bigram_t *bigram_table);

void writeToFile(char *filename,int valid_chars, bigram_t *bigram_table);
void bigramWriteFile(char * filename,int valid_chars, int *frequency_table_ptr);