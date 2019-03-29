#include "../NumericalMethods.h"
#include <stdlib.h>


main(int argc, char *argv[])
{
  int i;
  int ind;
  int Ndata=5;
  double *x, *y;
  double x0,y0;

  x=(double*)malloc(Ndata*sizeof(double));
  y=(double*)malloc(Ndata*sizeof(double));

  for(i=0; i<Ndata; i++)
  {
    x[i]=(double)i/(Ndata-1);
    y[i]=-0.5+x[i]*x[i];
    printf("%e %e\n", x[i], y[i]); 
  }
  y0=0.0;
  find_X_at_Yvalue(x, y, Ndata, y0, &x0);  
  printf("x0=%e\n", x0);

  find_index_closest_to_Yvalue(y, Ndata, y0, &ind );

  printf("x_closest=%e\n", x[ind]);


  return 0;
}
