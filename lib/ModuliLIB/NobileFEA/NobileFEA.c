/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
/*
  Finite Element Approximation to obtain the elastic modulus G'(w) and loss modulus G''(w) from the relaxation modulus G(t).

  Method as described by Nobile and Coccini in Rheol. Acta 40, 111-119 (2001)
*/
#include "NobileFEA.h"

double h1func(double p, double q, double w)
  {return  ( cos(w*q) + ( sin(w*q)-sin(w*p) )/( (p-q)*w ));}
double h2func(double p, double q, double w)
  {return  (-sin(w*q) + ( cos(w*q)-cos(w*p) )/( (p-q)*w ));}


double H1func(double *t, double w, int N, int i)
{
  if(i==0)
    return (h1func(t[1], t[0], w));
  else if(i==N-1)
    return (-h1func(t[N-2], t[N-1], w));
  else
    return (h1func(t[i+1], t[i], w) - h1func(t[i-1], t[i], w));
}
double H2func(double *t, double w, int N, int i)
{
  if(i==0)
    return (h2func(t[1], t[0], w));
  else if(i==N-1)
    return (-h2func(t[N-2], t[N-1], w));
  else
    return (h2func(t[i+1], t[i], w) - h2func(t[i-1], t[i], w));
}

void NobileFEA(double *time, double *G, int Nt, double omega, double *Gelas, double *Gvisc)
{
  int j;
  *Gelas=0.0; // G'  - elastic/storage modulus
  *Gvisc=0.0; // G'' - viscous/loss modulus
  for(j=0; j<Nt; j++)
  {
    *Gelas       +=H1func(time, omega, Nt, j)*G[j];
    *Gvisc +=H2func(time, omega, Nt, j)*G[j];
  }
}

