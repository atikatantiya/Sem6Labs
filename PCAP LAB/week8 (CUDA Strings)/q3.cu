#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void func(char *a,char *B,unsigned int *d_count,int len,int wordlen) {
	int id = blockIdx.x*blockDim.x + threadIdx.x;
	int start = 0;
	int end = 0;
	for(int j=0; j<len; ++j) {
	  if(a[j] == ' '){
	    id--;
	    if(id == 0) 
	      start = j+1;
	    if(id < 0){
	      end = j-1;
	      break;
	    }
	  }
	}
	if((end - start + 1) == wordlen) {
		int f = 1;
		int j = 0;
		while(start < end) {
			if(a[start] != B[j]) {
				f = 0;
				break;
			}
			j++;
			start++;		
		}
		if(f == 1)
			atomicAdd(d_count,1);
	}	
}

int main() {
	char a[500],b[100];

	char *d_A,*d_B;
	unsigned int count = 0, *d_count, result;

	printf("Enter string: ");
	gets(a);
	int len = strlen(a);
  	a[len++] = ' ';

	printf("Enter number of words: ");
	int n;
	scanf("%d",&n);

	gets(b);
	printf("Enter word: ");
	gets(b);
	int wordlen = strlen(b);

	cudaMalloc((void **)&d_A,strlen(a)*sizeof(char));
	cudaMalloc((void **)&d_B,strlen(b)*sizeof(char));
	cudaMalloc((void **)&d_count,sizeof(unsigned int));

	cudaMemcpy(d_A,a,strlen(a)*sizeof(char),cudaMemcpyHostToDevice);
	cudaMemcpy(d_B,b,strlen(b)*sizeof(char),cudaMemcpyHostToDevice);
	cudaMemcpy(d_count,&count,sizeof(unsigned int),cudaMemcpyHostToDevice);

	func<<<1,n>>>(d_A,d_B,d_count,len,wordlen);	

	cudaMemcpy(&result,d_count,sizeof(unsigned int),cudaMemcpyDeviceToHost);

	printf("Total occurences of %s: %u\n",b,result);	

	cudaFree(d_A);
	cudaFree(d_count);
}