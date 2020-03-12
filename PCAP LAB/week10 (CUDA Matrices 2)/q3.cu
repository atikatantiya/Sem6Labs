#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void func(int *dataArr,int *x,int *ptr,int *y,int *index) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int val = 0;
	for(int j = ptr[i]; j < ptr[i+1]; j++) {
		val = val + dataArr[j]*x[index[j]];
	}
	y[i] = val;
} 

int main() {
	int m,n,i,j;
	int *d_a,*d_x,*d_y,*d_ptr,*d_index;

	printf("Enter value of m: ");
	scanf("%d",&m);
	printf("Enter value of n: ");
	scanf("%d",&n);

	int size = sizeof(int);
	int a[m][n];
	int x[n];
	int y[n];
	int ptr[100];
	int ptrid = 0;
	int index[100];
	int indexid = 0;
	int dataArr[100];
	int temp,f;

	printf("Enter input matrix: ");
	for(i = 0; i<m; i++) {
		f = 0;
		for(j = 0; j<n; j++) {
			scanf("%d",&a[i][j]);
			if(a[i][j] != 0) {
				if(f == 0)
					temp = indexid;
				f = 1;
				index[indexid] = j;
				dataArr[indexid] = a[i][j];
				indexid++;
			}
		}		
		if(f == 1) {
			ptr[ptrid] = temp;
			ptrid++;
		}
	}
	ptr[ptrid] = indexid;
	ptrid++;

	printf("Enter input vector: ");
	for(i = 0; i<n; i++) {
		scanf("%d",&x[i]);		
	}

	cudaMalloc((void **)&d_a,size*indexid);
	cudaMalloc((void **)&d_x,size*n);
	cudaMalloc((void **)&d_y,size*n);
	cudaMalloc((void **)&d_ptr,size*ptrid);
	cudaMalloc((void **)&d_index,size*indexid);

	cudaMemcpy(d_a,dataArr,size*indexid,cudaMemcpyHostToDevice);
	cudaMemcpy(d_x,x,size*n,cudaMemcpyHostToDevice);
	cudaMemcpy(d_ptr,ptr,size*ptrid,cudaMemcpyHostToDevice);
	cudaMemcpy(d_index,index,size*indexid,cudaMemcpyHostToDevice);

	func<<<1,n>>>(d_a,d_x,d_ptr,d_y,d_index);

	cudaMemcpy(y,d_y,size*n,cudaMemcpyDeviceToHost);

	printf("Result vector is: \n");
	for(j = 0; j < n; j++) {
		printf("%d\t",y[j]);
	}
	printf("\n");

	cudaFree(d_a);
	cudaFree(d_x);
	cudaFree(d_y);
	cudaFree(d_ptr);
	cudaFree(d_index);
	return 0;
}