__kernel void prime_check(__global int *A,__global int *C){
	int i = get_global_id(0);
	int temp = A[i];
	
	int f = 1;
	for(int j = 2; j < temp; j++) {
		if(temp % j == 0) {
			f = 0;
			break;
		}
	}
	
	if(f == 1)
		C[i] = A[i];
	else
		C[i] = A[i] * A[i];
}
