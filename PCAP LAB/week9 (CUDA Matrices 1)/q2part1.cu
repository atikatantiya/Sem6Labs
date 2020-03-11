#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void func(int *a,int *b,int *t,int cols) {
	int id = threadIdx.x*cols;
	for(int i = 1; i <= cols; i++) {
		t[id] =  a[id] + b[id];
		id++;
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

	func<<<1,m>>>(d_a,d_b,d_t,n);

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