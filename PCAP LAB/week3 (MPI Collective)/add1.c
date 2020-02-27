#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size); 

  int n = size;
  char s[200];
  char new_string[200], final_string[200];
  int m,slice;
  int arr3[200];
  int ans[200];

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

  for(int i=0; i<slice; i++) {    
    if(isupper(new_string[i]))
      new_string[i] = tolower(new_string[i]);
    else if(islower(new_string[i]))
      new_string[i] = toupper(new_string[i]);
    arr3[i] = new_string[i];
  }    

  MPI_Gather(arr3,slice,MPI_INT,ans,slice,MPI_INT,0,mc);
  MPI_Gather(new_string,slice,MPI_CHAR,final_string,slice,MPI_CHAR,0,mc);

  if(rank==0) {
    printf("The string is: %s\n",final_string);
    printf("The ascii values are: \n");
    for(int i=0; i<m; i++)
      printf("%d ",ans[i]);
    printf("\n");
  }
  
  MPI_Finalize();
  return 0;
}