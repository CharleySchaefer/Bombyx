//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h> // check if file exists
//#include <string.h>

//#define MAX_LINE_WIDTH   100000

#include "../../ZiltoidLIB.h"



int main(int argc, char *argv[])
{

  //-------------------------------------------------------------
  // DECLARATIONS
  int i;
  int Nlines, Ncol,Nheader,  Ndata;
  char *fname;

  fname=(char*)malloc(MAX_LINE_WIDTH*sizeof(char));
  fname[0]='\0';
  strcat(fname, "DemoData/demodata.txt");  // default input file

  //line=(char*)malloc(MAX_LINE_WIDTH*sizeof(char));
  // DECLARATIONS DONE
  //-------------------------------------------------------------


  //-------------------------------------------------------------
  // READING SCRIPT ARGUMENTS
  i=1;
  while( i < argc ){
    const char *arg = argv[i];
   
    if( strcmp( arg, "--file" ) == 0 ){
      fname = argv[i+1];
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

  // END CORE
  //-------------------------------------------------------------



  return(1);
}
