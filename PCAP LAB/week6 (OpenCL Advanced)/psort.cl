__kernel void psort(__global int *inp, int size, __global int *sorted) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 	int pos = 0;
 	for(int j=0; j<size; ++j){
 		if(inp[j] <= inp[i]) pos++;
 	}
 	sorted[pos-1] = inp[i];
}