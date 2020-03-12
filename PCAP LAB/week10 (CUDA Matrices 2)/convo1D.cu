#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void func(float *N,float *M,float *P,int mwidth, int width) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	float pval = 0.0;
	int N_start_point = i - (mwidth/2);
	for(int j = 0; j < mwidth; j++) {
		if((N_start_point + j) >= 0 && (N_start_point + j) < width) {
			pval = pval + N[N_start_point + j] * M[j];
			//printf("Multiplying: %f and %f\n",N[N_start_point + j],M[j]);
		}
	}
	P[i] = pval;
} 

int main() {
	int i,j;
	float *d_n,*d_m,*d_p;

	int width,mwidth;

	printf("Enter size of input: ");
	scanf("%d",&width);
	printf("Enter size of mask (has to be odd): ");
	scanf("%d",&mwidth);

	int size = sizeof(float);
	float n[width];
	float m[mwidth];

	printf("Enter input vector: ");
	for(i = 0; i<width; i++) {
		scanf("%f",&n[i]);
	}

	printf("Enter mask: ");
	for(i = 0; i<mwidth; i++) {
		scanf("%f",&m[i]);
	}

	float p[width];

	cudaMalloc((void **)&d_n,size*width);
	cudaMalloc((void **)&d_m,size*mwidth);
	cudaMalloc((void **)&d_p,size*width);

	cudaMemcpy(d_n,n,size*width,cudaMemcpyHostToDevice);
	cudaMemcpy(d_m,m,size*mwidth,cudaMemcpyHostToDevice);

	dim3 dimGrid(((width - 1) / mwidth) + 1,1,1);
	dim3 dimBlock(mwidth,1,1);
	func<<<dimGrid,dimBlock>>>(d_n,d_m,d_p,mwidth,width);

	cudaMemcpy(p,d_p,size*width,cudaMemcpyDeviceToHost);

	printf("Result vector is: \n");
	for(j = 0; j <width; j++) {
		printf("%4f\n",p[j]);
	}
	printf("\n");

	cudaFree(d_m);
	cudaFree(d_n);
	cudaFree(d_p);
	return 0;
}