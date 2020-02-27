#include "mpi.h"
#include <stdio.h>
int main(int argc,char *argv[]) {
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int a,b;
	a = 16;
	b = 4;
	printf("a = 16 and b = 4\n");
	if(rank == 0) 
		printf("Rank is %d and operation is addition with result %d\n",rank,a+b);				
	else if(rank == 1) 
		printf("Rank is %d and operation is subtraction with result %d\n",rank,a-b);			
	else if(rank == 2) 
		printf("Rank is %d and operation is multiplication with result %d\n",rank,a*b);			
	else if(rank == 3) 
		printf("Rank is %d and operation is division with result %d\n",rank,a/b);
	MPI_Finalize();
	return 0;
}