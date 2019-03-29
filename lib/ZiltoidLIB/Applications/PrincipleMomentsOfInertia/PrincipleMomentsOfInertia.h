#ifndef PMI_PAI_H
  #define PMI_PAI_H
  #include "../../Mathematics/Mathematics.h"

  /*
    PURPOSE
    From positions and masses calculate the 
    principle moments of inertia (PMI) and the 
    principle axes of inertia (PAI)

    USAGE
    - Requires blas/lapack libraries.
    - Use in script: #include "PrincipleMomentsOfInertia.h"
    - compile:       gcc main.c PrincipleMomentsOfInertia.c -lZiltoidLIB -lapack -lm
  */
  int getPrincipalMomentsOfInertia(double *, double *, double *, double *, int, double *,double **,  double *, double *);
#endif
