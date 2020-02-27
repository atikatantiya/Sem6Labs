#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__global__ void add(float *a,float *b) {
	int idx = blockIdx.x*blockDim.x + threadIdx.x;		
	b[idx] = sinf(a[idx]);
}

int main() {

	printf("Enter the number of elements: ");
	int n;
	scanf("%d",&n);

	float *a = (float *)malloc(sizeof(float)*n);
	float *b = (float *)malloc(sizeof(float)*n);

	//setup i/p
	printf("Enter radian values: ");
	for(int i=0; i<n; i++) {
		scanf("%f",&a[i]);
	}

	float *d_a = (float *)malloc(sizeof(float)*n);
	float *d_b = (float *)malloc(sizeof(float)*n);

	int size = n*sizeof(float);

	//allocate space for device copies
	cudaMalloc((void **)&d_a,size);
	cudaMalloc((void **)&d_b,size);	

	//copy ips to device
	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);

	//launch add()
	add<<<n,1>>>(d_a,d_b);

	//copy result to host
	cudaMemcpy(b,d_b,size,cudaMemcpyDeviceToHost);
	for(int i=0; i<n; i++) {
		printf("%f\t",b[i]);
	}
	printf("\n");

	//cleanup
	cudaFree(d_a);
	cudaFree(d_b);
}