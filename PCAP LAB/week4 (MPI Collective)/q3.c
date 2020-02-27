#include <stdio.h>
#include <mpi.h>
#include "err.h"
#define mc MPI_COMM_WORLD
#define intt MPI_INT
#define charr MPI_CHAR
#define floatt MPI_FLOAT

int main(int argc, char *argv[]) {
	int rank, size, a[9],arr[3],ele,count=0,countsum;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(mc, &rank);
	MPI_Comm_size(mc, &size);

	if(rank==0)	{
		printf("Enter the matrix : \n");
		for(int i=0; i<9; i++)
			scanf("%d",&a[i]);
		printf("Enter the element : \n");
		scanf("%d",&ele);
	}

	MPI_Bcast(&ele,1,intt,0,mc);

	MPI_Scatter(a,3,intt,arr,3,intt, 0, mc);

	for(int j=0; j<3; j++) {
			if(arr[j] == ele)
				count++;
	}

	int errc = MPI_Reduce(&count,&countsum,1,intt, MPI_SUM,0,mc);
	handle(errc);

	if(rank == 0)	
		printf("Number of occurences= %d\n",countsum);

	MPI_Finalize();
	return 0;
}