/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer
*/
#include "StickyRouse.h"
/*-------------------------------------------------------------------------------------------- 
  Sticky Rouse

  Prefactor (1/5) for q<Ze: 
    Milner and McLeish, Phys. Rev. Lett., 81, 725--728 (1998)
    Likhtman and McLeish, Macromolecules 35, 6332--6343 (2002)
    Chen et al., ACS Macro Lett., 2, 970 (2013)
    Chen et al., J. Rheol. 57, 1441 (2013)

  N:    Degree of polymerisation
  Ze:   Number of entanglements per chain (average -> floating value)
  Zs:   Number of stickers per chain (average -> floating value)
  tau:  Time in units of the Rouse time of the entire chain
  tauS: Sticker dissociation time (in units of the Rouse time of a segment)

  Modulus in units of the plateau modulus Ge ~ (4/5) rho Ze kT, with rho 
  the number density of linear polymers

  NOTE: 
    Zs = 0 gives usual Rouse relaxation
    N  = 1 ignores relaxation at times t<tauS
*/
// Relaxation modulus
double Grelax_stickyrouse(int N, double Ze, double Zs, double tauS, double tau)
{
  int q,count;
  double sum, G=0;
  double dsum;
  double tauShort=tau/(N*N);
  double tauLong=(Zs>0?tau/(tauS*Zs*Zs):tau);
  // Short wavelengths (strands between stickers)
  sum=0.0; dsum=0.0; count=0;
  for (q=1+(int)(Zs); q<=N; q++)
  {  
    count++;
    if(q<Ze)
    {
      sum += (1.0/5)*exp(  -tauShort*q*q  );
      dsum+= 1.0/5;
    }
    else
    {
      sum +=         exp(  -tauShort*q*q  );
      dsum+= 1;
    }
  }
  if(N>1)
    G=(sum/Ze)*((double)count/dsum); 


  // Long wavelengths (longer than the strands between stickers)
  sum=0.0;dsum=0.0; count=0;
  for (q=1; q<=(int)(Zs); q++)
  { 
    count++;
    if(q<Ze)
    {
      sum += (1.0/5)*exp(  -tauLong*q*q  );
      dsum+= 1.0/5;
    }
    else
    {
      sum +=         exp(  -tauLong*q*q  );
      dsum+= 1;
    }
  }
  if(Zs>0)
    G+=((double)count/dsum)*(sum/Ze); 

  return G;    
}

// elastic/storage modulus G'
double Gelas_stickyrouse(int N, double Ze, double Zs, double tauS, double omega)
{
  int q,count;
  double sum, G=0;
  double sumf;
  double omegaShort=omega*(N*N);
  double omegaLong=omega*(tauS*Zs*Zs);

  // Short wavelengths (strands between stickers)
  sum=0.0; sumf=0.0; count=0;
  for (q=1+(int)(Zs); q<=N; q++)
  {  
    count++;
    if(q<Ze)
    {
      sum += (1.0/5)*Gelas_maxwell( omegaShort/(q*q) ); 
      sumf+= 1.0/5;
    }
    else
    {
      sum +=         Gelas_maxwell( omegaShort/(q*q) ); 
      sumf+= 1;
    }
  }
  if(N>1)
    G=(sum/Ze)*((double)count/sumf); 


  // Long wavelengths (longer than the strands between stickers)
  sum=0.0;sumf=0.0; count=0;
  for (q=1; q<=(int)(1.0*Zs); q++)
  { 
    count++;
    if(q<Ze)
    {
      sum += (1.0/5)*Gelas_maxwell( omegaLong/(q*q) ); 
      sumf+= 1.0/5;
    }
    else
    {
      sum +=         Gelas_maxwell( omegaLong/(q*q) ); 
      sumf+= 1;
    }
  }
  if(Zs>0)
    G+=((double)count/sumf)*(sum/Ze); 

  return G;    
}

// viscous/loss modulus G''
double Gvisc_stickyrouse(int N, double Ze, double Zs, double tauS, double omega)
{
  int q,count;
  double sum, G=0;
  double sumf;
  double omegaShort=omega*(N*N);
  double omegaLong=omega*(tauS*Zs*Zs);

  // Short wavelengths (strands between stickers)
  sum=0.0; sumf=0.0; count=0;
  for (q=1+(int)(Zs); q<=N; q++)
  {  
    count++;
    if(q<Ze)
    {
      sum += (1.0/5)*Gvisc_maxwell( omegaShort/(q*q) );
      sumf+= 1.0/5;
    }
    else
    {
      sum +=         Gvisc_maxwell( omegaShort/(q*q) );
      sumf+= 1;
    }
  }
  if(N>1)
    G=(sum/Ze)*((double)count/sumf); 


  // Long wavelengths (longer than the strands between stickers)
  sum=0.0;sumf=0.0; count=0;
  for (q=1; q<=(int)(1.0*Zs); q++)
  { 
    count++;
    if(q<Ze)
    {
      sum += (1.0/5)*Gvisc_maxwell( omegaLong/(q*q) );
      sumf+= 1.0/5;
    }
    else
    {
      sum +=         Gvisc_maxwell( omegaLong/(q*q) ); 
      sumf+= 1;
    }
  }
  if(Zs>0)
    G+=((double)count/sumf)*(sum/Ze); 

  return G;    
}




