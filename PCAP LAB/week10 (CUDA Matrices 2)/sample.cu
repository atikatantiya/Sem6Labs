#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BW 2
#define TW 2
#define W 4

__device__ int getTid() {
	int blockSkip = (blockIdx.y * gridDim.x * blockDim.x * blockDim.y);
	int rowSkip = (threadIdx.y * gridDim.x * blockDim.x);
	int rowDisp = (blockIdx.x * blockDim.x) + threadIdx.x;
	int tid = blockSkip + rowSkip + rowDisp;
	return tid;
}

__global__ void func(int *a,int *b,int *t) {
	int tid = getTid();
	t[tid] = a[tid] + b[tid];
} 

int main() {
	int i,j;
	int *d_a,*d_b,*d_t;

	//m = n = 4
	int m,n;
	m = n = W;

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

	int numblocks = W / BW;
	dim3 dimGrid(numblocks,numblocks,1);
	dim3 dimBlock(BW,BW,1);
	func<<<dimGrid,dimBlock>>>(d_a,d_b,d_t);

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