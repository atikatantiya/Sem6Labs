__kernel void swapper(__global int *A,__global int *C){
	int i = get_global_id(0);
	if(i%2 == 0)
		C[i+1] = A[i];
	else
		C[i-1] = A[i];
}
