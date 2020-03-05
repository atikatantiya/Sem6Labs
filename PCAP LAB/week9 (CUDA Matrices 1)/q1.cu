#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void func(int *a,int *t) {
	int id = threadIdx.y*blockDim.x + threadIdx.x;
	int power = threadIdx.y + 1;
	t[id] = 1;
	for(int i = 1; i <= power; i++)
		t[id] = t[id] * a[id];
}

int main() {
	int n,i,j;
	int *d_a,*d_t;

	printf("Enter value of n: ");
	scanf("%d",&n);

	int size = sizeof(int)*n*n;
	int a[n][n];
	int t[n][n];

	printf("Enter input matrix: ");
	for(i = 0; i<n; i++) {
		for(j = 0; j<n; j++) {
			scanf("%d",&a[i][j]);
		}		
	}

	cudaMalloc((void **)&d_a,size);
	cudaMalloc((void **)&d_t,size);

	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);

	dim3 dimGrid(1,1,1);
	dim3 dimBlock(n,n,1);
	func<<<dimGrid,dimBlock>>>(d_a,d_t);

	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
	printf("Result matrix is: \n");
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			printf("%d\t",t[i][j]);
		}
		printf("\n");
	}
	cudaFree(d_a);
	cudaFree(d_t);
	return 0;
}