__kernel void reverse(__global char *ip, int len) {

    // Get the index of the current element to be processed
    int i = get_global_id(0);
    int start = 0;
    int end = 0;
 	for(int j=0;j<len;++j) {
 		if(ip[j] == ' '){
 			i--;
 			if(i == 0) start = j+1;
 			if(i < 0){
 				end = j-1;
 				break;
 			}
 		}
 	}
 	while(start < end){
 		i = ip[start];
 		ip[start++] = ip[end];
 		ip[end--] = i;
 	}
}