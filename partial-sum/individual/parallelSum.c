#include <mpi.h>


int main(int argc, char** argv){

    #define N (1000)

    MPI_Init(&argc, &argv);
    MPI_Finalize();
    return 0;
}