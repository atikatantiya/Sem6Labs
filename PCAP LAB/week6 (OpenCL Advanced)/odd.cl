__kernel void odd(__global int *inp){
	int i = get_global_id(0)*2+1;
	int size = get_global_size(0)*2;
	int tmp;
	if((i+1) < size && inp[i] > inp[i+1]){
		tmp  = inp[i];
		inp[i] = inp[i+1];
		inp[i+1] = tmp;
	}
}