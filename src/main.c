/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
#include "main.h"

int main(int argc, char *argv[])
{ 
  //======================================================================
  // PROGRAM DECLARATIONS
  int i,j;
  int verbose,doMC,doErrorAnalysis, Nchi; // default values
  double tol; // Error analysis
  INPUT  *Set;   // Experimental data           (src/Import)
  SREP   *Model; // Sticky-reptation parameters (lib/ModuliLIB)
  MCSET  *MCSet; // Monte Carlo settings        (src/DataAnalysis)
  char *inputfile;

  double tauSbest, Zebest, Zsbest, Gefacbest, alphaBest, resnormBest,resnorm0;
  FILE *ifp;
  // PROGRAM DECLARATIONS DONE
  //======================================================================

  //======================================================================
  // READ INPUT 
  verbose=0;
  Nchi=10;  
  tol=0.1;

  inputfile=argv[1];
  if(ParseProgramArguments(argc, argv, &MCSet, &Model, &verbose)==-1)
  {
    printf("Error: ParseProgramArguments() failed!\n"); exit(1);
  }

  if(!readinput(inputfile, &Set, (verbose=0)))
  {printf("Error: readinput() failed!\n"); exit(1);}
  export_bbx_data(Set);

  // NUMERICAL PARAMETERS
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
  Model->NH=10; // Number of samples of the descloizeaux H parameter to interpolate
                // Interpolation domain of H is set automatically
  Model->Nnum_rep=100;   // Number of frequencies to interpolate double-reptation function
  Model->tnum_rep_L=1e-7; Model->tnum_rep_U=1e2; // Interpolation domain of frequency

  //     double-reptation embedded g() and F() functions
  Model->tol=1e-4;       // accuracy tolerance
  Model->Ng=100; Model->NF=100; // numbers of samples


  // MONTE CARLO SETTINGS
  if(MCSet->tauS_L==-1)        // minimum sticker dissociation time
    MCSet->tauS_L=0.2/d1max(Set->w1, Set->Ndata); // minimum sticker dissociation time
  if(MCSet->tauS_U==-1)        // maximum sticker dissociation time
    MCSet->tauS_U=1.0/d1min(Set->w1, Set->Ndata); 

  if(MCSet->Zs_L==-1)     // minimum number of stickers
    MCSet->Zs_L  =1;      // default 
  if(MCSet->Zs_U==-1)     // maximum number of stickers
    MCSet->Zs_U  =Set->N; // default 

  if(MCSet->Ze_L==-1)     // minimum number of entanglements
    MCSet->Ze_L=1;        // Default
  if(MCSet->Ze_U==-1)     // maximum number of entanglements
    MCSet->Ze_U=Set->N;   // Default

  // INPUT READ
  //======================================================================

  //======================================================================
  // INITIALISE MODELLING STRUCTURE

  Model->trep_L= Model->tnum_rep_L*1e-2/(1.0+Model->alpha);
  Model->trep_U= Model->tnum_rep_U*1e1/(1.0+Model->alpha);
  Model->H_L=0.1*1.0/Model->alpha, Model->H_U=(double)Set->N/Model->alpha; // Range of H parameter

  Model->Hrow=(double*)malloc(Model->NH*sizeof(double));
  Model->omega_mat=(double**)malloc(Model->NH*sizeof(double*));
  Model->Gvisc_mat=(double**)malloc(Model->NH*sizeof(double*));
  Model->Gelas_mat=(double**)malloc(Model->NH*sizeof(double*));
  for(i=0; i<Model->NH; i++)
  {
    Model->Hrow[i]=Model->H_L*pow(Model->H_U/Model->H_L, (double)i/(Model->NH-1)); // sample H parameters in log space
    // Solve double-reptation model for each H value
    G_doublereptation_initialise(Model->Ng, Model->NF, Model->tol, Model->Hrow[i], Model->trep_L, Model->trep_U, Model->Nnum_rep, &(Model->omega_mat[i]), &(Model->Gelas_mat[i]), &(Model->Gvisc_mat[i]));
  }


  Model->Ndata  = Set->Ndata;
  Model->N      = Set->N;
  Model->v      = Set->v;
  Model->phi    = Set->phi;
  Model->T      = Set->T;

  Model->w1    =(double*)malloc(Set->Ndata*sizeof(double));
  Model->w2    =(double*)malloc(Set->Ndata*sizeof(double));
  for(i=0; i<Set->Ndata; i++)
    {Model->w1[i]=Set->w1[i];Model->w2[i]=Set->w2[i];}

  Model->Gvisc =(double*)calloc(Set->Ndata,sizeof(double));
  Model->Gelas =(double*)calloc(Set->Ndata,sizeof(double));

  Model->alpha;
  Model->tau0 =1.0/(Model->w1[Set->Ndata-1]*Model->N*Model->N)*1e-6; // Early-stage Rouse-relaxation does not affect the sticker plateau.


  if(MCSet->rseed==-1)
    srand(time(NULL));
  else
    srand(MCSet->rseed);
  //======================================================================



  //==============================
  // Optimise parameters  
  ifp=fopen(BBX_MONTECARLO_OUTPUT_FILE, "w");
  fclose(ifp);
printf("#MC_perform_iterations()\n");
  if(!MC_perform_iterations(Set, Model, MCSet,   &tauSbest, &Zebest, &Zsbest, &Gefacbest, &alphaBest, &resnormBest, BBX_MONTECARLO_OUTPUT_FILE, verbose))
  {printf("Error: MC_perform_iterations() failed.\n"); exit(1);}
printf("#done\n");
  // Parameters optimised
  //==============================
  
  
  //==============================
  // Report best fit
  Model->tauS=tauSbest; Model->Ze=Zebest; Model->Zs=Zsbest; Model->Ge_fac=Gefacbest; Model->alpha=alphaBest;
  Model->Ge=Gefacbest*PlateauModulus(K_BOLTZMANN*Model->T, Model->N, Model->v, Model->phi, Model->Ze);

  if(verbose)
  {
    printf("#Best parameters:\n");
    printf("#resnorm    tauS    Ze    Zs    Ge    Ge_fac  alpha\n");
    printf("#%f %f %f %f %f %f %f\n", resnormBest, Model->tauS, Model->Ze, Model->Zs, Model->Ge, Gefacbest, Model->alpha);
  }

  if(!G_StickyReptation_at_alpha_tauS_Zs_Ze(Model, Model->Hrow, Model->NH, 1.0/Model->tnum_rep_U, 1.0/Model->tnum_rep_L, Model->Nnum_rep, Model->omega_mat, Model->Gelas_mat, Model->Gvisc_mat))
  {printf("Error: G_StickyReptation_at_alpha_tauS_Zs_Ze() failed!\n"); exit(1);}

  ifp=fopen("bbx_model.out", "w");
  fprintf(ifp, "%15s %15s %15s\n", "#omega", "Gvisc", "Gelas"); 
  for(i=0; i<Model->Ndata; i++)
    fprintf(ifp, "%15e %15e %15e\n", Model->w1[i], Model->Gvisc[i], Model->Gelas[i]); 
  fclose(ifp);
  //==============================


  // Filter Monte Carlo samples
  if(!analyse_bbx_convergence_file(BBX_MONTECARLO_OUTPUT_FILE, MCSet->TSA, tol, Nchi, verbose))
    {printf("Error: analyse_bbx_convergence_file() failed!\n"); exit(1);}

  // Free memory
  free_bbx_input(Set);
  free_MC_SET(MCSet);
  free_SREP(Model);
  
  return(0);
}
