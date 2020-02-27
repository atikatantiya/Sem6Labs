#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__global__ void add(int *a,int *b,int *c) {
	int idx = blockIdx.x*blockDim.x + threadIdx.x;
	c[idx] = a[idx] + b[idx];
}

int main() {
	printf("Enter the number of elements: (multiple of 5) ");
	int n;
	scanf("%d",&n);
	int *a = (int *)malloc(sizeof(int)*n);
	int *b = (int *)malloc(sizeof(int)*n);
	int *c = (int *)malloc(sizeof(int)*n);

	int *d_a = (int *)malloc(sizeof(int)*n);
	int *d_b = (int *)malloc(sizeof(int)*n);
	int *d_c = (int *)malloc(sizeof(int)*n);
	int size = n*sizeof(int);

	//allocate space for device copies
	cudaMalloc((void **)&d_a,size);
	cudaMalloc((void **)&d_b,size);
	cudaMalloc((void **)&d_c,size);

	//setup i/p
	for(int i=0; i<n; i++) {
		a[i] = i;
		b[i] = i+10;
	}

	//copy ips to device
	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);

	//launch add()
	int blocks = n/5;
	add<<<blocks,5>>>(d_a,d_b,d_c);

	//copy result to host
	cudaMemcpy(c,d_c,size,cudaMemcpyDeviceToHost);
	for(int i=0; i<n; i++) {
		printf("%d\t",c[i]);
	}
	printf("\n");

	//cleanup
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
}