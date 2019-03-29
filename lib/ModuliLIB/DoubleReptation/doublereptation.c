/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include "doublereptation.h"

/*
  Calculate plateau modulus, Ge, in [Pa]
  kT:  Thermal energy in [J]
  N:   Number of monomers per polymer chain []
  v:   Volume per monomer [m3]
  phi: Volume fraction of the polymer [-]
  Ze:  Number of entanglements per polymer chain [-]
*/
double PlateauModulus(double kT, int N, double v, double phi, double Ze)
{
  double rho=phi/(v*N); // Calculate number density of polymers
  double rhoe=rho*Ze;   // Calculate number density of entanglements
  double Ge=0.8*rhoe*kT; //L. J. Fetters, D. J. Lohse, and W. W. Graessley, J. polym. Sci. B, polym. Phys. Edn. 37, 1023 (1999)
  return Ge;
}


/*
  gx, gy, Ng: interpolation of the g function
  Fx, Fy, NF: interpolation of the F function
  
  tau: time in units of the reptation time
  H:   proportional to the number of entanglements per chain
*/

double G_doublereptation(double *gx, double *gy, int Ng, double *Fx, double *Fy, int NF, double H, double tau )
{
  double U = tau + g_descloizeaux_interpolation(gx, gy, Ng,  H*tau)/H;
  double F =       F_descloizeaux_interpolation(Fx, Fy, NF,  U);
  double Grep=F*F;
  return Grep;
}


int    G_doublereptation_interpolation(double omega_L, double omega_U, int Nsample, double *omega_in, double *Gelas_in, double *Gvisc_in, double omega_out, double *Gelas_out, double *Gvisc_out)
{
  double gelas, gvisc;

  if(omega_out<omega_L)      // extrapolate: small frequencies -> G' scales as w^2 and G'' as w^1
  {
    interp1(omega_in, Gelas_in, Nsample, omega_L, &gelas);
    (*Gelas_out)=gelas*pow(omega_out/omega_L, 2);
    interp1(omega_in, Gvisc_in, Nsample, omega_L, &gvisc);
    (*Gvisc_out)=gvisc*pow(omega_out/omega_L, 1);
  }
  else if(omega_out>omega_U) // extrapolate: high frequencies -> G'~1 and G''~w^{-0.25} 
  {
    interp1(omega_in, Gelas_in, Nsample, omega_U, &gelas);
    (*Gelas_out)=1-(gelas)*exp(-log(omega_out)/log(omega_U));  // TODO: empirical approximation: is there an exact high frequency limit?

    interp1(omega_in, Gvisc_in, Nsample, omega_U, &gvisc);
    (*Gvisc_out)=gvisc*pow(omega_out/omega_U, -0.25);          // TODO: empirical approximation: is there an exact high frequency limit?
  }
  else                       // interpolate
  {
    interp1(omega_in, Gelas_in, Nsample, omega_out, &gelas);
    interp1(omega_in, Gvisc_in, Nsample, omega_out, &gvisc);

    (*Gelas_out)=gelas;
    (*Gvisc_out)=gvisc;
  }
  
  return(1);
}

/*
  DOUBLE REPTATION MODEL: INITIALISE

    Calculate dynamic moduli in a time interval
    > memory is allocated within the function
    > results can later be used to interpolate and extrapolate    
    > results are dimensionless:
      - Moduli is given in units of the plateau modulus Ge 
        (Ge is in principle proportional to the number density of 
         entanglements times the thermal energy)
      - Time is given in units of the reptation time
        (reptation time is proportional to the cube of the 
         molecular weight)

  1. Numerical settings:
    typical values for numerical settings:
      Set_Ng:   100
      Set_NF:   100
      Set_tol: 1e-4
    More insight into these parameters may be obtained using
      demo_descloizeaux.c, 
    which is compiled with
      compile_descloizeaux.sh
  2. Physical settings:
    H: Materials parameter - only physical parameter.
    Small H (<<1) implies the terminal relaxation time
    is smaller than the reptation time. This can be due to fast
    relaxation processes due to contour-length fluctuations.
  3. Time range:
    tau_L:   shortest time (in units of reptation time)
    tau_U:   longest  time (in units of reptation time)
    Nsample: number of time samples in this interval (log scaled)

  4. Output:
    omega:   angular frequency (in units of inverse reptation time)
    Gelas:   elastic/storage modulus (in units of plateau modulus)
    Gvisc:   viscous/loss    modulus (in units of plateau modulus)
*/
int G_doublereptation_initialise(int Set_Ng, int Set_NF, double Set_tol, double H, double tau_L, double tau_U, int Nsample, double **omega, double **Gelas, double **Gvisc)
{
  int i;
  double *gx, *gy, *Fx, *Fy; // Arrays with solutions of the g() and F() functions of des cloizeaux
  double w, gelas, gvisc;

  //----------------------------------
  // Double-reptation model initialise
  double *time_interp   =(double*)malloc(Nsample*sizeof(double));
  double *Grelax_interp =(double*)malloc(Nsample*sizeof(double));


  (*omega) = (double*)malloc(Nsample*sizeof(double));
  (*Gelas) = (double*)malloc(Nsample*sizeof(double));
  (*Gvisc) = (double*)malloc(Nsample*sizeof(double));

  g_descloizeaux_initialise(&gx, &gy, Set_Ng, Set_tol);
  F_descloizeaux_initialise(&Fx, &Fy, Set_NF, Set_tol);


  // relaxation modulus
  for(i=0; i<Nsample; i++)
  {
    time_interp[i]  =tau_L*pow(tau_U/tau_L, (double)i/(Nsample-1));
    Grelax_interp[i]= G_doublereptation(gx, gy, Set_Ng, Fx, Fy, Set_NF, H, time_interp[i] );
  }

  for(i=0; i<Nsample; i++)
  {
    w=(1.0/tau_U)*pow(tau_U/tau_L, (double)i/(Nsample-1) );
    NobileFEA(time_interp, Grelax_interp, Nsample, w, &gelas, &gvisc);
    (*omega)[i]=w;
    (*Gelas)[i]=gelas;
    (*Gvisc)[i]=gvisc;
  }

  free(gx);
  free(gy);
  free(Fx);
  free(Fy);
  free(time_interp);
  free(Grelax_interp);

  return(1);
}

/*
  J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
  limiting behavior:
  g(x)=sqrt(pi*x) for x<<1
  g(x)=(pi^2/6)         for x>>1 ( in this limit g=\sum_{m=1}^{\infty}m^{-2} )
*/
double g_descloizeaux(double x, double tol)
{
  
  int m, m2;
  double err=2*tol; // initialise
  double dg, g=0;
  m=0;
  while (err>tol)
  {
    m++;
    m2=m*m;
    dg=(1.0-exp(-m2*x))/m2;
    g+=dg;

    err=dg/g;
  }
  return g;
}


double get_g_descloizeaux_convergence_tolerance(double tol)
{
  // Determine convergence tolerance = dg_num/g_num
  // required to obey tol = (g_exact-g_num)/g_num
  
  double  g=0.0, dg;
  double err=2*tol;
  double gexact=M_PI*M_PI/6;
  double tolConvergence;
  int m=0;

  while(err>tol)
  {
    m++;
    dg=1.0/(m*m);
    g+=dg;
    err=(gexact-g)/g;
  }
  tolConvergence=dg/g;
  return tolConvergence;
}

int get_g_descloizeaux_boundaries( double tol, double *xL, double *xU )
{ 
  double x, g=0, err;
  double tolConvergence=get_g_descloizeaux_convergence_tolerance(tol);

  // Find range to satisfy tolerance level
  // lower boundary
  x=1.0;
  err=2*tol;
  while (err>tol & !isnan(err))
  {
    x*=0.5;
    g=g_descloizeaux(x, tolConvergence);
    err=( g-sqrt(M_PI*x) )/g;  
    err=( err<0?-err:err ); // absolute value 
    if(isnan(err))
      x*=2;
  }
  *xL=x;
  // upper boundary
  x=1.0;
  err=2*tol;
  while (err>tol & !isinf(x))
  {

    g=g_descloizeaux(x, tolConvergence);
    err=( g-(M_PI*M_PI/6) )/g;
    err=( err<0?-err:err ); // absolute value
    x*=2;
  }
  *xU=x;
  return(1);
}

/*
  Requires intialisation of xsample and gsample
*/
double g_descloizeaux_interpolation(double *xsample, double *gsample, int Nsample, double x)
{
  double gint;
  if(x<xsample[0])               // Extrapolate 
    gint=sqrt(M_PI*x);           //   approximation for x<<1
  else if (x>xsample[Nsample-1]) // Extrapolate x>>1
    gint=M_PI*M_PI/6;            //   approximation for x>>1
  else                           // Interpolate
    interp1(xsample, gsample, Nsample, x, &gint);

  return gint;
}

int g_descloizeaux_initialise(double **xsample, double **gsample, int Nsample, double tol)
{
  int i;
  double x;
  double xL, xU, log10_xL, log10_xU;
  double tolConvergence;
  // Allocate memory
  (*xsample)=(double*)malloc(Nsample*sizeof(double));
  (*gsample)=(double*)malloc(Nsample*sizeof(double));

  // set interpolation interval
  get_g_descloizeaux_boundaries( tol, &xL, &xU );
  log10_xL=log10(xL);
  log10_xU=log10(xU);
  tolConvergence=get_g_descloizeaux_convergence_tolerance(tol);

  // get interpolation values
  for (i=0; i<Nsample; i++)
  {
    x=xL*pow(xU/xL, (double)i/(Nsample-1)); //pow(10, ( log10_xL + (log10_xU-log10_xL)*i/(Nsample-1) ));
    (*xsample)[i]= x;
    (*gsample)[i]=g_descloizeaux(x, tolConvergence);

  }
  return(1);
}


/*
  F(U)=1 for U<<1 
  F(U)=(8/pi^2)*exp(-U) for U>>1
*/
double F_descloizeaux(double U, double tol)
{  
  int q, q2;
  double err=2*tol; // initialise
  double dF, F=0;
  q=1;
  while (err>tol)
  {
    q2=q*q;
    dF=exp(-q2*U)/q2;
    F+=dF;

    err=dF/F;

    q+=2;
  }
  F*=(double)8/(M_PI*M_PI);
  return F;
}


double get_F_descloizeaux_convergence_tolerance(double tol)
{
  // Determine convergence tolerance = dg_num/g_num
  // required to obey tol = (g_exact-g_num)/g_num
  
  double  F=0.0, dF;
  double err=2*tol;
  double Fexact=M_PI*M_PI/8;
  double tolConvergence;
  int q=1;

  while(err>tol)
  {
    dF=1.0/(q*q);
    F+=dF;
    err=(Fexact-F)/F;
    q+=2;
  }
  tolConvergence=dF/F;
  return tolConvergence;
}


int get_F_descloizeaux_boundaries( double tol, double *UL, double *UU )
{ 
  double U, F=0, err;
  double tolConvergence=get_F_descloizeaux_convergence_tolerance(tol);

  // Find range to satisfy tolerance level
  // lower boundary
  U=1.0;
  err=2*tol;
  while (err>tol & !isnan(err))
  {
    U*=0.5;
    F=F_descloizeaux(U, tolConvergence);
    err=( F-1.0 )/F;  
    err=( err<0?-err:err ); // absolute value 
    if(isnan(err))
      U*=2;
  }
  *UL=U;
  // upper boundary
  U=1.0;
  err=2*tol;
  while (err>tol & !isinf(U))
  {

    F=F_descloizeaux(U, tolConvergence);
    err=( F-(8.0/(M_PI*M_PI)*exp(-U)) )/F;
    err=( err<0?-err:err ); // absolute value
    U*=2;
  }
  *UU=U;
  return(1);
}

/*
  Requires intialisation of xsample and gsample
*/
double F_descloizeaux_interpolation(double *Usample, double *Fsample, int Nsample, double U)
{
  double Fint;
  if(U<Usample[0])               // Extrapolate 
    Fint=1.0;           //   approximation for x<<1
  else if (U>Usample[Nsample-1]) // Extrapolate x>>1
    Fint=8.0/(M_PI*M_PI)*exp(-U);            //   approximation for x>>1
  else                           // Interpolate
    interp1(Usample, Fsample, Nsample, U, &Fint);

  return Fint;
}

int F_descloizeaux_initialise(double **Usample, double **Fsample, int Nsample, double tol)
{
  int i;
  double U;
  double UL, UU;
  double tolConvergence;
  // Allocate memory
  (*Usample)=(double*)malloc(Nsample*sizeof(double));
  (*Fsample)=(double*)malloc(Nsample*sizeof(double));

  // set interpolation interval
  get_F_descloizeaux_boundaries( tol, &UL, &UU );

  tolConvergence=get_F_descloizeaux_convergence_tolerance(tol);

  // get interpolation values
  for (i=0; i<Nsample; i++)
  {
    U=UL*pow(UU/UL, (double)i/(Nsample-1)); //pow(10, ( log10_xL + (log10_xU-log10_xL)*i/(Nsample-1) ));
    (*Usample)[i]= U;
    (*Fsample)[i]=F_descloizeaux(U, tolConvergence);

  }
  return(1);
}


