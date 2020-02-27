#include "mpi.h"
#include <stdio.h>

#define mc MPI_COMM_WORLD

float avg(int arr[],int n) {
  float ans = 0;
  for(int i=0; i<n; i++)
    ans = ans + arr[i];
  ans = (float)ans/n;
  return ans;
}

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  int n = size;
  int m;
  int arr[100],arr2[100];
  float x,ans[100],final_ans = 0;

  if(rank==0) {
    printf("Enter value of m:\n");
    scanf("%d",&m);
    printf("Enter %d*%d values in master:\n",n,m);
    for(int i=0; i<n*m; i++)
      scanf("%d",&arr[i]);
  }

  MPI_Bcast(&m,1,MPI_INT,0,mc);

  MPI_Scatter(arr,m,MPI_INT,arr2,m,MPI_INT,0,mc);

  x = avg(arr2,m);

  MPI_Gather(&x,1,MPI_FLOAT,ans,1,MPI_FLOAT,0,mc);
  printf("Average computed in process %d is %f\n",rank,x);
  
  for(int i=0; i<n; i++)
    final_ans+=ans[i];
  final_ans=final_ans/size;

  if(rank==0)
    printf("Value gathered by root is:%f\n",final_ans);
  
  MPI_Finalize();
  return 0;
}