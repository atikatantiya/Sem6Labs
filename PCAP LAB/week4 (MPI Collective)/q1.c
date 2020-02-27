#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "err.h"

#define mc MPI_COMM_WORLD
#define intt MPI_INT
#define charr MPI_CHAR
#define floatt MPI_FLOAT

int main() {
	int rank,size;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(mc,&rank);
	MPI_Comm_size(mc,&size);

	MPI_Errhandler_set(mc,MPI_ERRORS_RETURN);

	int fact = 1;
	for(int i = 1; i<=(rank+1); i++)
		fact = fact * i;

	int factsum;
	int errc = MPI_Scan(&fact,&factsum,1,intt,MPI_SUM,mc);
	handle(errc);
	
	printf("Process %d and sum of the factorials is %d\n",rank,factsum);

	MPI_Finalize();
}