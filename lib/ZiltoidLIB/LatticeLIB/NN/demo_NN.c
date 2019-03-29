#include <stdio.h>
#include <stdlib.h>
#include "NNlib.h"


int main(int argc, char *argv[])
{
  // Define nearest-neighbour matrix
  NN_STRUCT *NN=new_nn_struct(12, 2);

  NN->dx[ 0]=-1; NN->dy[ 0]=-1;
  NN->dx[ 1]=-1; NN->dy[ 1]= 0;
  NN->dx[ 2]=-1; NN->dy[ 2]= 1;
  NN->dx[ 3]= 0; NN->dy[ 3]=-1;
  NN->dx[ 4]= 0; NN->dy[ 4]= 0;
  NN->dx[ 5]= 0; NN->dy[ 5]= 1;
  NN->dx[ 6]= 1; NN->dy[ 6]=-1;
  NN->dx[ 7]= 1; NN->dy[ 7]= 0;
  NN->dx[ 8]= 1; NN->dy[ 8]= 1;
  NN->dx[ 9]= 2; NN->dy[ 9]=-1;
  NN->dx[10]= 2; NN->dy[10]= 0;
  NN->dx[11]= 2; NN->dy[11]= 1;


  // Analyse nearest neighbourhood (size, min and max dimensions)
  nn_analyse(NN);
  int *ibuff=(int*)malloc(NN->Nx*NN->Ny*sizeof(int)); // used for printing 


  printf("Original matrix in dense form:\n");
  nn_print(NN, ibuff);

  // Do rotations  
  nn_rotatez_right(NN);
  putchar('\n');
  printf("Rotate right\n");
  nn_print(NN, ibuff);

  nn_rotatez_right(NN);
  putchar('\n'); 
  printf("Rotate right\n");
  nn_print(NN, ibuff);

  nn_rotatez_right(NN);
  putchar('\n'); 
  printf("Rotate right\n");
  nn_print(NN, ibuff);

  nn_rotatez_right(NN);
  putchar('\n'); 
  printf("Rotate right\n");
  nn_print(NN, ibuff);

  nn_rotatez_left(NN);
  putchar('\n'); 
  printf("Rotate left\n");
  nn_print(NN, ibuff);

  nn_rotatez_180(NN);
  putchar('\n');
  printf("Rotate 180\n");
  nn_print(NN, ibuff);

  nn_reflect_x(NN);
  putchar('\n');
  printf("reflect x\n");
  nn_print(NN, ibuff);

  nn_reflect_y(NN);
  putchar('\n');
  printf("reflect y\n");
  nn_print(NN, ibuff);

  printf("\nFree memory.\n");
  free_nn_struct(NN);
  free(ibuff);
  printf("Done.\n");

  return(0);
}
