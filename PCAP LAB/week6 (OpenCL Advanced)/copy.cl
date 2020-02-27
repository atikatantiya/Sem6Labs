__kernel void copy(__global char *ip, int len, __global char *res) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
 	int offset = len*i;
 	for(int i=0;i<len;++i)
 		res[offset + i] = ip[i];
}