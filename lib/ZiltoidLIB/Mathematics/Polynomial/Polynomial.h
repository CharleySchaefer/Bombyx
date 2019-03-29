// DOWNLOADED FROM http://van-der-waals.pc.uni-koeln.de/quartic/quintic_C.c at 2013-02-13
//  Solves p(x) = dd[0]*x^4 + dd[1]*x^3 + dd[2]*x^2 + dd[3]*x + dd[4]
// 
// The following prototype declarations need to be added to your code (ie. header file).
//

#ifndef POLYNOMIAL_H
  #define POLYNOMIAL_H  1


  #include <stdio.h>
  #include <math.h>

  int quintic(double [], double [], double [], int*, double);
  int quartic(double[], double[], double[], int* );
  int cubic(double[], double[], int*);
  int signR(double);
  double CBRT(double);

#endif








		
		

