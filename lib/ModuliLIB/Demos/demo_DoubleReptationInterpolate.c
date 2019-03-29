/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../ModuliLIB.h"




int main(int argc, char *argv[])
{
  int i,j;

  // Double-reptation interpolation 
  double *time_interp,  *omega_interp;          // Time and frequency
  double *Grelax_interp,  *Gelas_interp, *Gvisc_interp; // Moduli

  double gelas, gvisc;
  double gelas_exact, gvisc_exact;

  FILE *ifp;


  //----------------------------------
  // PHYSICAL PROPERTIES
  //    set values
  int N=5000;        // Degree of polymerisation
  double tau0=1.0;   // Rouse relaxation time of a segment
  double tauS=100.0; // Sticker dissociation time
  double Zs=10;      // Number of stickers
  double Ze=10;      // Number of entanglements
  double alpha=10.0;  // Materials parameter (v Ruymbeke)

  //    derived values
  double tau_Rs  = tau0*pow(N/Zs, 2); // Rouse time of strand between stickers
  double tau_Re  = tau0*pow(N/Ze, 2); // Rouse time of strand between entanglements
  double tau_rep = (tauS>tau_Rs ? tauS*Zs*Zs*Ze : tau0*Ze*N*N); // Reptation time
  double H=alpha/Ze; // alpha: v Ruymbeke materials parameter; 
                     // H: des Cloizeaux parameter
  double tau_term = tau_rep/(1.0+alpha); // Approximation of terminal relaxation time:
                                         // Shorter than the reptation time due to unknown relaxation mechanisms (such as contour-length fluctuations) captured by alpha!
  //----------------------------------

  //----------------------------------
  // NUMERICAL PARAMETERS
  //    set values
  double tol=1e-4;
  int    Ng=100, NF=100; // Number of interpolation points for the descloizeaux g() and F() functions
  int    Nnum_rep=200;   // Number of points to get double-reptation interpolation function
  double tnum_rep_L=1e-3, tnum_rep_U=1e2; // Time range of accuracy of double-reptation around terminal relaxation time 


  //    derived values
  double trep_L= tnum_rep_L*1e-2/(1.0+alpha), trep_U=tnum_rep_U*1e1/(1.0+alpha); 

  //----------------------------------


  G_doublereptation_initialise(Ng, NF, tol, H, trep_L, trep_U, Nnum_rep, &omega_interp, &Gelas_interp, &Gvisc_interp);
  for(i=0; i<Nnum_rep; i++)
  {
    G_doublereptation_interpolation(1.0/tnum_rep_U, 1.0/tnum_rep_L, Nnum_rep, omega_interp, Gelas_interp,Gvisc_interp, omega_interp[i], &gelas, &gvisc);

    printf("%15e %15e %15e %15e %15e\n", omega_interp[i], Gelas_interp[i], Gvisc_interp[i], gelas, gvisc);
  }

  return(1);
}


