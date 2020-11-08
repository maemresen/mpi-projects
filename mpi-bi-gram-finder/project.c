// system libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <mpi.h>
#include <unistd.h>

// helper libraries
#include "arg.h"
#include "readfile.h"
#include "writefile.h"
#include "filter.h"

// constant definitions
#define ROOT 0  // Rank of the Root node
#define FILE_NAME_MAX_LEN 100

int main(int argc, char **argv)
{
    // Inititalize MPI
    MPI_Init(&argc, &argv);
    double total_time_start = MPI_Wtime(); // total timer start

    // MPI_Request and MPI_Status for track async communication
    MPI_Request request;
    MPI_Status status;
    int request_complete = 0;

    // MPI Variables
    int rank; // rank of each parallel node
    int size; // number of nodes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize Arguments
    char *input_file_name = (char *) malloc(FILE_NAME_MAX_LEN * sizeof(char));
    char *output_file_name = (char *) malloc(FILE_NAME_MAX_LEN * sizeof(char));
    int result = init_args(argc, argv, input_file_name, output_file_name);
    if(result && rank == ROOT)
    {
        printf("\n Usage: mpirun [-np <num_of_nodes>] project -input <input_file_path> -output <output_file_path>\n\n");
        return 0;
    }

    if((input_file_name == NULL || output_file_name == NULL) && rank == ROOT)
    {
        printf("Missing argument\n");
    }
 
    if(access(input_file_name, F_OK ) == -1) {
        if(rank == ROOT){
            printf("Input File not found\n");
        }
        return 1;
    }

    // Other Variables
    int valid_chars = 26 + get_extra_char_size();

    //--------------- FILE LOAD BEGIN ---------------//
    double file_time_start = MPI_Wtime();
    char *input_data = readFile(input_file_name);   // file to buffer
    int file_length = strlen(input_data);           // get file length
    double file_time_end = MPI_Wtime();
    //--------------- FILE LOAD END ---------------//

    printf("- Node %d read file in = %f seconds\n", rank, file_time_end - file_time_start);

    // Root Node (The Process with rank = 0)
    if (rank == ROOT)
    {
        double root_start = MPI_Wtime(); // root timer start

        // represents frequency of each 2-Gram
        int char_frequency[valid_chars][valid_chars];
        memset(char_frequency, 0, (sizeof char_frequency));

        // allocated memory for each buffer
        int node_buffer[valid_chars][valid_chars];
        memset(node_buffer, 0, (sizeof node_buffer));

        for (int rank = 1; rank < size; rank++) // receiving buffer from worker node with rank = 1,2,3...
        {
            MPI_Irecv(&node_buffer, valid_chars * valid_chars, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status); // wait for consistency
            for (int i = 0; i < valid_chars; i++)
            {
                for (int j = 0; j < valid_chars; j++)
                {
                    char_frequency[i][j] += node_buffer[i][j]; // increment main frequency table according to node_buffers
                }
            }
        }

        usleep(50);                                                                     // sleep 50ms before print results
        bigramWriteFile(output_file_name, valid_chars, (int *)char_frequency);                            // Export Outputs

        double root_end = MPI_Wtime();                                                  // Root timer end
        printf("\n- Root node time spent = %f seconds\n", root_end - root_start);       // Root time spent

        double total_time_end = MPI_Wtime();                                            // Total timer end
        printf("- Total time spent = %f seconds\n", total_time_end - total_time_start); // Total time spent
    }
    // Worker Nodes
    else
    {
        double node_start = MPI_Wtime(); // worker node timer start

        // worker node buffer creation
        int node_buffer[valid_chars][valid_chars];
        memset(node_buffer, 0, (sizeof node_buffer));

        for (int i = rank - 1; i < file_length; i += size - 1) // work work work
        {
            char c1 = input_data[i];
            char c2 = input_data[i + 1];

            int i1 = asci_to_index(c1);
            if (i1 == -1)
            {
                // printf("- Skipping 2-Gram");
                continue;
            }

            int i2 = asci_to_index(c2);
            if (i2 == -1)
            {
                // printf("- Skipping 2-Gram");
                continue;
            }

            // node_buffer increment
            node_buffer[i1][i2] += 1;
        }

        // send calculated buffer to root node
        MPI_Isend(&node_buffer, valid_chars * valid_chars, MPI_INT, ROOT, 0, MPI_COMM_WORLD, &request);

        double node_end = MPI_Wtime(); // worker node timer end
        printf("- Node %d time spent on work = %f seconds\n", rank, node_end - node_start);
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}