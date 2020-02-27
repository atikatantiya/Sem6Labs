#include <stdio.h>
#include <mpi.h>
#include "err.h"
#define mc MPI_COMM_WORLD
#define intt MPI_INT
#define charr MPI_CHAR
#define floatt MPI_FLOAT

int main(int argc, char *argv[]) {
	int rank, size, a[16],arr1[4],arr2[4],i,j;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(mc, &rank);
	MPI_Comm_size(mc, &size);

	if(rank == 0)	{
		printf("Enter the matrix : \n");
		for(i=0; i<16; i++)		{
			scanf("%d",&a[i]);
		}
		printf("\n");
	}

	MPI_Scatter(a,4,intt,arr1,4,intt, 0, mc);
	
	int errc = MPI_Scan(arr1,arr2,4,intt, MPI_SUM,mc);
	handle(errc);

	MPI_Barrier(mc);

	for(int i=0; i<4; i++) {
			printf( "%d ",arr2[i]);
	}
	printf("\n");

	MPI_Finalize();
	return 0;
}