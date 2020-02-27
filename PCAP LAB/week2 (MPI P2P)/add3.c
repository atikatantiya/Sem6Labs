#include "mpi.h"
#include <stdio.h>

#define mc MPI_COMM_WORLD

int fac(int x) {
  int f = 1;
  for(int i=2; i<=x; i++)
    f = f*i;
  return f;
}

int main(int argc, char *argv[]) {
  int rank,size,x,sum = 0,temp,tosend; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  MPI_Status status;

  if(rank == 0) {
    temp = rank + 1;
    sum = sum + fac(temp);
    for(int i=1; i<size; i++) {
      MPI_Recv(&x,1,MPI_INT,i,i,mc,&status);
      sum = sum + x;
    }
    printf("Sum is %d\n",sum);
  }
  else {
    temp = rank + 1;
    if(rank%2 == 0) 
      tosend = fac(temp);
    else 
      tosend = ((temp)*(temp+1))/2;
    MPI_Send(&tosend,1,MPI_INT,0,rank,mc);
  }
  
  MPI_Finalize();
  return 0;
}
