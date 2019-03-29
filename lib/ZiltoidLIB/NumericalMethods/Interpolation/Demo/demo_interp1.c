#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
  int i, Nx=10;
  double *x, *y, xint, yint;


  printf("Demo interp1\n");


  x=(double*)malloc(Nx*sizeof(double));
  y=(double*)malloc(Nx*sizeof(double));

  for(i=0; i<Nx; i++)
  {
    x[i]=((double)2*i/((double)(Nx-1))); // Range [0 : 2]
    y[i]=sqrt(x[i]);
    printf("%15e %15e\n", x[i], y[i]);
  }
  putchar('\n');
  xint=0.5;
  interp1(x,y,Nx, xint, &yint);
  printf("%15e %15e\n", xint, yint);
  

  return(0);
}
