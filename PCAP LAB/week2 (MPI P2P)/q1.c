#include "mpi.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define mc MPI_COMM_WORLD

int main(int argc, char *argv[]) {
  int rank,size,x; 

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(mc,&rank);
  MPI_Comm_size(mc,&size);

  MPI_Status status; 

  char str[] = "Hello";
  char *str2 = (char *)malloc(sizeof(char)*12);
  int len = strlen(str);  

  if(rank==0) {
    MPI_Ssend(str,len,MPI_CHAR,1,1,mc);
    printf("Sent string from master\n");
    MPI_Recv(str,len,MPI_CHAR,1,2,mc,&status);
    printf("Received string %s in master\n",str);
  }
  else {
    MPI_Recv(str2,len,MPI_CHAR,0,1,mc,&status);
    printf("Received string in slave: %s\n",str2);
    for(int i=0; i<len; i++) {
      if(isupper(str2[i]))
        str2[i] = tolower(str2[i]);
      else if(islower(str2[i]))
        str2[i] = toupper(str2[i]);
    }
    MPI_Ssend(str2,len,MPI_CHAR,0,2,mc);
    printf("Sent string from slave\n");
  }
  
  MPI_Finalize();
  return 0;
}
