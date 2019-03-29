#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../ZiltoidLIB.h"

// compile using "gcc demo_stretch_matrix.c -L../../.. -lZiltoidLIB"

int main(int argc, char *argv[])
{
  int i,j, Nx_in=3, Ny_in=4, Nx_out=6, Ny_out=6;
  double **mat_in, **mat_out;

  printf("Demo stretch_matrix()\n");

  mat_in =(double**)malloc(Nx_in*sizeof(double*));
  for(i=0; i<Nx_in; i++)
    mat_in[i] =(double*)malloc(Ny_in*sizeof(double));
  
  mat_out=(double**)malloc(Nx_out*sizeof(double*));
  for(i=0; i<Nx_out; i++)
    mat_out[i] =(double*)malloc(Ny_out*sizeof(double));

  mat_in[0][0] = 0.2; mat_in[0][1] = 0.3; mat_in[0][2] = 0.4; mat_in[0][3] = 0.5;
  mat_in[1][0] = 0.3; mat_in[1][1] = 0.4; mat_in[1][2] = 0.5; mat_in[1][3] = 0.6;
  mat_in[2][0] = 0.4; mat_in[2][1] = 0.5; mat_in[2][2] = 0.6; mat_in[2][3] = 0.7;

  printf("\nMatrix:\n");
  for(i=0; i<Nx_in; i++)
  {
    for(j=0; j<Ny_in; j++)
      printf("%3.1f ", mat_in[i][j]);
    putchar('\n');
  }

  printf("\nNon-periodic stretching:\n");
  stretch_matrix(0, mat_in, Nx_in, Ny_in, mat_out, Nx_out, Ny_out);
  for(i=0; i<Nx_out; i++)
  {
    for(j=0; j<Ny_out; j++)
      printf("%4.2f ", mat_out[i][j]);
    putchar('\n');
  }
  printf("\nPeriodic stretching:\n");
  stretch_matrix(1, mat_in, Nx_in, Ny_in, mat_out, Nx_out, Ny_out);
  for(i=0; i<Nx_out; i++)
  {
    for(j=0; j<Ny_out; j++)
      printf("%4.2f ", mat_out[i][j]);
    putchar('\n');
  }

  // free memory
  for(i=0; i<Nx_in; i++)
    free(mat_in[i]);
  free(mat_in);
  for(i=0; i<Nx_out; i++)
    free(mat_out[i]);
  free(mat_out);
  

  return(0);
}
