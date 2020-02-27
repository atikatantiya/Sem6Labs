__kernel void bin_to_dec(__global int *A,__global int *C){
	int i = get_global_id(0);
	int temp = A[i];
	int rem;
	int p = 1;
	int num = 0;
	
	while( temp > 0 ) {
		rem = temp%10;
		num = num + rem*p;
		p = p * 2;
		temp = temp / 10;
	}
	
	C[i] = num;		
}
