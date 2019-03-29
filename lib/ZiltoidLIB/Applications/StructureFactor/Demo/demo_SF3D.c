//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h> // check if file exists
//#include <string.h>

//#define MAX_LINE_WIDTH   100000

//#include "../../ZiltoidLIB.h"

#include "../StructureFactor.h"
#include "../../ReadTextFiles/ReadTextFiles.h"



int PDBREAD_get_atom_rows(char *fname, char *cbuff1, char *cbuff2, int max_line_width,  int *atomrows )
{
  int i;
  FILE *ifp;
  int Nchar, Nwords, Nlines;
  char *line=cbuff1; 
  char *word=cbuff2; 

  if( (ifp = fopen(fname, "r")) == NULL)
    {printf("\nError: fopen() failed.\n", fname);return(-1);}

  i=0;
  while( fgets( line, max_line_width*sizeof(char), ifp) != NULL)
  {
    if( getWord( line, word, &Nchar) )
    {
      if(!strcmp(word, "ATOM"))
      {
        atomrows[i]=1;
      }
      else
        atomrows[i]=0;
    }
    i++;
  }
  fclose(ifp);

  return(1);
}

int main(int argc, char *argv[])
{

  //-------------------------------------------------------------
  // DECLARATIONS
  int i,j;
  int Nlines, Ncol,Nheader,  Ndata;
  char *fname;

  int *x_col,*y_col,*z_col;

  fname=(char*)malloc(MAX_LINE_WIDTH*sizeof(char));
  sprintf(fname, "DemoData/demodata.pdb");  // default input file

  //line=(char*)malloc(MAX_LINE_WIDTH*sizeof(char));
  // DECLARATIONS DONE
  //-------------------------------------------------------------


  //-------------------------------------------------------------
  // READING SCRIPT ARGUMENTS
  i=1;
  while( i < argc ){
    const char *arg = argv[i];
   
    if( strcmp( arg, "--file" ) == 0 ){
      sprintf(fname, argv[i+1]);
      if( access( fname, F_OK ) == -1 ) 
      {
        printf("ERROR: File \'%s\' does not exist!\n"); return(0);
      }
      i += 2;
    }else{
      printf( "Error parsing args! Arg \"%s\" not recognized!\n", arg );
      return -1;
    }
  }
  // SCRIPT ARGUMENTS READ
  //-------------------------------------------------------------


  //-------------------------------------------------------------
  // CORE
  if(!analyse_data_file_properties(fname, &Nlines, &Ncol, &Nheader, &Ndata, 1))
  {printf("ERROR: analyse_data_file_properties() failed!\n"); exit(1);}

  // READPDB
  int *atomrows=(int*)malloc(Nlines*sizeof(int));
  char *cbuff1=(char*)malloc(1000*sizeof(char));
  char *cbuff2=(char*)malloc(1000*sizeof(char));

  // Find rows that start with "ATOM"
  PDBREAD_get_atom_rows(fname, cbuff1, cbuff2, 1000, atomrows );
  Nheader=0;

  // Assume PDB file has a header followed by consecutive lines starting with "ATOM"
  Nheader=0;
  while(atomrows[Nheader]==0)
    Nheader++;
  Ndata=0;
  while(atomrows[Nheader+Ndata]==1)
    Ndata++;

//  printf("Nheader: %d\n", Nheader);
//  printf("Ndata:   %d\n", Ndata);
  free(cbuff1);
  free(cbuff2);
  free(atomrows);

  double xmean, ymean, zmean, xvar, yvar, zvar;
  x_col=(int*)malloc(Ndata*sizeof(int));
  y_col=(int*)malloc(Ndata*sizeof(int));
  z_col=(int*)malloc(Ndata*sizeof(int));
  
  ireadColumn(fname, Ndata, Nheader, 4, x_col); 
  ireadColumn(fname, Ndata, Nheader, 5, y_col); 
  ireadColumn(fname, Ndata, Nheader, 6, z_col); 
  free(fname);
  int xmin,xmax,ymin,ymax,zmin,zmax;

  printf("#xrange: %d, %d\n", xmin=i1min(x_col, Ndata), xmax=i1max(x_col, Ndata));
  printf("#yrange: %d, %d\n", ymin=i1min(y_col, Ndata), ymax=i1max(y_col, Ndata));
  printf("#zrange: %d, %d\n", zmin=i1min(z_col, Ndata), zmax=i1max(z_col, Ndata));

  //---------------------------------------------
  // MAKE GRID
  int Ngrid, delta;
  delta=(xmax-xmin > ymax-ymin ? (zmax-zmin > xmax-xmin ? zmax-zmin : xmax-xmin) : (zmax-zmin > ymax-ymin ? zmax-zmin : ymax-ymin));
  printf("#delta: %d\n", delta);
  Ngrid=1;
  while(Ngrid<delta)
    Ngrid*=2;
  Ngrid*=2;
  printf("#Ngrid: %d\n", Ngrid);

  // Get density matrix
  double*** Psi3D=(double***)malloc(Ngrid*sizeof(double**));
  for(i=0; i<Ngrid; i++)
  {
    Psi3D[i]=(double**)malloc(Ngrid*sizeof(double*));
    for(j=0; j<Ngrid; j++)
      Psi3D[i][j]=(double*)calloc(Ngrid, sizeof(double));
  }
  for(i=0; i<Ndata; i++)
  {
    Psi3D[ x_col[i]-xmin ][ y_col[i]-ymin ][ z_col[i]-zmin ] = 1;
  }
  
  //---------------------------------------------
  double  dx = 1.0;

  int Nbins  =(int)( (double)(Ngrid)/sqrt(2) ) - 1;
  int*counter=(    int*)malloc(          Nbins*sizeof(int    ));
  double*q_arr  =( double*)malloc(          Nbins*sizeof(double ));
  double*SF_arr =( double*)malloc(          Nbins*sizeof(double ));
  complex* buff1D =(complex*)malloc(2*(Ngrid)*sizeof(complex));
  complex*** Psi3D_FT=(complex***)malloc(Ngrid*sizeof(complex**));
  for(i=0; i<Ngrid; i++)
  {
    Psi3D_FT[i]=(complex**)malloc(Ngrid*sizeof(complex*));
    for(j=0; j<Ngrid; j++)
      Psi3D_FT[i][j]=(complex*)malloc(Ngrid*sizeof(complex));
  }
  calculateStructureFactor3D(Ngrid, Ngrid, Ngrid, dx, Psi3D, Psi3D_FT, Nbins, buff1D, q_arr, SF_arr);


  // Calculate correlation function C(r)
  double*R_arr  =( double*)malloc(         (2*Nbins-1)*sizeof(double ));
  complex* C_arr  =(complex*)malloc(         (2*Nbins-1)*sizeof(complex ));
  //free(buff1D);
  //complex* buff1D  =(complex*)malloc(         (2*Nbins-1)*sizeof(complex ));

  for (i=0; i<Nbins; i++)
    C_arr[i]=SF_arr[i];
  for (i=Nbins; i<(2*Nbins-1); i++)
    C_arr[i]=SF_arr[2*Nbins-1-i];

  dft(C_arr, buff1D, 2*Nbins-1  ); // Discrete Fourier transform (should actually be the inverse FT

  double sumf=0.0;
  for (i=0; i<2*Nbins-1; i++)
    sumf+=creal( C_arr[i] );

  dx=2.0*M_PI/q_arr[Nbins-1];

  printf("%16s %16s %16s %16s\n", "#q", "S(q)", "R", "C(R)");
  for (i=0; i<Nbins; i++)
  {
    printf("%16e %16e %16e %16e\n", q_arr[i], SF_arr[i]/(Ngrid*Ngrid*Ngrid), dx*i, creal(buff1D[i])/sumf );
  }

  free(x_col);
  free(y_col);
  free(z_col);

  return(1);
}
