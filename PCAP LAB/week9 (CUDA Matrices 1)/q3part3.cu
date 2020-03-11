#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void mult(int *a,int *b,int *c,int wa,int wb) {
  int col = threadIdx.x;
  int row = threadIdx.y;
  int sum = 0;
  for(int i=0; i<wa; i++)
    sum = sum + a[row*wa+i]*b[i*wb+col];
  c[row*wb+col] = sum;
}

int main() {
  int ha,wa,wb; 
  printf("Enter ha,wa,wb: ");
  scanf("%d %d %d",&ha,&wa,&wb);

  int a[ha][wa],b[wa][wb]; 
  int c[ha][wb];

  printf("Enter A:\n");
  for(int i=0; i<ha; i++){
    for(int j = 0; j < wa; j++) {
        scanf("%d",&a[i][j]);
    }
  }

  printf("Enter B:\n");
  for(int i=0; i<wa; i++){
    for(int j = 0; j < wb; j++) {
        scanf("%d",&b[i][j]);
    }
  }

  int *d_a,*d_b,*d_c;
  int size = sizeof(int); 

  cudaMalloc((void**)&d_a,size*ha*wa);
  cudaMalloc((void**)&d_b,size*wa*wb);
  cudaMalloc((void**)&d_c,size*ha*wb);

  cudaMemcpy(d_a,&a,size*ha*wa,cudaMemcpyHostToDevice);
  cudaMemcpy(d_b,&b,size*wa*wb,cudaMemcpyHostToDevice);

  dim3 block(wb,ha,1);
  mult<<<1,block>>>(d_a,d_b,d_c,wa,wb);
  
  cudaMemcpy(&c,d_c,size*ha*wb,cudaMemcpyDeviceToHost);

  printf("C:\n");
  for(int i=0; i<ha; i++) {
    for(int j=0; j<wb; j++)
      printf("%d ",c[i][j]);
    printf("\n");
  }
  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);
}
