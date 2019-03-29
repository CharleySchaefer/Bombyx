#include "Interpolation.h"

int find_boundary_indices(double *xarr, double *yarr, int N, double xint, int *indL, int *indU)
{
  int    nL=0, nU=N-1, n;
  double x, xL, xU; // upper and lower x values
  double f;
  if (xint<(xL=xarr[nL]) | xint>(xU=xarr[nU])) // extrapolation needed
    return(2);

  if(xint==xL)
  { *indL=0; *indU=0; }
  else if (xint==xU)
  { *indL=N-1; *indU=N-1; }
  else
  {
    // find nL and nU such that nU=nL+1 and xarr[nL]<=xint<=xarr[nU]
    while(nU-nL>1) 
    {
      n=(int)(nL+0.5*(float)(nU-nL));
      x=xarr[n];
      if((x<xL)|(x>xU)) // xarr not monotonically increasing!
        {printf("ERROR: entries of xarr should be monotonically increasing!\n"); return(0);}
      if (x==xint) // exact value found -> no interpolation needed -> done.
      {
        *indL=n;*indU=n;
        return(1);
      }
      else if (x>xint) // update upper value
      {
        nU=n;
        xU=x;
      }
      else if (x<xint) // update lower value
      {
        nL=n;
        xL=x;
      }
    }
    *indL=nL;
    *indU=nU;
  }
  return(1);
}

int interp1(double *xarr, double *yarr, int N, double xint, double *yint)
{
  int    nL=0, nU=N-1, n;
  double x, xL, xU; // upper and lower x values
  double f;
  if (xint<(xL=xarr[nL]) | xint>(xU=xarr[nU])) // extrapolation needed
    return(2);

  if(xint==xL)
    *yint=yarr[0];
  else if (xint==xU)
    *yint=yarr[N-1];
  else
  {
    if(!find_boundary_indices(xarr, yarr, N, xint, &nL, &nU))
    {
      printf("ERROR: find_boundary_indices() failed!\n"); 
      return(0);
    }

    // interpolate
    if(nL==nU)
      *yint=yarr[nL];
    else
    {
      f=(xint-xarr[nL])/(xarr[nU]-xarr[nL]);
      *yint = (1-f)*yarr[nL] + f*yarr[nU];
    }
  }
  return(1);
}

/*
  mode
  0: non-periodic boundaries
  1: periodic boundaries
*/
int stretch_matrix(int mode, double **mat_in, int Nx_in, int Ny_in, double **mat_out, int Nx_out, int Ny_out)
{
  int i,j,k,l;
  double x, xL, xU; // upper and lower x values
  double F,fx,fy,G, dx, dy;

  if(Nx_out<Nx_in | Ny_out<Ny_in)
  {
    printf("Error: New Nx and Ny should be larger or equal to the original ones.\n");
    return(-1);
  }


  if(mode==0) // non-periodic
  {
    dx=(double)(Nx_in-1)/(Nx_out-1); 
    dy=(double)(Ny_in-1)/(Ny_out-1);
    for(i=0; i<Nx_out; i++)
    {
      F  = i*dx;
      k  = (int)F; // floor
      fx = 1-(F-k);

      for(j=0; j<Ny_out; j++)
      {
        F  = j*dy;
        l  = (int)F; // floor
        fy = 1-(F-l);

        G     = (  fx)*(  fy)*mat_in[k  ][l  ];
        if(k<Nx_in-1)
        {
          G+= (1-fx)*(  fy)*mat_in[k+1][l  ];
          if(l<Ny_in-1)
          {
            G+= (  fx)*(1-fy)*mat_in[k  ][l+1];
            G+= (1-fx)*(1-fy)*mat_in[k+1][l+1];
          }
        }
        else if(l<Ny_in-1)
          G+=   (  fx)*(1-fy)*mat_in[k  ][l+1];

        mat_out[i][j]=G;
      }
    }
  }
  else if(mode==1) // periodic
  {
    dx=(double)((Nx_out))/((Nx_in)); 
    dy=(double)((Ny_out))/((Ny_in)); 
    for(i=0; i<Nx_out; i++)
    {
      F  = i*dx;
      k  = (int)F; // floor
      fx = 1-(F-k);

      for(j=0; j<Ny_out; j++)
      {
        F  = j*dy;
        l  = (int)F; // floor
        fy = 1-(F-l);


        G     = (  fx)*(  fy)*mat_in[k  ][l  ];
        if(k<Nx_in-1)
        {
          G+= (1-fx)*(  fy)*mat_in[k+1][l  ];
          if(l<Ny_in-1)
          {
            G+= (  fx)*(1-fy)*mat_in[k  ][l+1];
            G+= (1-fx)*(1-fy)*mat_in[k+1][l+1];
          }
          else // periodic boundary
          {
            G+= (  fx)*(1-fy)*mat_in[k  ][0  ];
            G+= (1-fx)*(1-fy)*mat_in[k+1][0  ];
          }
        }
        else 
        {
          G+= (1-fx)*(  fy)*mat_in[0  ][l  ];
          if(l<Ny_in-1)
          {
            G+= (  fx)*(1-fy)*mat_in[k  ][l+1];
            G+= (1-fx)*(1-fy)*mat_in[0  ][l+1];
          }
          else // periodic boundary
          {
            G+= (  fx)*(1-fy)*mat_in[k  ][0  ];
            G+= (1-fx)*(1-fy)*mat_in[0  ][0  ];
          }
        }


        mat_out[i][j]=G;
      }
    }
  }
  else
  {
    printf("Error: mode should be 0 (non-periodic) or 1 (non-periodic).\n");
    return(-1);
  }

  return(1);
}
