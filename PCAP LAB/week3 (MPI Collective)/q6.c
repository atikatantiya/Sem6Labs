#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  int n = 4;
  int len;
  int arr[400],arr2[400],ans[400];
  int slice;

  if(rank == 0) {
    printf("Enter number of elements in array(should be multiple of 4): \n");
    scanf("%d",&len);
    if(len%n != 0) {
      printf("Array length should be divisible by 4!!\n");
      exit(0);
    }
    printf("Enter array elements: \n");
    for(int i=0; i<len; i++)
      scanf("%d",&arr[i]);
    slice = len/n;
  }

  MPI_Bcast(&slice,1,MPI_INT,0,mc);

  MPI_Scatter(arr,slice,MPI_INT,arr2,slice,MPI_INT,0,mc);

  for(int i=1; i<slice; i++) {
    arr2[i] = arr2[i] + arr2[i-1];
  }  

  MPI_Gather(arr2,slice,MPI_INT,ans,slice,MPI_INT,0,mc);

  if(rank==0) {
    printf("The array is:\n");
    for(int i=0; i<len; i++) {
      printf("%d ",ans[i]);
    }
    printf("\n");
  }
  
  MPI_Finalize();
  return 0;
}