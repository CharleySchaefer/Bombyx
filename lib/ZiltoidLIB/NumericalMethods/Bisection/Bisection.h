#ifndef BISECTION_H 
  #define BISECTION_H 1

  #include <stdio.h>
  #include <stdlib.h>

  int  find_X_at_Yvalue(double*, double*, int, double, double *);

  int  find_index_above_y0_float(    float*, int,   float, int *);
  int  find_index_above_y0_double(  double*, int,   double, int *);
  int  find_index_closest_to_Yvalue(double*, int,  double, int *);

#endif
