#include <complex.h>
#include "Fourier.h"


void dft(complex *arr, complex *arrFT, int N)
{
  int m, n;
  double re, im, fac;
  complex sum;
  for (n = 0; n < N; n++)
  { 
    sum = 0;
    for (m = 0; m < N; m++) 
    {
      fac=2*M_PI*m*n/N;
      re = creal(arr[m]);
      im = cimag(arr[m]);
      sum +=  ( re*cos(fac) + im*sin(fac) ) + I*( -re*sin(fac) + im*cos(fac) );
    }
    arrFT[n] = sum;
  }
}
void dft2(complex **arr2, complex *buf, int Nx, int Ny)
{
  int     i,j;
  int     maxN=(Nx>Ny?Nx:Ny);
  complex *buf2=buf+maxN;
	
  // Fourier transform
  // 1. rows
  for (i=0; i<Nx; i++)
  {
    // Copy a row
    for ( j = 0; j < Ny; j++) 
      buf[j] = arr2[i][j];

      // Fourier transform a row
      dft(buf, buf2, Ny);
		
      // Copy back
      for ( j = 0; j < Ny; j++) 
        arr2[i][j] = buf2[j];
  }

  // 2. columns
  for (i=0; i<Ny; i++)
  {
    // Copy a row
    for ( j = 0; j < Nx; j++) 
      buf[j] = arr2[j][i];

    // Fourier transform a row
    dft(buf, buf2, Nx);

    // Copy back
    for ( j = 0; j < Nx; j++) 
      arr2[j][i] = buf2[j];
  }
}


//----------------------------------------------
//  array of length n
//  buffer of length n
//----------------------------------------------

//==============================================
// FAST FOURIER TRANSFORM  Adapted from RosettaCodeData https://github.com/acmeism/RosettaCodeData/tree/948b86eafab0e034330a3b6c31617370c6cca2fc/Task/Fast-Fourier-transform/C
void fft(complex *arr, complex *buff, int N)
{
  int i;

  for(i=0; i<N; i++)
    buff[i] = arr[i];

  fft_emb(arr, buff, N, 1);
}

void fft_emb(complex *arr, complex *buff, int N, int iter)
{
  int n;
  complex tmp;

  if (iter < N) 
  {
    fft_emb(     buff,      arr, N, 2*iter);
    fft_emb(iter+buff, iter+arr, N, 2*iter);

    for (n=0; n<N; n+=2*iter)
    {
      tmp = cexp(-M_PI*I*n/N)*buff[n+iter];
      arr[    n/2] = buff[n] + tmp;
      arr[(n+N)/2] = buff[n] - tmp;
    }
  }
}


//==============================================
// FAST FOURIER TRANSFORM - INVERSE
void ifft(complex *arr, complex *buff, int N)
{
  int i;

  for(i=0; i<N; i++)
    buff[i] = arr[i];

  ifft_emb(arr, buff, N, 1);
}

void ifft_emb(complex *arr, complex *buff, int N, int iter)
{
  int n;
  complex tmp;

  if (iter < N) 
  {
    ifft_emb(     buff,      arr, N, 2*iter);
    ifft_emb(iter+buff, iter+arr, N, 2*iter);

    for (n=0; n<N; n+=2*iter)
    {
      tmp = cexp( M_PI*I*n/N)*buff[n+iter];
      arr[    n/2] = 0.5*(buff[n] + tmp);
      arr[(n+N)/2] = 0.5*(buff[n] - tmp);
    }
  }
}
// LENGTH BUFFER SHOULD BE TWICE THE MAXIMUM LINEAR DIMENSION, i.e., 2*max(Nx,Ny)
void fft2(complex **arr2, complex *buf, int Nx, int Ny)
{
  int     i,j;
  int     maxN=(Nx>Ny?Nx:Ny);
  complex *buf2=buf+maxN;
	
  // Fourier transform
  // 1. rows
  for (i=0; i<Nx; i++)
  {
    // Copy a row
    for ( j = 0; j < Ny; j++) 
      buf[j] = arr2[i][j];

      // Fourier transform a row
      fft(buf, buf2, Ny);
		
      // Copy back
      for ( j = 0; j < Ny; j++) 
        arr2[i][j] = buf[j];
  }

  // 2. columns
  for (i=0; i<Ny; i++)
  {
    // Copy a row
    for ( j = 0; j < Nx; j++) 
      buf[j] = arr2[j][i];

    // Fourier transform a row
    fft(buf, buf2, Nx);

    // Copy back
    for ( j = 0; j < Nx; j++) 
      arr2[j][i] = buf[j];
  }
}
void ifft2(complex **arr2, complex *buf, int Nx, int Ny)
{
  int     i,j;
  int     maxN=(Nx>Ny?Nx:Ny);
  complex *buf2=buf+maxN;
	
  // Fourier transform
  // 1. rows
  for (i=0; i<Nx; i++)
  {
    // Copy a row
    for ( j = 0; j < Ny; j++) 
      buf[j] = arr2[i][j];

      // Fourier transform a row
      ifft(buf, buf2, Ny);
		
      // Copy back
      for ( j = 0; j < Ny; j++) 
        arr2[i][j] = buf[j];
  }

  // 2. columns
  for (i=0; i<Ny; i++)
  {
    // Copy a row
    for ( j = 0; j < Nx; j++) 
      buf[j] = arr2[j][i];

    // Fourier transform a row
    ifft(buf, buf2, Nx);

    // Copy back
    for ( j = 0; j < Nx; j++) 
      arr2[j][i] = buf[j];
  }
}



// LENGTH BUFFER SHOULD BE TWICE THE MAXIMUM LINEAR DIMENSION, i.e., 2*max(Nx,Ny)
void fft3(complex ***arr3, complex *buf, int Nx, int Ny, int Nz)
{	
  int i,j,k;
  int maxN=(Nx>Ny?Nx:Ny);
      maxN=(Nz>maxN?Nz:maxN);
  complex *buf2=buf+maxN;

  for(j=0; j<Ny;j++)
    for(k=0; k<Nz;k++)
    {
      for(i=0; i<Nx; i++)
        buf[i] = arr3[i][j][k];
      fft(buf, buf2, Nx);
      for(i=0; i<Nx; i++)
        arr3[i][j][k] = buf[i];
    }
  for(i=0; i<Nx; i++)
    for(k=0; k<Nz;k++)
    {
      for(j=0; j<Ny;j++)
        buf[j] = arr3[i][j][k];
      fft(buf, buf2, Ny);
      for(j=0; j<Ny;j++)
        arr3[i][j][k] = buf[j];
    }	
  for(j=0; j<Ny;j++)
    for(i=0; i<Nx; i++)
    {
      for(k=0; k<Nz;k++)
        buf[k] = arr3[i][j][k];
      fft(buf, buf2, Nz);
      for(k=0; k<Nz;k++)
        arr3[i][j][k] = buf[k];
    }
}
void ifft3(complex ***arr3, complex *buf, int Nx, int Ny, int Nz)
{	
  int i,j,k;
  int maxN=(Nx>Ny?Nx:Ny);
      maxN=(Nz>maxN?Nz:maxN);
  complex *buf2=buf+maxN;

  for(j=0; j<Ny;j++)
    for(k=0; k<Nz;k++)
    {
      for(i=0; i<Nx; i++)
        buf[i] = arr3[i][j][k];
      ifft(buf, buf2, Nx);
      for(i=0; i<Nx; i++)
        arr3[i][j][k] = buf[i];
    }
  for(i=0; i<Nx; i++)
    for(k=0; k<Nz;k++)
    {
      for(j=0; j<Ny;j++)
        buf[j] = arr3[i][j][k];
      ifft(buf, buf2, Ny);
      for(j=0; j<Ny;j++)
        arr3[i][j][k] = buf[j];
    }	
  for(j=0; j<Ny;j++)
    for(i=0; i<Nx; i++)
    {
      for(k=0; k<Nz;k++)
        buf[k] = arr3[i][j][k];
      ifft(buf, buf2, Nz);
      for(k=0; k<Nz;k++)
        arr3[i][j][k] = buf[k];
    }
}
