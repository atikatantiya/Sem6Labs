#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  int rank,size,x; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size); 

  MPI_Status status;

  if(rank==0) {
    printf("Enter value in master:\n" );
    scanf("%d",&x);
    MPI_Send(&x,1,MPI_INT,1,1,MPI_COMM_WORLD);
  }
  else {
    MPI_Recv(&x,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
    printf("Received: %d\n",x);
  }
  
  MPI_Finalize();
  return 0;
}
