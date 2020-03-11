#include<stdio.h>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>

__device__ int invBin(int x) {
  int arr[100];

  int temp = x;
  int val = 0,i = 0;

  while(temp > 0) {
    arr[i++] = temp%2;
    temp = temp / 2;
  }

  for(int j=i-1; j>=0; j--)
    val = val*10 + !arr[j];

  return val;
}

__global__ void add(int *a,int *b,int m,int n) {
  int row = threadIdx.x;
  int col = threadIdx.y;
  if(row == 0 || row == m-1 || col == 0 || col == n-1) {
    b[row*n+col] = a[row*n+col];
  }
  else {
    b[row*n+col] = invBin(a[row*n+col]);
  }
}

int main() {
  int m,n,i,j;
  int *d_a,*d_b;

  printf("Enter value of m: ");
  scanf("%d",&m);
  printf("Enter value of n: ");
  scanf("%d",&n);

  int size = sizeof(int)*n*m;
  int a[m][n];
  int b[m][n];

  printf("Enter input matrix 1: ");
  for(i = 0; i<m; i++) {
    for(j = 0; j<n; j++) {
      scanf("%d",&a[i][j]);
    }   
  }

  cudaMalloc((void **)&d_a,size);
  cudaMalloc((void **)&d_b,size);

  cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);

  dim3 block(m,n,1);
  add<<<1,block>>>(d_a,d_b,m,n);

  cudaMemcpy(&b,d_b,size,cudaMemcpyDeviceToHost);

  printf("Result matrix is: \n");
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++) {
      printf("%d\t",b[i][j]);
    }
    printf("\n");
  }

  cudaFree(d_a);
  cudaFree(d_b);
}
