/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/

#include "ReadBBXdata.h"

INPUT *make_bbx_input(void)
{
  INPUT *Param=(INPUT *)malloc(sizeof(INPUT));
  Param->N=-1; 
  Param->v=1.300000e-28;     // Default volume of a mononmer [m^-3]
  Param->phi =   1.0;        // Default volume fraction [-]
  Param->T   = 298.0;        // Default temperature in [K]
  Param->FrequencyUnits = 1; // 0: Angular [rad/s]; 1: Hertz [1/s]

  Param->FirstDataLine = -1; 
  Param->LastDataLine = -1; 
  Param->Ndata = -1; 
  Param->w1    = NULL; 
  Param->Gelas = NULL; 
  Param->w2    = NULL; 
  Param->Gvisc = NULL; 

  return Param;
}

int free_bbx_input(INPUT *Param)
{
  if(Param!=NULL)
  {
    if(Param->w1 != NULL)
    {free(Param->w1); Param->w1=NULL;}
    if(Param->Gelas != NULL)
    {free(Param->Gelas); Param->Gelas=NULL;}
    if(Param->w2 != NULL)
    {free(Param->w2); Param->w2=NULL;}
    if(Param->Gvisc != NULL)
    {free(Param->Gvisc); Param->Gvisc=NULL;}

    free(Param);
    Param=NULL;
  }
}

int readinput(char *inputfile, INPUT **Set_p, int verbose)
{
  int  i,j,Nlines,Ndata;
  char word[MAX_LINE_WIDTH],
       word2[MAX_LINE_WIDTH],
       line[MAX_LINE_WIDTH];
  double tmp;
  FILE	*ifp;

  *Set_p = make_bbx_input();
  INPUT *Set=(*Set_p);  

  //---------------------------------------------------------
  // Read file properties
  if(verbose)
    printf("Reading %s\n", inputfile);
  if( (ifp = fopen(inputfile, "r")) == NULL)
    {printf("Error: Failed to open file \"%s\" in readSystemFile!\n", inputfile); return(0);}

  Nlines=0;
  while( fgets(line, sizeof(line), ifp) != NULL ) // Scan all lines
  {
    if(getWord(line, word, &i)) // scan words in line
    {
      if (     strcmp(word, "#DegreeOfPolymerisation:")==0)
      {Set->N              = atoi(line+i);}
      else if (strcmp(word, "#MonomerVolume:")==0)
      {Set->v              = atof(line+i);}
      else if (strcmp(word, "#VolumeFraction:")==0)
      {Set->phi            = atof(line+i);} 
      else if (strcmp(word, "#Temperature:")==0)
      {Set->T              = atof(line+i);}
      else if (strcmp(word, "#Frequency:")==0)
      {Set->FrequencyUnits = atoi(line+i);}
      else if (strcmp(word, "#FirstDataLine:")==0)
      {Set->FirstDataLine  = atoi(line+i);}
      else if (strcmp(word, "#LastDataLine:")==0)
      {Set->LastDataLine  = atoi(line+i);}
    } // all words in line scanned
    Nlines++;
  } // all lines scanned
  fclose(ifp);

  Set->Ndata=Set->LastDataLine-Set->FirstDataLine+1;

  if(verbose)
  {
    printf("File properties.\n");
    printf("%15s: %d\n", "N", Set->N);
    printf("%15s: %e\n", "v", Set->v);
    printf("%15s: %f\n", "phi", Set->phi);
    printf("%15s: %f\n", "T", Set->T);
    printf("%15s: %-15d %s\n", "FrequencyUnits", Set->FrequencyUnits, "# 0: Angular [rad/s]; 1: Hertz [1/s]");
    printf("%15s: %d\n", "FirstDataLine", Set->FirstDataLine);
    printf("%15s: %d\n", "LastDataLine", Set->LastDataLine);
    printf("%15s: %d\n", "Ndata", Set->Ndata);
    printf("%15s: %d\n", "Nlines", Nlines);
    putchar('\n');
    printf("Reading data from file between lines %d and %d\n", Set->FirstDataLine, Set->LastDataLine);
    printf("Allocating memory.\n");
  }
  // Finished reading file properties 
  //---------------------------------------------------------


  //---------------------------------------------------------
  // Reading file column data
  Set->w1=(double*)malloc(Set->Ndata*sizeof(double));
  Set->Gelas=(double*)malloc(Set->Ndata*sizeof(double));
  Set->w2=(double*)malloc(Set->Ndata*sizeof(double));
  Set->Gvisc=(double*)malloc(Set->Ndata*sizeof(double));

  // TODO: HEADER RECOGNITION
  if(verbose)
  {
    printf("Finished allocating memory.\n");
    printf("Reading column data\n");
  }
  if(!dreadColumn(inputfile, Set->Ndata, Set->FirstDataLine-1, 0, Set->w1   ))
  {printf("Error: Failed to read column 1 using dreadColumn()\n"); return(0);}
  if(!dreadColumn(inputfile, Set->Ndata, Set->FirstDataLine-1, 1, Set->Gvisc))
  {printf("Error: Failed to read column 2 using dreadColumn()\n"); return(0);}
  if(!dreadColumn(inputfile, Set->Ndata, Set->FirstDataLine-1, 2, Set->w2   ))
  {printf("Error: Failed to read column 3 using dreadColumn()\n"); return(0);}
  if(!dreadColumn(inputfile, Set->Ndata, Set->FirstDataLine-1, 3, Set->Gelas))
  {printf("Error: Failed to read column 4 using dreadColumn()\n"); return(0);}

  Ndata=Set->Ndata;
  if(Set->FrequencyUnits) // Units in Hertz
  {
    if(verbose)
      printf("Input frequency in Hertz: convert to rad/s\n");
    for(i=0; i<Set->Ndata; i++)
    {Set->w1[i]*=(2*M_PI); Set->w2[i]*=(2*M_PI);}
  }

  // TODO: robust sorting algorithm
  if(verbose)
    printf("Sorting data in order of frequency (currently flipping data; TODO: mergesort()); Result:\n");
  if(Set->w1[0]>Set->w1[Set->Ndata-1])
  {
    for(i=0; i<(double)Ndata/2; i++)
    {
      tmp=Set->w1[i];    Set->w1[i]   =Set->w1[Ndata-1-i];    Set->w1[Ndata-1-i]   =tmp;
      tmp=Set->w2[i];    Set->w2[i]   =Set->w2[Ndata-1-i];    Set->w2[Ndata-1-i]   =tmp;
      tmp=Set->Gvisc[i]; Set->Gvisc[i]=Set->Gvisc[Ndata-1-i]; Set->Gvisc[Ndata-1-i]=tmp; 
      tmp=Set->Gelas[i]; Set->Gelas[i]=Set->Gelas[Ndata-1-i]; Set->Gelas[Ndata-1-i]=tmp;
    }
  }

  if(verbose)
  {
    printf("%15s %15s %15s %15s\n", "#w[rad/s]", "Gvisc[Pa]", "w[rad/s]", "Gelas[Pa]");
    printf("%15s %15s %15s %15s\n", "------------", "------------", "------------", "------------");
    for(i=0; i<Set->Ndata; i++)
      printf("%15e %15e %15e %15e\n", Set->w1[i], Set->Gvisc[i], Set->w2[i], Set->Gelas[i]); 
  }
  // Finished reading file column data
  //---------------------------------------------------------
  return(1);
}
