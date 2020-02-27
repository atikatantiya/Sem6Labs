#include "mpi.h"
#include <stdio.h>
int isprime(int n) {
	if(n == 2 || n == 3)
		return 1;
	for(int i=2; i*i<=n; i++) {
		if(n%i == 0)
			return 0;
	}
	return 1;
}
int main(int argc,char *argv[]) {
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank == 0) {
		for(int i=2; i<=50; i++) {
			if(isprime(i) == 1)
				printf("%d ",i);
		}
		printf("\n");
	}
	else if (rank == 1) {
		for(int i=51; i<=100; i++) {
			if(isprime(i) == 1)
				printf("%d ",i);
		}
		printf("\n");
	}
	MPI_Finalize();
	return 0;
}