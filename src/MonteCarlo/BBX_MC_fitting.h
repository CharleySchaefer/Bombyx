/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
/*
    Monte Carlo curve-fitting algorithm:

    1. Specify parameter boundaries [Pi_L, Pi_U]
    2. Get initial parameter values and calculate chi square.
    3. Vary parameter values within a factor "delta":
       Pi_new = Pi_old +- rand(), with:

         max( Pi_old*(1-delta), Pi_L ) <= rand() <= min( Pi_old*(1+delta), Pi_U )

    4. Calculate new chi square
    5. If Chi2_new <= Chi2_old, accept new parameter set
       If Chi2_new > Chi2_old, accept new parameter set with probability
     
         exp(  -(Chi2_new - Chi2_old) / 2 sigma^2  )
    6. Repeat [3-5] Niter times

    Summarised, the parameters that control this algorithm are Niter, delta, sigma.
    
*/
#ifndef BBX_MC_FITTING_H
#define BBX_MC_FITTING_H

  #include <stdlib.h>

  typedef struct
  {
    int      Nseeds;     // number of initial parameter sets
    int      Niter;      // Number of Monte Carlo steps
    double   delta;      // Step size by which parameters are varied
    double   TSA;        // TSA=2*sigma_a^2 controls the acceptance probability exp(-deltaX2/TSA)
    int      rseed;      // Random number seed: -1: time stamp; >0: user-defined integer seed
    double   tauS_L, tauS_U; // Sticker dissociation time
    double   Zs_L,   Zs_U;   // Number of stickers per chain
    double   Ze_L,   Ze_U;   // Number of entanglements per chain
    double   Ge_L,   Ge_U;   // Elastic modulus
    double   alpha_L,   alpha_U;   // Materials parameter
    double   Ge_fac;   
  } MCSET; // Monte Carlo Settings

  MCSET *make_MC_SET(void);
  int    free_MC_SET(MCSET *);

  #include "../Import/ParseProgramArguments.h"
  #include "../Import/ReadBBXdata.h"
  #include "../../lib/ModuliLIB/ModuliLIB.h"
  #include "../../lib/ZiltoidLIB/ZiltoidLIB.h"

  // Chi square
  int calculateResnorm(INPUT *, SREP *, double *);

  // Create proposed Monte Carlo Step
  double MC_propose_new_parameter(double , double , double , double );

  // Iterate Monte Carlo steps
  int    MC_perform_iterations(INPUT *, SREP *, MCSET *, double *, double *, double *, double *, double *, double *, char *, int );
#endif
