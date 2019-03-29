/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
#include "BBX_MC_fitting.h"

MCSET *make_MC_SET(void)
{
  MCSET *MCSet=(MCSET *)malloc(sizeof(MCSET));

  // default values
  MCSet->Nseeds=10;     // Number of initial parameter sets
  MCSet->Niter=1000;    // Number of Monte Carlo steps per parameter set
  MCSet->delta=0.1;     // Step size by which parameters are varied
  MCSet->TSA=0.05;      // TSA=2*sigma_a^2 controls the acceptance probability exp(-deltaX2/TSA)
  MCSet->rseed=1;       // Default random number seed

  // Parameter value boundaries
  MCSet->alpha_L=-1;
  MCSet->alpha_U=-1;
  MCSet->tauS_L=-1;
  MCSet->tauS_U=-1;
  MCSet->Zs_L=-1;
  MCSet->Zs_U=-1;
  MCSet->Ze_L=-1;
  MCSet->Ze_U=-1;
  MCSet->Ge_L=1;
  MCSet->Ge_U=1;
  MCSet->Ge_fac=1.0;

  return MCSet;
}




int free_MC_SET(MCSET *MCSet)
{
  if(MCSet!=NULL)
  {
    free(MCSet); MCSet=NULL;
  }

  return(1);
}

/*
  Chi square definition
*/
int calculateResnorm(INPUT *Set, SREP *Model, double *chisquare)
{
  int i;
  *chisquare=0.0;
  for(i=0; i<Model->Ndata; i++)
  {
    (*chisquare) += pow( (Model->Gelas[i]-Set->Gelas[i])/Set->Gelas[i], 2) + pow( (Model->Gvisc[i]-Set->Gvisc[i])/Set->Gvisc[i], 2);
  }
  (*chisquare)/=(Set->Ndata-Model->Npar);
  return(1);
}


/*
  Vary original parameter value "par0" by a random factor between 0 and "delta",
  but within a domain specified by "parL" and "parU". 
*/
double MC_propose_new_parameter(double par0, double delta, double parL, double parU)
{
  if(parL>parU-1e-7)
    return parL;

  par0   *= 1.0 + 2*delta*((double)rand()/RAND_MAX-0.5);
  while(par0<=parL)
    par0*=1.0   + delta*((double)rand()/RAND_MAX);
  while(par0>=parU)
    par0*=1.0   - delta*((double)rand()/RAND_MAX);
  return par0;
}

int MC_perform_iterations(INPUT *Set, SREP *Model, MCSET *MCSet, double *tauSbest, double *Zebest, double *Zsbest, double *Gefacbest, double *alphabest, double *resnormBest, char *fname, int verbose)
{
  
  int    Niter=MCSet->Niter;
  double delta=MCSet->delta; 
  double TSA=MCSet->TSA;
  int    Nseeds=MCSet->Nseeds;
  double alpha_L=MCSet->alpha_L;
  double alpha_U=MCSet->alpha_U;
  double tauS_L=MCSet->tauS_L;
  double tauS_U=MCSet->tauS_U;
  double Zs_L=MCSet->Zs_L;
  double Zs_U=MCSet->Zs_U;
  double Ze_L=MCSet->Ze_L;
  double Ze_U=MCSet->Ze_U;
  double Ge_L=MCSet->Ge_L;
  double Ge_U=MCSet->Ge_U;

  int count, i,j,k;
  double tauS0, Ze0, Zs0, resnorm, resnorm0, Ge_fac0, alpha0;
  FILE *ifp;

  ifp=fopen(fname, "a"); count=0;
  fprintf(ifp,"%8s %15s %15s %15s %15s %15s %15s\n",  "#iter",     "chi2",     "tauS", "Ze", "Zs", "Ge", "Ge_fac"); // header
  fprintf(ifp,"%8s %15s %15s %15s %15s %15s %15s\n", "------", "--------", "--------", "--------", "--------", "--------", "--------"); // header


  for(k=0; k<Nseeds; k++)
{
  /* initial paramter values */
  if(k==0)
  {
    estimate_StickyReptation_parameters_from_data(Model, Set->w1, Set->Gelas, Set->Gvisc, Set->Ndata, Set->N, Set->v, Set->phi, Set->T);

    calculateResnorm(Set, Model, &resnorm0);

    // store initial parameters 
    (*resnormBest)=resnorm0; (*tauSbest)=Model->tauS; (*Zebest)=Model->Ze; (*Zsbest)=Model->Zs;
    tauS0=Model->tauS; Ze0=Model->Ze; Zs0=Model->Zs, Ge_fac0=Model->Ge_fac, alpha0=Model->alpha;

  }
  else
  {
    Model->alpha    =alpha_L + drand(alpha_U-alpha_L);
    Model->tauS    =pow(10,  log10(tauS_L)+drand( log10(tauS_U/tauS_L) ));
    Model->Zs      =  Zs_L + drand(Zs_U-Zs_L);
    Model->Ze      =  Ze_L + drand(Ze_U-Ze_L);
    Model->Ge_fac  =  Ge_L + drand(Ge_U-Ge_L);
  }

  if(!G_StickyReptation_at_alpha_tauS_Zs_Ze(Model, Model->Hrow, Model->NH, 1.0/Model->tnum_rep_U, 1.0/Model->tnum_rep_L, Model->Nnum_rep, Model->omega_mat, Model->Gelas_mat, Model->Gvisc_mat))
  {printf("Error: G_StickyReptation_at_alpha_tauS_Zs_Ze() failed!\n"); exit(1);}
  calculateResnorm(Set, Model, &resnorm0);


  calculateResnorm(Set, Model, &resnorm0);

  // store initial parameters 
  tauS0=Model->tauS; Ze0=Model->Ze; Zs0=Model->Zs, Ge_fac0=Model->Ge_fac, alpha0=Model->alpha;

  // Optimise parameters
  for(j=0; j<Niter; j++)
  {
    //-----------------------------------------------------------------
    // Propose new parameters
    Model->alpha   =MC_propose_new_parameter(Model->alpha , delta, alpha_L, alpha_U);
    Model->tauS    =MC_propose_new_parameter(Model->tauS  , delta,  tauS_L,  tauS_U);
    Model->Zs      =MC_propose_new_parameter(Model->Zs    , delta,    Zs_L,    Zs_U);
    Model->Ze      =MC_propose_new_parameter(Model->Ze    , delta,    Ze_L,    Ze_U);
    Model->Ge_fac  =MC_propose_new_parameter(Model->Ge_fac, delta,    Ge_L,    Ge_U);
    // New parameters proposed
    //-----------------------------------------------------------------  

    //-----------------------------------------------------------------  
    // Evaluate parameters
    if(!G_StickyReptation_at_alpha_tauS_Zs_Ze(Model, Model->Hrow, Model->NH, 1.0/Model->tnum_rep_U, 1.0/Model->tnum_rep_L, Model->Nnum_rep, Model->omega_mat, Model->Gelas_mat, Model->Gvisc_mat))
    {printf("Error: G_StickyReptation_at_alpha_tauS_Zs_Ze() failed!\n"); printf("Ze=%f\n", Model->Ze); return(0);}
    calculateResnorm(Set, Model, &resnorm);
    // Parameters evaluated 
    //-----------------------------------------------------------------  

    //-----------------------------------------------------------------  
    // Accept/reject parameters 
    if(resnorm<resnorm0 | ((double)rand()/RAND_MAX)<exp(-(resnorm-resnorm0)/TSA) ) // ACCEPT
    {
      fprintf(ifp,"%8d %15f %15f %15f %15f %15f %15f %15f\n", j, resnorm, Model->tauS, Model->Ze, Model->Zs, Model->Ge, Model->Ge_fac, Model->alpha); resnorm0=resnorm;
      tauS0=Model->tauS; Ze0=Model->Ze; Zs0=Model->Zs; Ge_fac0=Model->Ge_fac;

      if(resnorm0<(*resnormBest))
      {
        (*resnormBest)=resnorm0; (*tauSbest)=Model->tauS; (*Zebest)=Model->Ze; (*Zsbest)=Model->Zs;(*Gefacbest)=Model->Ge_fac;(*alphabest)=Model->alpha;
      }
    }
    else // REJECT
    { count++;
      fprintf(ifp,"%8d %15f %15f %15f %15f %15f %15f %15f\n", j, resnorm, Model->tauS, Model->Ze, Model->Zs, Model->Ge, Model->Ge_fac, Model->alpha);
if(verbose & (count%50==0))
{
  printf("#rejections: %d/%d = %f %%\n", count, j, 100.0*count/(j+1));
}
      Model->tauS=tauS0; Model->Ze=Ze0, Model->Zs=Zs0; Model->Ge_fac=Ge_fac0; Model->alpha=alpha0;
    }
    // Parameters accepted/rejected
    //----------------------------------------------------------------- 
}
  } // End for loop Monte Carlo steps
  fclose(ifp);

  return(1);
}
