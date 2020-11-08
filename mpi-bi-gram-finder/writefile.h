#include <stdio.h>
#include <stdlib.h>

// constant definitions
#define COL_SIZE 5 // number of columns on output

/*
    Store calculated frequency values on char1 and char2.
    Required to save ascii values of characters after sorting the table.
*/
typedef struct bigram{
    char char1;
    char char2;
    int freq;
 } bigram_t;

int index_to_asci(int index); // function declaration

/*
    Converts 2D array to 1D array.
    
    Before
    {0, 0}, {0, 1}, {0, 2}
    {1, 0}, {1, 1}, {1, 2}
    {2, 0}, {2, 1}, {2, 2}

    After
    {0, 1, 2, 3, 4, 5, 6, 7, 8}

    Mapping indexes to row and column values
    row = index/width
    col = index%width
*/
void flatArray(int valid_chars, int *frequency_table_ptr, bigram_t *bigram_table_ptr)
{    
    for(int i=0; i<valid_chars*valid_chars; i++) {
        // Define Struct
        bigram_t * bigram = (bigram_t *) malloc(sizeof(bigram_t)); // allocate memory
        bigram->char1 = index_to_asci(i / valid_chars);            // calculate char 1
        bigram->char2 = index_to_asci(i % valid_chars);            // calculate char 2
        bigram->freq = (*frequency_table_ptr);                     // store freq
        
        // Set value
        bigram_table_ptr[i] = (*bigram);
        frequency_table_ptr++;
    }
}

/*
    Sort the given array with Bubble Sort algorithm
*/
void sort(int valid_chars, bigram_t *bigram_table){
    for (int i = 0; i < valid_chars*valid_chars - 1; i++){
        for (int j = 0; j < valid_chars*valid_chars-i-1; j++){
            if (bigram_table[j].freq < bigram_table[j+1].freq)
            {
                bigram_t swap = bigram_table[j];
                bigram_table[j] = bigram_table[j+1];
                bigram_table[j+1] = swap;
            }
        }
    }
}


/*
    Write calculated frequency table and chars to both file and console.
*/
void writeToFile(char *filename,int valid_chars, bigram_t *bigram_table)
{
    FILE *file_ptr = fopen(filename, "w"); // Open file and assign to a file pointer.

    fprintf(file_ptr,"\n----------------------------------------------");
    printf("\n----------------------------------------------");
    fprintf(file_ptr,"\n\t2-Gram Table");
    printf("\n\t2-Gram Table");
    fprintf(file_ptr,"\n----------------------------------------------\n");
    printf("\n----------------------------------------------\n");
 
    int col = 0; // to count printed column
    for (int i = 0; i < valid_chars*valid_chars; i++)
    {
        char c1 = bigram_table[i].char1; // first character
        char c2 = bigram_table[i].char2; // second character
        int freq = bigram_table[i].freq; // frequency of character pair
        
        // Printing Outputs to console and File
        if(freq != 0) // do not print if not found
        {   
            if(COL_SIZE <= col){
                fprintf(file_ptr, "\n");
                printf( "\n");
                col = 0;
            }
            col++;
            fprintf(file_ptr," %c%c:%8d |", c1 ,c2, freq);
            printf(" %c%c:%8d |", c1 ,c2, freq);
        }
    }
    printf("\n");
    fclose(file_ptr); // close file
}

/*
    Handle 2-Gram 
*/
void bigramWriteFile(char * filename,int valid_chars, int *frequency_table_ptr)
{
    // Declare Empty Struct Array
    bigram_t result[valid_chars*valid_chars];
    
    // Convert it 2D -> 1D
    flatArray(valid_chars, frequency_table_ptr, (bigram_t *) result);
    
    // Sort flattened array
    sort(valid_chars, (bigram_t *) result);

    // Print Outputs to the file and console
    writeToFile(filename, valid_chars, (bigram_t *) result);
}
