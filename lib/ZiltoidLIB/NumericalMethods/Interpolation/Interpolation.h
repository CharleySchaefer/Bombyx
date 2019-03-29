#ifndef INTERPOLATION_H
  #define INTERPOLATION_H  1

  #include <stdio.h>
  #include <stdlib.h>


  int find_boundary_indices(double *, double *, int, double, int *, int *);
  int interp1(double *, double *, int, double, double *);
  int stretch_matrix(int, double **, int, int, double **, int, int);
#endif
