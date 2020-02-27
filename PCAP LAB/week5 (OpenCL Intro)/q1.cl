__kernel void dec_to_oct(__global int *A,__global int *C){
	int i= get_global_id(0);
	int num = 0;
	int temp = A[i];
	int rem;
	
    while (temp != 0) {  
        rem = temp % 8; 
        num = num*10 + rem;
        temp = temp / 8; 
    }
    
    int rev = 0;
    while(num > 0) {
    	rem = num%10;
    	rev  = rev*10 + rem;
    	num = num/10;
    }
    
    C[i] = rev; 
}