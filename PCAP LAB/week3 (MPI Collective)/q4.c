#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  //N is divisor of string length

  int n = size;
  char s[200],s2[200];
  char a[200],b[200];
  char ans[400];
  int m,slice;

  if(rank == 0) {
    printf("Enter 2 strings of length multiple of %d: \n",n);
    gets(s);
    gets(s2);
    if(strlen(s) != strlen(s2)) {
      printf("Strings should be of same size!!\n");
      exit(0);
    }
    m = strlen(s);
    if(m%n != 0) {
      printf("String length should be divisible by n!!\n");
      exit(0);
    }
    slice = m/n;
  }

  MPI_Bcast(&slice,1,MPI_INT,0,mc);

  MPI_Scatter(s,slice,MPI_CHAR,a,slice,MPI_CHAR,0,mc);
  MPI_Scatter(s2,slice,MPI_CHAR,b,slice,MPI_CHAR,0,mc);

  char new_string[400];
  int j = 0;
  for(int i=0; i<slice; i++) {
    new_string[j++] = a[i];
    new_string[j++] = b[i];
  }

  printf("String in process %d is %s\n",rank,new_string);
  

  MPI_Gather(new_string,slice*2,MPI_CHAR,ans,slice*2,MPI_CHAR,0,mc);

  if(rank==0) {
    ans[m*2] = '\0';
    printf("Final string is %s\n",ans);
  }
  
  MPI_Finalize();
  return 0;
}