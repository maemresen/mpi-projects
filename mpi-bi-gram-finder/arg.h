// system libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int init_args(int argc, char **argv, char *input_file_name, char *output_file_name){

    // Inititalize Args
    int option_index = 0;
    while (( option_index = getopt(argc, argv, "hi:o:")) != -1){
        switch (option_index) {
            case 'i':
                strcpy(input_file_name, optarg);
                break;
            case 'o':
                strcpy(output_file_name, optarg);
                break;
            case 'h':
                return 1;
            default:
                printf("Option incorrect\n");
        }
    }
    return 0;
}