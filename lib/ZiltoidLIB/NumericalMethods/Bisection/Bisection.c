#include "Bisection.h"


int  find_index_closest_to_Yvalue(double* y, int Ndata, double y0, int *ind )
{
  int mode;
  int indL=0, indU=Ndata, indnew;
  double yL, yU; // y values Lower and Upper around y0
  double f; // used for linear interpolation

  if(  y[Ndata-1]>y[0]  ) // Mode 1: increasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=1;
  }
  else if (  y[Ndata-1]<y[0]  ) // Mode 2: decreasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=2;
  }
  else
    {printf("ERROR: y[0]=%e equals y[Ndata-1]=%e", y[0], y[Ndata-1]); return(0);}

  if ((y0<yL) | (y0>yU))
    {printf("ERROR: y0=%e out of boundary [%e; %e]\n", y0, yL, yU); return(0);}

  //--------------------------------------  
  // find two y values closest to y0
  while( (indU-indL)>1 )
  {
    indnew=indL+(int)(0.5*(indU-indL)); 
    if(  y[indnew] > y0 )
    {
      yU=y[indnew];
      if(mode==1)
      {
        indU=indnew;
      }
      else if(mode==2)
      {
        indL=indnew;
      }
    }
    else if(  y[indnew] < y0 )
    {
      yL=y[indnew];
      if(mode==1)
      {
        indL=indnew;
      }
      else if(mode==2)
      {
        indU=indnew;
      }
    }
    else if (y[indnew]==y0)
    {(*ind)=indnew; return(1);}
  }
  // Two y values closest to y0 found.
  //--------------------------------------  

  //--------------------------------------
  printf("%e %e\n", yU-y0, y0-yL);
  if( (yU-y0) < (y0-yL) )
    (*ind)=(mode==1? indU : indL);
  else
    (*ind)=(mode==1? indL : indU);


  return(1);
}  

int  find_index_above_y0_float(float* y, int Ndata, float y0, int *ind )
{
  int mode;
  int indL=0, indU=Ndata, indnew;
  float yL, yU; // y values Lower and Upper around y0
  float f; // used for linear interpolation

  if(  y[Ndata-1]>y[0]  ) // Mode 1: increasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=1;
  }
  else if (  y[Ndata-1]<y[0]  ) // Mode 2: decreasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=2;
  }
  else
    {printf("ERROR: y[0]=%e equals y[Ndata-1]=%e", y[0], y[Ndata-1]); return(0);}

  if (y0<yL)
  {(*ind)=0; return(0);}

  if ((y0>yU))
    {printf("ERROR: y0=%e out of boundary [%e; %e]\n", y0, yL, yU); return(0);}

  //--------------------------------------  
  // find two y values closest to y0
  while( (indU-indL)>1 )
  {
    indnew=indL+(int)(0.5*(indU-indL)); 
    if(  y[indnew] > y0 )
    {
      yU=y[indnew];
      if(mode==1)
      {
        indU=indnew;
      }
      else if(mode==2)
      {
        indL=indnew;
      }
    }
    else if(  y[indnew] < y0 )
    {
      yL=y[indnew];
      if(mode==1)
      {
        indL=indnew;
      }
      else if(mode==2)
      {
        indU=indnew;
      }
    }
    else if (y[indnew]==y0)
    {(*ind)=indnew; return(1);}
  }
  // Two y values closest to y0 found.
  //--------------------------------------  

  //--------------------------------------
  (*ind)=indU;

  return(1);
}  

int  find_index_above_y0_double(double* y, int Ndata, double y0, int *ind )
{
  int mode;
  int indL=0, indU=Ndata, indnew;
  double yL, yU; // y values Lower and Upper around y0
  double f; // used for linear interpolation

  if(  y[Ndata-1]>y[0]  ) // Mode 1: increasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=1;
  }
  else if (  y[Ndata-1]<y[0]  ) // Mode 2: decreasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=2;
  }
  else
    {printf("ERROR: y[0]=%e equals y[Ndata-1]=%e", y[0], y[Ndata-1]); return(0);}

  if (y0<yL)
  {(*ind)=0; return(0);}

  if ((y0>yU))
    {printf("ERROR: y0=%e out of boundary [%e; %e]\n", y0, yL, yU); return(0);}

  //--------------------------------------  
  // find two y values closest to y0
  while( (indU-indL)>1 )
  {
    indnew=indL+(int)(0.5*(indU-indL)); 
    if(  y[indnew] > y0 )
    {
      yU=y[indnew];
      if(mode==1)
      {
        indU=indnew;
      }
      else if(mode==2)
      {
        indL=indnew;
      }
    }
    else if(  y[indnew] < y0 )
    {
      yL=y[indnew];
      if(mode==1)
      {
        indL=indnew;
      }
      else if(mode==2)
      {
        indU=indnew;
      }
    }
    else if (y[indnew]==y0)
    {(*ind)=indnew; return(1);}
  }
  // Two y values closest to y0 found.
  //--------------------------------------  

  //--------------------------------------
  (*ind)=indU;

  return(1);
}  

//------------------------------------------------------------------------
// Find x value x0 where y takes y value y0
//
// Algorithm: Bisection method followed by linear interpolation
// Assumptions
//         1. y is monotonically increasing or decreasing
//         2. x is monotonically increasing
     
int  find_X_at_Yvalue(double* x, double* y, int Ndata, double y0, double *x0)
{
  int mode;
  int indL=0, indU=Ndata, indnew;
  double yL, yU; // y values Lower and Upper around y0
  double f; // used for linear interpolation

  if(  y[Ndata-1]>y[0]  ) // Mode 1: increasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=1;
  }
  else if (  y[Ndata-1]<y[0]  ) // Mode 2: decreasing y
  {
    yU=y[Ndata-1]; yL=y[0]; mode=2;
  }
  else
    {printf("ERROR: y[0]=%e equals y[Ndata-1]=%e", y[0], y[Ndata-1]); return(0);}

  if ((y0<yL) | (y0>yU))
    {printf("ERROR: y0=%e out of boundary [%e; %e]\n", y0, yL, yU); return(0);}
  if(x[Ndata-1]<=x[0])
    {printf("ERROR: x is not monotonically increasing!\n"); return(0);}

  //--------------------------------------  
  // find two y values closest to y0
  while( (indU-indL)>1 )
  {
    indnew=indL+(int)(0.5*(indU-indL)); 
    if(  y[indnew] > y0 )
    {
      yU=y[indnew];
      if(mode==1)
      {
        indU=indnew;
      }
      else if(mode==2)
      {
        indL=indnew;
      }
    }
    else if(  y[indnew] < y0 )
    {
      yL=y[indnew];
      if(mode==1)
      {
        indL=indnew;
      }
      else if(mode==2)
      {
        indU=indnew;
      }
    }
    else if (y[indnew]==y0)
    {(*x0)=x[indnew]; return(1);}
  }
  // Two y values closest to y0 found.
  //--------------------------------------  

  //--------------------------------------
  // Linear interpolation
  if(x[indU]<x[indL])
    {printf("ERROR: x is not monotonically increasing!\n"); return(0);}

  f=(y0-yL)/(yU-yL);
  f=(mode==1 ? f: 1-f );
  (*x0)=x[indL ] +  f*(x[indU]-x[indL]) ;

  return(1);
}  



