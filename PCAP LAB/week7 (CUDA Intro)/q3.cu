#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__global__ void add(int *a,int *b,int *c) {
	int idx = blockIdx.x*blockDim.x + threadIdx.x;
	b[idx] = (a[idx]*(*c)) + b[idx];
}

int main() {
	printf("Enter the number of elements: ");
	int n;
	scanf("%d",&n);
	int *a = (int *)malloc(sizeof(int)*n);
	int *b = (int *)malloc(sizeof(int)*n);

	int alpha;
	printf("Enter value of alpha: ");
	scanf("%d",&alpha);

	int *d_a = (int *)malloc(sizeof(int)*n);
	int *d_b = (int *)malloc(sizeof(int)*n);
	int size = n*sizeof(int);
	
	int size2 = sizeof(int);
	int *d_alpha;

	//allocate space for device copies
	cudaMalloc((void **)&d_a,size);
	cudaMalloc((void **)&d_b,size);
	cudaMalloc((void **)&d_alpha,size2);


	//setup i/p
	for(int i=0; i<n; i++) {
		a[i] = i;
		b[i] = i+10;
	}

	//copy ips to device
	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_alpha,&alpha,size2,cudaMemcpyHostToDevice);

	//launch add()
	add<<<n,1>>>(d_a,d_b,d_alpha);

	//copy result to host
	cudaMemcpy(b,d_b,size,cudaMemcpyDeviceToHost);
	for(int i=0; i<n; i++) {
		printf("%d\t",b[i]);
	}
	printf("\n");

	//cleanup

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_alpha);
}