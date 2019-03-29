/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include "StickyReptation.h"


SREP *make_SREP(void)
{
  SREP *Param=(SREP *)malloc(sizeof(SREP));


  Param->Ge_fac=1.0;   
  Param->alpha=10.0; // Default value


  Param->w1    =NULL;
  Param->Gelas =NULL;
  Param->w2    =NULL;
  Param->Gvisc =NULL;
  Param->Hrow  =NULL;
  Param->omega_mat=NULL;
  Param->Gelas_mat=NULL;
  Param->Gvisc_mat=NULL;


  return(Param);
}


int free_SREP(SREP *Param)
{
  int i;
  if(Param!=NULL)
  {
    if(Param->w1!=NULL)
    {free(Param->w1); Param->w1=NULL;}
    if(Param->Gelas!=NULL)
    {free(Param->Gelas); Param->Gelas=NULL;}
    if(Param->w2!=NULL)
    {free(Param->w2); Param->w2=NULL;}
    if(Param->Gvisc!=NULL)
    {free(Param->Gvisc); Param->Gvisc=NULL;}
    if(Param->Hrow!=NULL)
    {free(Param->Hrow); Param->Hrow=NULL;}

    if(Param->omega_mat!=NULL)
    {
      for(i=0; i<Param->NH; i++)
        if(Param->omega_mat[i]!=NULL)
          {free(Param->omega_mat[i]); Param->omega_mat[i]=NULL; }

      free(Param->omega_mat); Param->omega_mat=NULL;
    }

    if(Param->Gelas_mat!=NULL)
    {
      for(i=0; i<Param->NH; i++)
        if(Param->Gelas_mat[i]!=NULL)
          {free(Param->Gelas_mat[i]); Param->Gelas_mat[i]=NULL; }

      free(Param->Gelas_mat); Param->Gelas_mat=NULL;
    }

    if(Param->Gvisc_mat!=NULL)
    {
      for(i=0; i<Param->NH; i++)
        if (Param->Gvisc_mat[i]!=NULL)
          {free(Param->Gvisc_mat[i]); Param->Gvisc_mat[i]=NULL; }

      free(Param->Gvisc_mat); Param->Gvisc_mat=NULL;
    }

    free(Param); Param=NULL;
  }
  return(1);
}
/*
  INPUT:
  omega: Angular frequency [rad/s]
  Gelas: Elastic modulus
  Gvisc: Viscous modulus
  Ndata: Number of data points
  N:     Number of monomers per chain
  V:     Volume of a monomer
  phi:   Volume fraction of the polymer
  T:     Temperature

  OUTPUT:
  Estimates for Zs, Ze, and tauS, stored in
  Model->Zs
  Model->Ze
  Model->tauS  
*/
int estimate_StickyReptation_parameters_from_data(SREP *Model, double *omega, double *Gelas, double *Gvisc, int Ndata, double N, double V, double phi, double T)
{
  int i;
  double Ge_est=0,Gvisc_max=0,C;
  double kB=1.3806503e-23; // Boltzmann constant [J/K] 

  for(i=0; i<Ndata; i++)
  { 
    Ge_est=(Gelas[i]>Ge_est?Gelas[i]:Ge_est); // Maximum in Gelas
    if(Gvisc[i]>Gvisc_max)                  // Maximum in Gvisc
    {
      Gvisc_max=Gvisc[i];
      Model->tauS=1.0/omega[i];        // estimated sticker dissociation time
    }
  }

  C=0.8*phi*kB*T/(N*V); 
  Model->Ze=Ge_est/C; // Estimate Ze from plateau modulus
  
  Model->Zs=Model->Ze; // Estimate Zs=Ze 

  //if(!getCrossoverFrequency(Set->w1,Set->Gvisc,Set->Gelas,Set->Ndata, &taucross))
  //{printf("ERROR: getCrossoverFrequency() failed!\n"); exit(1);}
  //taucross=1.0/taucross; // Might be used to estimate terminal relaxation time

  return(1);
}


int G_StickyReptation_interp2(SREP *Model, double tau_rep, double H,  double *Hrow, int NH, double wL, double wU, int Nnum_rep, double **omega_mat, double **Gelas_mat, double **Gvisc_mat, double omega,  double *gvisc, double *gelas)
{

int N=Model->N; double Ze=Model->Ze; double Zs=Model->Zs; double tauS=Model->tauS; double Ge=Model->Ge; double tau0=Model->tau0;

  int indL, indU;
  double f;
  double gelasL, gelasU;
  double gviscL, gviscU;

  if(H<Hrow[0] | H>Hrow[NH-1]) // error capture
  {printf("Error: H=%e outside interval [%e : %e]\n", H, Hrow[0], Hrow[NH-1]); return(0);}

  // Interpolate H in Hrow
  find_boundary_indices(Hrow, Hrow, NH, H, &indL, &indU);

  G_StickyReptation_at_fixed_alpha(Model, tau_rep, wL, wU, Nnum_rep, omega_mat[indL], Gelas_mat[indL], Gvisc_mat[indL], omega, &gelasL, &gviscL);
  G_StickyReptation_at_fixed_alpha(Model, tau_rep, wL, wU, Nnum_rep, omega_mat[indU], Gelas_mat[indU], Gvisc_mat[indU], omega, &gelasU, &gviscU);

  f=(H-Hrow[indL])/(Hrow[indU]-Hrow[indL]);
  (*gelas) = (1-f)*gelasL + f*gelasU;
  (*gvisc) = (1-f)*gviscL + f*gviscU;

  return(1);
}

int G_StickyReptation_at_fixed_alpha(SREP *Model,  double tau_rep, double wrep_L, double wrep_U, int Nwrep, double *omega_interp, double *Gelas_interp, double *Gvisc_interp, double omega, double *gelas, double *gvisc)
{
  int N=Model->N; 
  double Ze=Model->Ze, Zs=Model->Zs, tauS=Model->tauS, Ge=Model->Ge, tau0=Model->tau0;

  double gelas_sr, gvisc_sr, gelas_dr, gvisc_dr;

  // Contribution sticky Rouse (time in units of tau0)
  gelas_sr = Gelas_stickyrouse(N, Ze, Zs, tauS/tau0, omega*tau0);
  gvisc_sr = Gvisc_stickyrouse(N, Ze, Zs, tauS/tau0, omega*tau0);

  // Contribution double reptation (time in units of tau_rep)
  G_doublereptation_interpolation(wrep_L, wrep_U, Nwrep, omega_interp, Gelas_interp,Gvisc_interp, omega*tau_rep, &gelas_dr, &gvisc_dr);

  (*gelas) = Ge*(gelas_sr + gelas_dr);
  (*gvisc) = Ge*(gvisc_sr + gvisc_dr);
  return(1);
}





/*
  G' and G'' are calculated over a specified frequency range

  Free parameters:
    alpha: materials parameter
    tauS:  sticker dissociation time
    Ze:    number of entanglements
    Zs:    number of stickers
  
  Dependent parameters:
    tau_rep: reptation time (depends in tauS, Ze, Zs)
    H:       materials parameter (depends on alpha and Ze)
    Ge:      depends on Ze   
*/
int G_StickyReptation_at_alpha_tauS_Zs_Ze(SREP *Model, double *Hrow, int NH, double wL, double wU, int Nnum_rep, double ** omega_mat, double ** Gelas_mat, double ** Gvisc_mat)
{
  int i;
  double tau_rep=Model->tauS*Model->Zs*Model->Zs*Model->Ze;
  double H=Model->Ze/Model->alpha;
  double omega;
  double kB=1.3806503e-23; // Boltzmann constant [J/K] 

  Model->Ge=PlateauModulus(kB*Model->T, Model->N, Model->v, Model->phi, Model->Ze);
  Model->Ge *= Model->Ge_fac; 
  for(i=0; i<Model->Ndata; i++)
  {
    omega=Model->w1[i];
    if(!G_StickyReptation_interp2(Model, tau_rep, H, Hrow, NH, wL, wU, Nnum_rep, omega_mat, Gelas_mat, Gvisc_mat, omega, &Model->Gvisc[i], &Model->Gelas[i] ))
    {printf("Error: G_StickyReptation_interp2() failed!\n"); return(0);}
  }
  return(1);
}
