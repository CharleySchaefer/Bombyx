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
  int Nbins, Rmax;
  Rmax=(xmax-xmin > ymax-ymin ? (zmax-zmin > xmax-xmin ? zmax-zmin : xmax-xmin) : (zmax-zmin > ymax-ymin ? zmax-zmin : ymax-ymin));
  Nbins=(1+Rmax)/2;
  int x1,y1,z1,x2,y2,z2,ind;
  int *counter, count;
  double R,dR=Rmax/(Nbins-1), *g;
  counter=(int*)malloc(Nbins*sizeof(int));
  g=(double*)malloc(Nbins*sizeof(double));
  complex*gd=(complex*)malloc(2*Nbins*sizeof(complex));
  complex*SF=(complex*)malloc(2*Nbins*sizeof(complex));
  for(i=0; i<Ndata; i++)
  {
    x1=x_col[i];
    y1=y_col[i];
    z1=z_col[i];
    for(j=i; j<Ndata; j++)
    {
      x2=x_col[j];
      y2=y_col[j];
      z2=z_col[j];
      R=sqrt( (x2-x1)*(x2-x1) +(y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );
      ind=(int)((double)R/(dR));
      
      g[ind]++;
    }
  }
  for(i=0; i<Nbins; i++)
  {
    if(i==0)
      g[i]=1;
    else
      g[i]/=(Ndata*i*i)/(4.0/3.0*M_PI);
    gd[i]=g[i]; gd[2*Ndata-i-1]=g[i];
  }

  double* SFd=(double*)malloc(Nbins*sizeof(double));
  for(i=0; i<Nbins; i++) 
    SFd[i]=creal(SF[i]);
  dft(gd, SF, Ndata);
  double SFmin=d1min( SFd, Nbins );
  printf("SFmin: %e %e\n", SFd[Nbins/2], SFmin);
  for(i=0; i<Nbins; i++)
  {
    SF[i] = 1 + SF[i]/(-SF[Nbins-1]);
  }

  for(i=0; i<Nbins; i++)
  {printf(" %12.3e %12.3e %12.3e %12.3e\n",  i*dR, g[i], i*1.0/(dR), creal(SF[i]));}

  free(x_col);
  free(y_col);
  free(z_col);

  return(1);
}
