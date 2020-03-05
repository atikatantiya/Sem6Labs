#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void func(int *a,int *b,int *t,int rows,int cols) {
	int id = threadIdx.y;
	for(int i = 0; i < rows; i++) {
		t[id] =  a[id] + b[id];
		id = id + cols;
	}
} 

int main() {
	int m,n,i,j;
	int *d_a,*d_b,*d_t;

	printf("Enter value of m: ");
	scanf("%d",&m);
	printf("Enter value of n: ");
	scanf("%d",&n);

	int size = sizeof(int)*n*m;
	int a[m][n];
	int b[m][n];
	int t[m][n];

	printf("Enter input matrix 1: ");
	for(i = 0; i<m; i++) {
		for(j = 0; j<n; j++) {
			scanf("%d",&a[i][j]);
		}		
	}

	printf("Enter input matrix 2: ");
	for(i = 0; i<m; i++) {
		for(j = 0; j<n; j++) {
			scanf("%d",&b[i][j]);
		}		
	}

	cudaMalloc((void **)&d_a,size);
	cudaMalloc((void **)&d_b,size);
	cudaMalloc((void **)&d_t,size);

	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);

	dim3 dimGrid(1,1,1);
	dim3 dimBlock(1,n,1);
	func<<<dimGrid,dimBlock>>>(d_a,d_b,d_t,m,n);

	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
	printf("Result matrix is: \n");
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			printf("%d\t",t[i][j]);
		}
		printf("\n");
	}
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_t);
	return 0;
}