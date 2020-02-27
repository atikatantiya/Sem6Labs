#include "mpi.h"
#include <stdio.h>

#define mc MPI_COMM_WORLD

int fact(int n) {
  int f = 1;
  for(int i=2; i<=n; i++)
    f = f*i;
  return f;
}

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  int n = size;
  int arr[n],arr2[n];
  int c,sum = 0;

  if(rank==0) {
    printf("Enter n values in master:\n" );
    for(int i=0; i<n; i++)
      scanf("%d",&arr[i]);
  }
  MPI_Scatter(arr,1,MPI_INT,&c,1,MPI_INT,0,mc);
  printf("I have received %d in process %d\n",c,rank);

  c = fact(c);
  printf("The factorial in process %d is: %d\n",rank,c);
  MPI_Gather(&c,1,MPI_INT,arr2,1,MPI_INT,0,mc);

  if(rank == 0) {
    printf("Result gathered in the root\n");
    for(int i=0; i<n; i++) {
      sum = sum + arr2[i];
    }
    printf("Sum of all factorials is: %d\n",sum);
  }
  
  MPI_Finalize();
  return 0;
}