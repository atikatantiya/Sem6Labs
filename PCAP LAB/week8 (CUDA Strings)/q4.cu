#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void reverse (char* str, char* res, int N, int len) {
    int numWords = threadIdx.x+1;
    int start = 0, end = 0;
    for(int i=0; i<len; ++i){
        if(str[i] == ' '){
            numWords--;
            if(numWords == 1) 
                start = i;
            else if(numWords <= 0){
                end = i;
                break;
            }
        }
    }
    for(int i=start; i<end;++i) 
        res[len-i-1] = str[i];
}

int main(){
    char string[100], res[100];
    int N, len;
    printf("Enter the string: ");
    fgets(string, 100, stdin);
    printf("Enter the number of words: ");
    scanf("%d", &N);
    
    char *d_str, *d_res;
    len = strlen(string);
    string[len-1] = ' ';

    cudaMalloc((void**)&d_str, sizeof(char)*(len+1));
    cudaMalloc((void**)&d_res, sizeof(char)*(len+1));

    cudaMemcpy(d_str, string, len, cudaMemcpyHostToDevice);

    reverse<<<1, N>>>(d_str, d_res, N, len);
    
    cudaMemcpy(res, d_res, len, cudaMemcpyDeviceToHost);

    res[len] = 0;

    printf("The result is: %s\n", res);

    for(int i=0; i<len; ++i) 
        printf("%c", res[i]);
    return 0;
}
