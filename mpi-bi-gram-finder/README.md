[<<< home](../README.md)
* MPI BI-Gram Finder
* [MPI Clustering](../mpi-clustering/README.md)
* [MPI Pi Number Calculation](../mpi-pi-number-calculation/README.md)

# MPI BI-Gram Finder
Parallel Computing Course (CSE452) Project

The main motivation of the projct is finding bi-grams in the given txt file as input to the program.

## Usage

* To directly run use following make command
  ```bash
  make [np=<NUMBER_OF_PARALLEL_PROCESSES>] [input=<INPUT_TXT_FILE>] [output=<OUTPUtTXT_FILE>]
  ```

* To build executable and run later; first build executable
  ```bash
  make build
  ```
* Then you can use executable
  ```bash
   Usage: mpirun [-np] project -input <arg1> -output <arg2>
   Options:
        np: number of parallel nodes
        input: txt file that contains the text which you want file bi-grams
        output: the file that program output will be written
  ```