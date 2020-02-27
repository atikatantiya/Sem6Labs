#include "mpi.h"
#include <stdio.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size,x; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  MPI_Status status;

  if(rank==0) {
    for(int i=1; i<size; i++) {
      x = i;
      MPI_Send(&x,1,MPI_INT,i,i,mc);
    }
  }
  else {
    MPI_Recv(&x,1,MPI_INT,0,rank,mc,&status);
    printf("Rank:%d x:%d\n",rank,x);
  }
  
  MPI_Finalize();
  return 0;
}
