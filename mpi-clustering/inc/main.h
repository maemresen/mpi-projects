#include "glo.h"

// system libraries
#include <mpi.h>
#include <string.h>
#include <unistd.h>

// helper libraries
#include "arg.h"
#include "readfile.h"
#include "writefile.h"
#include "filter.h"

// constant definitions
#define ROOT 0  // Rank of the Root node
#define FILE_NAME_MAX_LEN 100

int main(int argc, char **argv);