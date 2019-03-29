/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#ifndef DOUBLE_REPTATION_H
  #define DOUBLE_REPTATION_H 1
  #include <math.h>
  #include <stdlib.h>
  #include <stdio.h>



  #include "../ModuliLIB.h"

  double PlateauModulus(double, int, double, double, double);

  double G_doublereptation(double *, double *, int, double *, double *, int, double, double);
  int    G_doublereptation_interpolation(double, double, int, double *, double *, double *, double, double *, double *);
  int    G_doublereptation_initialise(int, int, double, double, double, double, int, double **, double **, double **);

/*
  J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
  limiting behavior:
  g(x)=sqrt(pi*x) for x<<1
  g(x)=(pi^2/6)         for x>>1 ( in this limit g=\sum_{m=1}^{\infty}m^{-2} )
*/
  double     g_descloizeaux(double, double);
  double get_g_descloizeaux_convergence_tolerance(double);
  int    get_g_descloizeaux_boundaries(double ,double *,double *);
  double     g_descloizeaux_interpolation(double *, double *, int, double);
  int        g_descloizeaux_initialise(double **, double **, int, double);

/*
  F(U)=1 for U<<1 
  F(U)=(8/pi^2)*exp(-U) for U>>1
*/
  double     F_descloizeaux(double, double);
  double get_F_descloizeaux_convergence_tolerance(double);
  int    get_F_descloizeaux_boundaries(double ,double *,double *);
  double     F_descloizeaux_interpolation(double *, double *, int, double);
  int        F_descloizeaux_initialise(double **, double **, int, double);
#endif
