#include<stdio.h>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>
#include<string.h>

__global__ void conc(char *a,int len) {
  int id = blockIdx.x*blockDim.x + threadIdx.x;
  int start = 0;
  int end = 0;
  for(int j=0; j<len; ++j) {
    if(a[j] == ' '){
      id--;
      if(id == 0) 
        start = j+1;
      if(id < 0){
        end = j-1;
        break;
      }
    }
  }
  while(start < end){
    id = a[start];
    a[start++] = a[end];
    a[end--] = id;
  }
}

int main() {
  int n,len;
  char a[100],b[100];

  printf("Enter str:");
  gets(a);

  printf("Enter n: ");
  scanf("%d",&n);

  len = strlen(a);
  a[len++] = ' ';

  char *d_a;
  memset(b,0,100);

  cudaMalloc((void**)&d_a,len);

  cudaMemcpy(d_a,&a,len,cudaMemcpyHostToDevice);

  conc<<<1,n>>>(d_a,len);

  cudaMemcpy(&a,d_a,len,cudaMemcpyDeviceToHost);

  printf("%s\n",a);

  cudaFree(d_a);
}
