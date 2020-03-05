#include<stdio.h>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>
#include<string.h>

__global__ void conc(char *a,int len,char *b) {
  int id = blockIdx.x*blockDim.x + threadIdx.x;
  for(int i=0; i<len; i++)
    b[id*len+i] = a[i];
}

int main() {
  int n,len;
  char a[100],b[500];

  printf("Enter string:");
  gets(a);

  printf("Enter n: ");
  scanf("%d",&n);

  len = strlen(a);

  char *d_a,*d_b;
  memset(b,0,100);

  cudaMalloc((void**)&d_a,len);
  cudaMalloc((void**)&d_b,len*n);

  cudaMemcpy(d_a,&a,len,cudaMemcpyHostToDevice);
  cudaMemcpy(d_b,&b,len*n,cudaMemcpyHostToDevice);

  conc<<<1,n>>>(d_a,len,d_b);

  cudaMemcpy(&b,d_b,len*n,cudaMemcpyDeviceToHost);

  printf("%s\n",b);

  cudaFree(d_a);
  cudaFree(d_b);
}
