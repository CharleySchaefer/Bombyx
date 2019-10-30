#include <unistd.h>

#include "../../ZiltoidLIB.h"



int main(int argc, char *argv[])
{

  //-------------------------------------------------------------
  // DECLARATIONS
  int i,j;
  int Nlines, Ncol,Nheader,  Ndata;
  char *fname;
  int verbose=0;
  int col_number=1;

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
      sprintf(fname, "%s", argv[i+1]);
      if( access( fname, F_OK ) == -1 ) 
      {
        printf("Error: File \'%s\' does not exist!\n", fname); return(0);
      }
      i += 2;
    }else if( strcmp( arg, "--verbose" ) == 0 ){
      verbose=1;
      i++;
    } else if( strcmp( arg, "--column" ) == 0 ){
      col_number=atoi(argv[i+1]);
      i+=2;
    } else{
      printf( "Error parsing args! Arg \"%s\" not recognized!\n", arg );
      return -1;
    }
  }
  // SCRIPT ARGUMENTS READ
  //-------------------------------------------------------------


  //-------------------------------------------------------------
  // CORE
  if(!analyse_data_file_properties(fname, &Nlines, &Ncol, &Nheader, &Ndata, verbose))
  {printf("Error: analyse_data_file_properties() failed!\n"); exit(1);}

  double *col=(double*)malloc(Ndata*sizeof(double));
  dreadColumn(fname, Ndata, Nheader, col_number-1, col); 

  // mean, variance, sem (sem=standard error of the mean = sqrt(var/(Ndata-1)))
  double xmean = d1mean(    col, Ndata); // Mean of array
  double xvar  = d1variance(col, Ndata); // Mean of array

  int Nq1,Nq2,Nq3;
  double median, firstquartile, thirdquartile;

  if(verbose)
    printf("#dmergesort().\n");
  dmergesort(col, Ndata);

  // Median
  Nq2 = Ndata/2;
  if(Ndata%2==1)   //odd
    median = col[Nq2];
  else             // even
    median = 0.5*(col[Nq2-1]+col[Nq2]);

  // First quartile
  Nq1 = Nq2/2;
  if(Nq2%2==1)   //odd
    firstquartile = col[Nq1];
  else             // even
    firstquartile = 0.5*(col[Nq1-1]+col[Nq1]);

  // Third quartile
  Nq3 = Nq2 + (Ndata-Nq2)/2;
  if((Ndata-Nq2)%2==1)   //odd
    thirdquartile = col[Nq3];
  else             // even
    thirdquartile = 0.5*(col[Nq3-1]+col[Nq3]);

  if(verbose)
    printf("#Nq1,2,3 = %d %d %d\n", Nq1, Nq2, Nq3);



  // Box-and-whisker: first quartile, median/second quartile, thirds quartile
  if(verbose)
    printf("#var: variance;\n#std: standard error;\n#sem: standard error of the mean (=sqrt(var/(Ndata-1)))\n");
  printf("%12s %12s %12s %12s %12s %12s %12s %12s %12s %12s\n", "#Ndata", "mean", "var", "std", "sem", "min", "max", "Median", "FirstQ", "ThirdQ");
  printf("%12d %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e\n", Ndata, xmean, xvar, sqrt(xvar), sqrt(xvar/(Ndata-1)), d1min(col,Ndata), d1max(col,Ndata), median, firstquartile, thirdquartile  );




  free(col);
  free(fname);

  return(1);
}
