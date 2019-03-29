/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer

    demo_StickyReptation.c

    This script demonstrates how the sticky-reptation model as developed by des Cloizeaux is implemented in a C program, see
    J. des Cloizeaux, Macromolecules 23, 4678--4687 (1990)

    To enhance numerical performance (especially for curve-fitting algorithms), 
    an initialisation step is done:

    The model model uses two infinite sums g(x) and F(x) (see des Cloizeaux, 1990). 
    At initialisation, these functions are calculated for various values of x,
    while linear interpolation is done at other parts of the program.
    Extrapolation is done using the analytical limits x-->0 and x-->inf.
    The numerical implementation is validated using the demo "compile_descloizeaux.sh"

    
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../ModuliLIB.h"

int main(int argc, char *argv[])
{
  int i,j;

  SREP *Set=(SREP *)malloc(sizeof(SREP));

  // Double-reptation interpolation 
  double *time_interp,  *omega_interp;          // Time and frequency
  double *Grelax_interp,  *Gelas_interp, *Gvisc_interp; // Moduli

  double gelas, gvisc;
  double gelas_sr, gvisc_sr, gelas_dr, gvisc_dr;

  FILE *ifp;


  //----------------------------------
  // PHYSICAL PROPERTIES
  //    physical contant 
  double kB=1.3806503e-23; // Boltzmann constant [J/K] 

  //    set values
  Set->N=5500;     // Degree of polymerisation
  Set->v=0.13e-27; // Average volume of a monomer in [m3]
  Set->T=300;      // Temperature in [K]
  Set->phi=0.2;    // Volume fraction of the polymer

  Set->tau0=1e-9;  // Rouse relaxation time of a segment
  Set->tauS=0.05;  // Sticker dissociation time
  Set->Zs=10;      // Number of stickers
  Set->Ze=15;      // Number of entanglements
  Set->alpha=10.0; // Materials parameter (v Ruymbeke)


  //    derived values 
  double kT=kB*Set->T;    // Thermal energy in [J]
  Set->Ge=PlateauModulus(kT, Set->N, Set->v, Set->phi, Set->Ze);    // Modulus [Pa]


  double tau_Rs  = Set->tau0*pow(Set->N/Set->Zs, 2); // Rouse time of strand between stickers
  double tau_Re  = Set->tau0*pow(Set->N/Set->Ze, 2); // Rouse time of strand between entanglements
  double tau_rep = (Set->tauS>tau_Rs ? Set->tauS*Set->Zs*Set->Zs*Set->Ze : Set->tau0*Set->Ze*Set->N*Set->N); // Reptation time
  double H=Set->Ze/Set->alpha; // alpha: v Ruymbeke materials parameter; 
                     // H:     des Cloizeaux parameter
  double tau_term = tau_rep/(1.0+Set->alpha); // Approximation of terminal relaxation time:
                                         // Shorter than the reptation time due to unknown relaxation mechanisms (such as contour-length fluctuations) captured by alpha!
  //----------------------------------


  //----------------------------------
  // NUMERICAL PARAMETERS
  //   Set values
  //     frequency interval
  int    Nw=1000;
  double wL=0.05*(1.0/tau_rep), wU=10.0*(1.0/Set->tau0),  omega;


  //     Define double-repation interpolation function
  //     The double-reptation model is numerically expensive for large data sets     
  //     Fortunately, there are only two parameters: 
  //       - the time in units of the reptation time
  //       - the des Cloizeaux H parameter: For small values fluctuations render
  //         the terminal relaxation time smaller than the reptation time
  //     Hence, in practice we initialise by evaluating the double-reptation model
  //     at a range of time and H values. For large datasets, the double-reptation
  //     moduli are obtained by inter- and extrapolation.
  Set->NH=10; // Number of samples of the descloizeaux H parameter to interpolate
                // Interpolation domain of H is set automatically
  Set->Nnum_rep=100;   // Number of frequencies to interpolate double-reptation function
  Set->tnum_rep_L=1e-7; Set->tnum_rep_U=1e2; // Interpolation domain of frequency

  //     double-reptation embedded g() and F() functions
  Set->tol=1e-4;       // accuracy tolerance
  Set->Ng=100;Set->NF=100; // numbers of samples


  //  Derived values
  Set->H_L=1.0/Set->alpha; 
  Set->H_U=(double)Set->N/Set->alpha; // Range of H parameter
  Set->Hrow;
  Set->trep_L= Set->tnum_rep_L*1e-2/(1.0+Set->alpha);
  Set->trep_U= Set->tnum_rep_U*1e1/( 1.0+Set->alpha);
  //double **omega_mat, **Gelas_mat, **Gvisc_mat;
  //----------------------------------


/*
    // NUMERICAL PARAMETERS
    int    Npar;       // number of free parameters
    int    Ndata;      // number of data points
    double *w1, *Gelas,*w2, *Gvisc; // data points

    // INTERPOLATION PARAMETERS
    //   Set values
    //     Define double-repation interpolation function
    //     The double-reptation model is numerically expensive for large data sets     
    //     Fortunately, there are only two parameters: 
    //       - the time in units of the reptation time
    //       - the des Cloizeaux H parameter: For small values fluctuations render
    //         the terminal relaxation time smaller than the reptation time
    //     Hence, in practice we initialise by evaluating the double-reptation model
    //     at a range of time and H values. For large datasets, the double-reptation
    //     moduli are obtained by inter- and extrapolation.
    int    NH; // Number of samples of the descloizeaux H parameter to interpolate
                  // Interpolation domain of H is set automatically
    int    Nnum_rep;   // Number of frequencies to interpolate double-reptation function
    double tnum_rep_L, tnum_rep_U; // Interpolation domain of frequency

    //     double-reptation embedded g() and F() functions
    double tol;       // accuracy tolerance
    int    Ng, NF; // numbers of samples

    //  Derived values
    double H, H_L, H_U; // Range of H parameter
    double *Hrow;
    double trep_L, trep_U;
    double **omega_mat, **Gelas_mat, **Gvisc_mat;
*/


  //----------------------------------
  // INITIALISE
  Set->Hrow=(double*)malloc(Set->NH*sizeof(double));
  Set->omega_mat=(double**)malloc(Set->NH*sizeof(double*));
  Set->Gvisc_mat=(double**)malloc(Set->NH*sizeof(double*));
  Set->Gelas_mat=(double**)malloc(Set->NH*sizeof(double*));
  for(i=0; i<Set->NH; i++)
  {
    Set->Hrow[i]=Set->H_L*pow(Set->H_U/Set->H_L, (double)i/(Set->NH-1)); // sample H parameters in log space
    // Solve double-reptation model for each H value
    G_doublereptation_initialise(Set->Ng, Set->NF, Set->tol, Set->Hrow[i], Set->trep_L, Set->trep_U, Set->Nnum_rep, &(Set->omega_mat)[i], &(Set->Gelas_mat)[i], &(Set->Gvisc_mat)[i]);
  }
  //----------------------------------
  
  for(i=0; i<Nw; i++)
  {
    omega=wL*pow(  wU/wL, (double)i/(Nw-1)  );
    for(j=0; j<Set->NH; j++) // Sweep H parameter
    {

       H=Set->Hrow[j];
       G_StickyReptation_at_fixed_alpha(Set,  tau_rep, 1.0/Set->trep_U, 1.0/Set->trep_L, Set->Nnum_rep, Set->omega_mat[j], Set->Gelas_mat[j], Set->Gvisc_mat[j], omega, &gelas, &gvisc);

       printf("%15e %15e %15e ", omega, gelas, gvisc);
    }
    putchar('\n');
  }

  return(1);
}


