#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void error_msg(int rank){
    printf("memory error in processor %d\n", rank);
}

int main(int argc, char** argv){

    #define N (1000)
    #define ROOT (0)

    int rank, size;
    int buffer[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int segment = N / size; // size assigned to each section

    if(rank == ROOT){
	// initialize buffer
        for(int i = 0; i < N; i++) buffer[i] = i + 1;

        // broadcast values
	// MPI_Bcast(buffer, size, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    MPI_Bcast(buffer, N, MPI_INT, ROOT, MPI_COMM_WORLD);

    const int offset = rank * segment;
    const int limit = offset + segment;
    int localsum = 0;
    int globalsum;

    for(int i = offset; i < limit; i++){
        localsum += buffer[i];
    }
    printf("Partial sum at processor %d is %d\n", rank, localsum);

    MPI_Reduce(&localsum, &globalsum, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

    if(rank == ROOT){
        printf("The sum is %d\n", globalsum);
    }




    MPI_Finalize();
}