#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void transpose(int *a,int *t) {
	int n = threadIdx.x;
	int m = blockIdx.x;
	int size = blockDim.x;
	int size1 = gridDim.x;
	t[n*size1 + m] = a[m*size + n];
}

int main() {
	int *a,*t,m,n,i,j;
	int *d_a,*d_t;

	printf("Enter value of m: ");
	scanf("%d",&m);

	printf("Enter value of n: ");
	scanf("%d",&n);

	int size = sizeof(int)*m*n;
	a = (int *)malloc(sizeof(int)*m*n);
	t = (int *)malloc(sizeof(int)*m*n);

	printf("Enter input matrix: ");
	for(i = 0; i<m*n; i++) {
		scanf("%d",&a[i]);
	}

	cudaMalloc((void **)&d_a,size);
	cudaMalloc((void **)&d_t,size);

	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);

	transpose<<<m,n>>>(d_a,d_t);

	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
	printf("Result matrix is: ");
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			printf("%d\t",t[i*m + j]);
		}
		printf("\n");
	}
	cudaFree(d_a);
	cudaFree(d_t);
	return 0;
}