#ifndef ZILTOID_MATHEMATICS_H
#define ZILTOID_MATHEMATICS_H 1

  #include <math.h>
  #include <stdlib.h>
  #include <stdio.h>

  #include "Polynomial/Polynomial.h"
  #include "Fourier/Fourier.h"
  #include "RandomNumbers/RandomNumbers.h"
  #include "LinearAlgebra/LinearAlgebra.h"

  // Scalar functions
  double absolute(double);
  int    isPowerOfTwo (unsigned int);
  int    bin2dec(int *, int *, int);
  int    dec2bin(int  , int *, int);

  double minimum(double, double);
  double maximum(double, double);
  int    minimum_int(int, int);
  int    maximum_int(int, int);

  // 1D array analysis
  // Find value in array
  int    i1find( int   , int    *, int);
  int    f1find( float , float  *, int);
  int    d1find( double, double *, int);

  // Find minimum in array
  int    i1min(        int *, int); // integers
  float  f1min(      float *, int); // floats
  double d1min(     double *, int); // double precision

  // Find maximum in array
  int    i1max(        int *, int); // integers
  float  f1max(      float *, int); // floats
  double d1max(     double *, int); // double precision

  // Mean of array
  double di1mean(      int *, int); 
  float  f1mean(     float *, int); // floats
  double d1mean(    double *, int); // double precision

  // Variance of array
  double di1variance(int *, int); // double precision
  float  f1variance( float *, int); // floats
  double d1variance(double *, int); // double precision



  // 2D array analysis
  void gradarray2D(double **, char, int, double, int, int, double *);



#endif
