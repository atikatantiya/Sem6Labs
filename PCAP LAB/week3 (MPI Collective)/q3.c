#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define mc MPI_COMM_WORLD

int find_vowels(char *s,int n) {
  int ans = 0;
  for(int i=0; i<n; i++) {
    if(s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'A' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U')
      ans++;
  }
  return ans;
}

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  //N is divisor of string length

  int n = size;
  char s[200];
  char new_string[200];
  int m,slice;
  int x[2],ans[400];
  int vowels = 0,cons = 0;

  if(rank == 0) {
    printf("Enter string with a length multiple of %d\n",n);
    gets(s);
    m = strlen(s);
    if(m%n != 0) {
      printf("Size not divisible!!\n");
      exit(0);
    }
    slice = m/n;
  }

  MPI_Bcast(&slice,1,MPI_INT,0,mc);

  MPI_Scatter(s,slice,MPI_CHAR,new_string,slice,MPI_CHAR,0,mc);
  x[0] = find_vowels(new_string,slice);
  x[1] = slice - x[0];
  printf("Process %d with vowels %d & consonants %d\n",rank,x[0],x[1]);

  MPI_Gather(x,2,MPI_INT,ans,2,MPI_INT,0,mc);

  if(rank==0) {
    for(int i=0; i<n*2; i++) {
      if(i%2 == 0)
        vowels = vowels + ans[i];
      else
        cons = cons + ans[i];
    }
    printf("Value gathered by root is: %d vowels and %d consonants\n",vowels,cons);
  }
  
  MPI_Finalize();
  return 0;
}