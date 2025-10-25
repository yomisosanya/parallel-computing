#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define N 256
#define ROOT 0

int main(int argc, char** argv){
    int rank, size;
    int buffer[N];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    

        if(rank == ROOT){
            // initialize buffer
	    for(int i = 0; i < N; i++) buffer[i] = i + 1;
        }

    const int levels = (int) log2(size);

    for(int i = 0, p = size, n = N/2; i < levels; i++, p /= 2, n /= 2){
        // iterate by the number of the levels
        if((rank % p) == 0){
	    // divide the chunk into 2 halves
            // parent rank
	    int child = rank + (p/2);
	    MPI_Send(&buffer[n], n, MPI_INT, child, 0, MPI_COMM_WORLD);
            printf("Processor %d sends %d data to processor %d\n", rank, n, child);
	}else{
	    int offset = p/2;
	    if((rank % offset) == 0){
	        int parent = rank - offset;
		MPI_Recv(buffer, n, MPI_INT, parent, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Processor %d received %d data from processor %d\n", rank, n, parent);
	    }
	}
    }

    const int segment = N/size;
    int localsum = 0;

    // local sum
    for(int i = 0; i < segment; i++) localsum += buffer[i];
    
//    printf("The local sum at processor %d is %d\n", rank, localsum);

    for(int i = levels, p = 2; i > 0; i--, p *= 2){
        if((rank % p) == 0){
	    int child = rank + (p/2);
	    int temp;
	    MPI_Recv(&temp, 1, MPI_INT, child, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    printf("Processor %d recieved partial sum %d from processor %d\n", rank, temp, child);
	    localsum += temp;
	}else{
	    int offset = p/2;
	    if((rank % offset) == 0){
	        int parent = rank - offset;
		MPI_Send(&localsum, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
		printf("Processor %d sent partial sum %d to processor %d\n", rank, localsum, parent);
	    }
	}
    }
    
    if(rank == ROOT){
        printf("The sum is %d\n", localsum);
    } 




//    for(int i = 0; i < levels; i++){
//        const int mask = (int) pow(2, i);
//
//      if(rank & mask){
//		int parent = rank - mask;
//		MPI_Send(localsum, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
//	}else{
//	    if(rank & 
//	}
//    }

    MPI_Finalize();
    return 0;
}