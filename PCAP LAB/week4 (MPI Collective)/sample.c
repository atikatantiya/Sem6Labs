#include "mpi.h"
#include <stdio.h>

#define mc MPI_COMM_WORLD
#define intt MPI_INT
#define charr MPI_CHAR
#define floatt MPI_FLOAT

int main() {
	int rank,size;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(mc,&rank);
	MPI_Comm_size(mc,&size);

	int fact = 1;
	for(int i = 1; i<=(rank+1); i++)
		fact = fact * i;

	int factsum;
	MPI_Reduce(&fact,&factsum,1,intt,MPI_SUM,0,mc);
	if(rank == 0)
		printf("Sum of the factorials is %d\n",factsum);

	MPI_Finalize();
}