#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void error_msg(int rank){
    printf("memory error in processor %d\n", rank);
}

int main(int argc, char** argv){

    #define N (1000)
    #define ROOT (0)
    #define MEM_ERR 25


    int rank, size;
    int *buffer;

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int segment = N / size; // size assign to each processor

    if(rank == ROOT){
        buffer = (int*) calloc(N, sizeof(int));
        if(buffer == NULL){
	    MPI_Finalize();
	    error_msg(rank);
	    return 1;
	}
        for(int i = 0; i < N; i++) buffer[i] = i+1;

        // 
        for(int i = 1, offset = segment; i < size; i++, offset += segment){
            MPI_Send(&buffer[offset], segment, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }else{
        buffer = (int*) calloc(segment, sizeof(int));
        if(buffer == NULL){
            MPI_Finalize();
            error_msg(rank);
	    return 1;
        }

        MPI_Recv(buffer, segment, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int sum = 0;

    for(int i = 0; i < segment; i++){
        sum += buffer[i];
    }

    // free dynamic resources
    free(buffer);

    printf("Partial sum from processor %d is: %d\n", rank, sum);

    if(rank == ROOT){
        int temp = 0;
        for(int i = 1; i < size; i++){
            MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += temp;
	    printf("Received %d from processor %d\n", temp, i);
        }

        printf("The parallel sum is %d\n", sum);
    }else{
        printf("Send %d to processor 0 from processor %d\n", sum, rank);
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}
