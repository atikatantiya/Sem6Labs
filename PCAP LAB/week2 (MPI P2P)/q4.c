#include "mpi.h"
#include <stdio.h>


#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size,x; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  MPI_Status status;

  if(rank == 0) {
    printf("Enter value in master: \n");
    scanf("%d",&x);
    MPI_Send(&x,1,MPI_INT,1,1,mc);
    MPI_Recv(&x,1,MPI_INT,size-1,size,mc,&status);
    printf("Master received %d from last process\n",x);
  }
  else {
    MPI_Recv(&x,1,MPI_INT,rank-1,rank,mc,&status);
    printf("Rank:%d x:%d\n",rank,x);
    x = x + 1;
    if(rank == (size-1))
      MPI_Send(&x,1,MPI_INT,0,rank+1,mc);
    else
      MPI_Send(&x,1,MPI_INT,rank+1,rank+1,mc);
  }
  
  MPI_Finalize();
  return 0;
}
