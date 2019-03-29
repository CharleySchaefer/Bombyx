#ifndef LINEAR_ALGEBRA_H
  #define LINEAR_ALGEBRA_H 1
  #include <stdio.h>
  #include <math.h>
  #include "DoolittleAlgorithm/DoolittleAlgorithm.h"
  #include "lapack/eigenproblem.h"
  int is_matrix_symmetric(double **, int, int);
  int matrix_vector_product(double **, int, int, double *, double *);
  int vector_inner_product(double *, double *, int,  double *);

  int gram_schmidt(double *, int, int, double *, double *, double *);
  int eigenvalues_QRalgorithm(double**, int, int, int, double, double*,double*, double*, double*, double *);
  void gauss_jordan(float **, int, int );
#endif

