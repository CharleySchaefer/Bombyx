/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#ifndef STICKY_REPTATION_H 
  #define STICKY_REPTATION_H 1

  //     The double-reptation model is numerically expensive for large data sets     
  //     Fortunately, there are only two parameters: 
  //       - the time in units of the reptation time
  //       - the des Cloizeaux H parameter: For small values fluctuations render
  //         the terminal relaxation time smaller than the reptation time
  //     Hence, in practice we initialise by evaluating the double-reptation model
  //     at a range of time and H values. For large datasets, the double-reptation
  //     moduli are obtained by inter- and extrapolation.

  typedef struct
  {
  // PHYSICAL PARAMETERS
    int    N;          // number of monomers per chain
    double v;          // volume of a monomer in [m3]
    double phi;        // volume fraction of the polymer (in solution)
    double T;          // temperature in [K]

    double Ze, Zs;     // Number of entanglements and stickers per chain
    double tau0, tauS; // Rouse time per segment and sticker dissociation time
    double alpha;      // Materials parameter
    double Ge;         // Plateau modulus ( equals 0.8*phi*Ze*kB*T/(v*N) )
    double Ge_fac;   


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


  } SREP;


  SREP *make_SREP(void);
  int free_SREP(SREP *);

  int estimate_StickyReptation_parameters_from_data(SREP *, double *, double *, double *, int, double, double, double, double);
  int G_StickyReptation_at_alpha_tauS_Zs_Ze(SREP *, double *, int, double, double, int, double **, double **, double **);
  int G_StickyReptation_at_fixed_alpha(SREP *,  double, double, double, int, double *, double *, double *, double, double *, double *);
  int G_StickyReptation_interp2(SREP *, double, double,  double *, int, double, double, int, double **, double **, double **, double,  double *, double *);

  #include "../ModuliLIB.h"

#endif


