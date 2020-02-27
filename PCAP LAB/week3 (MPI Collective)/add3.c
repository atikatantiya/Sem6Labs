#include "mpi.h"
#include <stdio.h>
#include <math.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  int n = size;
  int m;
  int arr[100],arr2[100];
  int ans[400];

  if(rank==0) {
    printf("Enter value of m:\n");
    scanf("%d",&m);
    printf("Enter %d*%d values in master:\n",n,m);
    for(int i=0; i<n*m; i++)
      scanf("%d",&arr[i]);
  }

  MPI_Bcast(&m,1,MPI_INT,0,mc);

  MPI_Scatter(arr,m,MPI_INT,arr2,m,MPI_INT,0,mc);

  for(int i=0; i<m; i++) {
    arr2[i] = (arr2[i])*(arr2[i]+1);
    arr2[i] = arr2[i]/2;
  }

  MPI_Gather(arr2,m,MPI_INT,ans,m,MPI_INT,0,mc);

  if(rank==0) {
    for(int i=0; i<m*n; i++)
      printf("%d ",ans[i]);
    printf("\n");
  }  
  
  MPI_Finalize();
  return 0;
}