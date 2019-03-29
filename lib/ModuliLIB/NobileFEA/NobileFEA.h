/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
/*
  Finite Element Approximation to obtain the elastic modulus G'(w) and loss modulus G''(w) from the relaxation modulus G(t).

  Method as described by Nobile and Coccini in Rheol. Acta 40, 111-119 (2001)
*/

#ifndef NOBILE_FEA_H
  #define NOBILE_FEA_H 1

  #include <math.h>

  double h1func(double, double, double);
  double h2func(double, double, double);
  double H1func(double *, double, int, int);
  double H2func(double *, double, int, int);



#endif
