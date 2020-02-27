__kernel void one_complement(__global int *A,__global int *C){
	int i= get_global_id(0);
	int temp = A[i];
	int rem;
	int p = 1;
	int num = 0;
	
	while( temp > 0 ) {
		rem = temp%10;
		rem = 1 - rem;
		num = num + rem*p;
		p = p * 10;
		temp = temp / 10;
	}
	
	C[i] = num;		
}
