#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size,t; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  MPI_Status status;  

  int *arr = (int *)malloc(sizeof(int)*size);
  char *buf = (char *)malloc(sizeof(int)*100);
  int s = 100;
  MPI_Buffer_attach(buf,s);

  if(rank == 0) {
    printf("Enter array of %d elements:\n",size);
    for(int i=0; i<size; i++)
      scanf("%d",&arr[i]);
    for(int i=1; i<=size-1; i++)
      MPI_Bsend(&arr[i-1],1,MPI_INT,i,i,mc);
  }
  else {
    MPI_Recv(&t,1,MPI_INT,0,rank,mc,&status);
    if(rank%2 == 0)
      printf("Rank:%d Val:%d\n",rank,t*t);
    else
      printf("Rank:%d Val:%d\n",rank,t*t*t);
  }
  MPI_Buffer_detach(&buf,&s);
  MPI_Finalize();
  return 0;
}
