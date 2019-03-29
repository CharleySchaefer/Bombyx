#include "ReadTextFiles.h"

int analyse_data_file_properties(char *fname, int *Nlines, int *Ncol, int *Nheader, int *Ndata, int verbose)
{
  if(verbose)
    printf("#Analyse data file \'%s\'\n", fname);

  if(verbose)
{
    printf("#Warning: Function expects same number of data points in each column.\n");
}


  if(!countLines(fname, Nlines))
  {printf("Error: countLines() failed!\n");   return(0);}

  if(!getNheader(fname, Nheader))
  {printf("Error: getNheader() failed!\n");   return(0);}

//  if(!countColumns(fname, (*Nheader), Ncol))
//  {printf("Error: countColumns() failed!\n"); return(0);}

  if(!countDataLines(fname, *Nlines, *Nheader, Ncol, Ndata))
  {printf("Error: countColumns() failed!\n"); return(0);}



  if(verbose)
  {
    printf("#Number of lines:                        %d\n", *Nlines);
    printf("#Number of columns:                      %d\n", *Ncol);
    printf("#Number of header lines:                 %d\n", *Nheader);
    printf("#Number of data lines:                   %d\n", *Ndata);
    printf("#Number of white lines at end of file:   %d\n", *Nlines-*Nheader-*Ndata);
  }
  return(1);
}
/*
  countDataLines expects the same number of datapoints in each column

  Nlines and Nheader expected as input:
  Run countLines() and getNheader() first
*/
int countDataLines(char *fname, int Nlines, int Nheader, int *Ncol, int *Ndata)
{
  int i, Nwords;
  char line[MAX_LINE_WIDTH], word[MAX_STR_L];
  FILE *ifp;


  if( (ifp = fopen(fname, "r")) == NULL)
    {printf("\nError: Failed to open file \'%s\'!\n\n", fname); return(0);}

  for(i=0; i<Nheader; i++)
    fgets( line, sizeof line, ifp);

  // First data line
  fgets( line, sizeof line, ifp);
  if(!countWords(line, word, &(*Ncol) ))
    {printf("Error: countWords() failed.\n"); return(0);}

  // Sweep following lines until number of columns does not match
  for((*Ndata)=1; (*Ndata)<Nlines-Nheader; (*Ndata)++)
  {
    fgets( line, sizeof line, ifp);
    if(!countWords(line, word, &Nwords ))
      {printf("Error: countWords() failed.\n"); return(0);}

    if(Nwords!=(*Ncol))
    {
      break;
    }
  }
  fclose(ifp);
  return(1);
}

int getNheader(const char *fname, int *Nheader)
{
  int i, atheader;
  char line[MAX_LINE_WIDTH];
  FILE *ifp;
  char c;

  if( (ifp = fopen(fname, "r")) == NULL)
    {printf("\nError: Failed to open file \'%s\'!\n\n", fname); return(0);}

  *Nheader=-1; atheader=1;
  while( atheader & fgets( line, sizeof line, ifp) != NULL )
  {
    (*Nheader)++;
    i=0;
    while( ((c=line[i]) == ' ' | c == '\t') & c!='\n' & c!='\0') // Skip white space
      i++;
    atheader=!('0'<=c & c<='9');
  }
  fclose(ifp); 

  return(1);
}

int countColumns(char *fname, int atline, int *Ncols)
{
  FILE *ifp;
  int  i, Nchar;
  char line[MAX_LINE_WIDTH], word[MAX_STR_L];

  // Open file
  if( (ifp = fopen(fname, "r")) == NULL)
    {printf("\nError: Failed to open file \"%s\" in countLines()!\n\n", fname); return(0);}

  // Get line (counting starts at 0) from which number of columns is determined
  for(i=0; i<=atline; i++)
    fgets( line, sizeof line, ifp);

  // Read words in the line till end of line has been reached
  if( !countWords(line, word, &(*Ncols)) )
    {printf("\nError: Failed to execute countWords()!\n\n", fname); return(0);}

  // Close file
  fclose(ifp);
  return(1);
}

// Count lines in a file
int countLines(char *fname, int *Nlines)
{
  FILE *ifp;
  char line[MAX_LINE_WIDTH];

  if( (ifp = fopen(fname, "r")) == NULL)
    {printf("\nError: Failed to open file \"%s\" in countLines()!\n\n", fname);return(0);}

  (*Nlines) = 0;
  while( fgets( line, sizeof line, ifp) != NULL)
    (*Nlines)++;
  fclose(ifp);
  return(1);
}

int transpose_plain_data_file(char *fname, double **buffer)
{
  int i,j, Nlines, Ncol;
  FILE *ifp;
    
    countLines(fname, &Nlines);
  countColumns(fname, 0, &Ncol);

  if(!readMatrix(fname, buffer, Nlines, Ncol))
    {printf("Error: readMatrix() failed!\n"); return(0);}

  ifp=fopen(fname, "w");
  for(i=0; i<Ncol; i++)
  {
    for(j=0; j<Nlines; j++)
    {
      fprintf(ifp, "%16e ", buffer[j][i]);
    }
    fprintf(ifp, "\n");
  }
  fclose(ifp);
  return(1);
}



int checkFileName(char *fname)
{
  int  exists,
       Nmax = 3,  // Length of 'str' by which the original string is modified
       N=0;
  char *newfileName,
       *fileNameTmp,
       *tmpc;
  FILE *ifp;

  newfileName=(char*)malloc(MAX_STR_L*sizeof(char));
  fileNameTmp=(char*)malloc(MAX_STR_L*sizeof(char));

  // If file already exists a new file name is tried
  if( (ifp = fopen( fname , "r")) != NULL )
  {  
    fclose(ifp);
    exists=1;
    while(exists==1)
    {
      fileNameTmp[0]='\0';
      strcat(fileNameTmp, fname);
      if(!fileNumber(fileNameTmp, N, Nmax, newfileName))
        {printf("Error: fileNumber() failed!\n"); return(0);}

      if( (ifp = fopen( newfileName , "r")) == NULL ) // File found!
        exists=0;
      N++;
    }
  }
  fname[0]='\0';
  strcat(fname, newfileName);

  return(1);
}



int countFiles(char *fname, int Nmax, int *Nfiles)
{
  int   i,
        check;
  char  c,
        ctmp[MAX_STR_L];
  FILE  *ifp;

  (*Nfiles)=0;
  if( (ifp=fopen(fname, "r")) != 0)
  {
    (*Nfiles)++;
    fclose(ifp);
  }

  check=1;i=0;
  while(check)
  {
    if(!fileNumber(fname, i, Nmax, ctmp))
    {
      printf("Error: fileNumber function not executed!\n");
      return(0);
    }
    if( (ifp=fopen(ctmp, "r")) != NULL) // file exists
    {
      (*Nfiles)++;
      fclose(ifp);
    }
    else  // file does not exist
      check=0;
    i++;
  }
  (*Nfiles)--;
  return(1);
}



int dreadColumn(char *fname, int Ndata, int Nheader, int Ncol, double *col)
{
  int  i,j,countCol;
  char c;
  char line[MAX_LINE_WIDTH];
  FILE *ifp;

  if(Ncol<0)
  {
    printf("WARNING: algorithm starts reading data at Ncol=0!\n");
    return(0);
  }
  if((ifp = fopen(fname, "r")) == NULL){printf("Error: Failed to open \'%s\'!\n", fname); return(0);}

  // Go to first line in file that contains data

  for(i=0; i<Nheader; i++)
    fgets(line, sizeof(line), ifp);


  // Get data from last column
  for(i=Nheader-1; i<Ndata+Nheader-1; i++) // sweep over all lines
  {
    fgets(line, sizeof(line), ifp);

    j=0;countCol=0;// initialise position in line
    // Skip white space before first column

    // Find position of first column in the line
    if((c=line[j])==' ') // Read line until space is found (column delimiter)
    {
      while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
        j++;

      // Read line until end of space
      while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
        j++;
    }

    // start reading columns
    while( countCol<Ncol & (c=line[j]) != '\0' & c != '\n' )
    {
      // Read line until space is found
      while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
        j++;

      // Read line until end of space
      while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
        j++;

      // If character after space is not the end of the line a column is found
      if( (c=line[j]) != ' ' & c != '\0' & c != '\n')
        countCol++;
    }
    col[i+1-Nheader] = atof(line+j); // Read column entry 
  }
  fclose(ifp);
  return(1);
}
int freadColumn(char *fname, int Ndata, int Nheader, int Ncol, float *col)
{
  int  i,j,countCol;
  char c;
  char line[MAX_LINE_WIDTH];
  FILE *ifp;

  if(Ncol<0)
  {
    printf("WARNING: algorithm starts reading data at Ncol=0!\n");
    return(0);
  }
  if((ifp = fopen(fname, "r")) == NULL){printf("Error: Failed to open \'%s\'!\n", fname); return(0);}

  // Go to first line in file that contains data

  for(i=0; i<Nheader; i++)
    fgets(line, sizeof(line), ifp);


  // Get data from last column
  for(i=Nheader-1; i<Ndata+Nheader-1; i++) // sweep over all lines
  {
    fgets(line, sizeof(line), ifp);

    j=0;countCol=0;// initialise position in line
    // Skip white space before first column

    // Find position of first column in the line
    if((c=line[j])==' ') // Read line until space is found (column delimiter)
    {
      while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
        j++;

      // Read line until end of space
      while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
        j++;
    }

    // start reading columns
    while( countCol<Ncol & (c=line[j]) != '\0' & c != '\n' )
    {
      // Read line until space is found
      while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
        j++;

      // Read line until end of space
      while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
        j++;

      // If character after space is not the end of the line a column is found
      if( (c=line[j]) != ' ' & c != '\0' & c != '\n')
        countCol++;
    }
    col[i+1-Nheader] = atof(line+j); // Read column entry 
  }
  fclose(ifp);
  return(1);
}
  
int ireadColumn(
  char  *fname,     // File name         
  int   Ndata,      // Number of datapoints     
  int   Nheader,   // Line number with first data points
  int    Ncol,      // Column number: Starts counting from zero    
  int  *col    // Output array in with column values   
  ) // End of function arguments
  {
    int  i,j,countCol;
    char  c;
    char  line[MAX_LINE_WIDTH];
    FILE  *ifp;

    if(Ncol<0)
    {
      printf("WARNING: algorithm starts reading data at Ncol=0!\n");
      return(0);
    }

    if((ifp = fopen(fname, "r")) == NULL){printf("Error: Failed to open \'%s\' in readColumn!\n", fname); return(0);}

  /* Go to first line with data */  
    for(i=0; i<Nheader; i++)
      fgets(line, sizeof(line), ifp);

  /* Get data from last column */
    for(i=Nheader-1; i<Ndata+Nheader-1; i++){
      fgets(line, sizeof(line), ifp);
      j=0;countCol=0;

      // Skip white space before first column
      if((c=line[j])==' ')
         {/* Read line until space is found */  
        while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
          j++;

        /* Read line until end of space */    
        while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
          j++;}
      
      // start reading columns
      while( countCol<Ncol & (c=line[j]) != '\0' & c != '\n' ){
    
        /* Read line until space is found */  
        while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
          j++;

        /* Read line until end of space */    
        while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
          j++;

        /* If character after space is not the end of the line a column is found */
        if( (c=line[j]) != ' ' & c != '\0' & c != '\n')
          countCol++;
        }
      col[i+1-Nheader] = atoi(line+j);  
      }
    fclose(ifp);
    return(1);
  }
  
int creadColumn(
  char  *fname,     // File name         
  int   Ndata,      // Number of datapoints     
  int   Nheader,   // Line number with first data points
  int    Ncol,      // Column number: Starts counting from zero    
  char  *col    // Output array in with column values   
  ) // End of function arguments
  {
    int  i,j,countCol;
    char  c;
    char  line[MAX_LINE_WIDTH];
    FILE  *ifp;

    if(Ncol<0)
    {
      printf("WARNING: algorithm starts reading data at Ncol=0!\n");
      return(0);
    }

    if((ifp = fopen(fname, "r")) == NULL){printf("Error: Failed to open \'%s\' in readColumn!\n", fname); return(0);}

  /* Go to first line with data */  
    for(i=0; i<Nheader; i++)
      fgets(line, sizeof(line), ifp);

  /* Get data from last column */
    for(i=Nheader-1; i<Ndata+Nheader-1; i++){
      fgets(line, sizeof(line), ifp);
      j=0;countCol=0;

      // Skip white space before first column
      if((c=line[j])==' ')
         {/* Read line until space is found */  
        while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
          j++;

        /* Read line until end of space */    
        while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
          j++;}
      
      // start reading columns
      while( countCol<Ncol & (c=line[j]) != '\0' & c != '\n' ){
    
        /* Read line until space is found */  
        while( (c=line[j]) != '\0' & c != '\n' & c != ' ')
          j++;

        /* Read line until end of space */    
        while( (c=line[j]) == ' ' & c != '\0' & c != '\n')
          j++;

        /* If character after space is not the end of the line a column is found */
        if( (c=line[j]) != ' ' & c != '\0' & c != '\n')
          countCol++;
        }
      col[i+1-Nheader] = line[j];  
      }
    fclose(ifp);
    return(1);
  }

int dreadRow(char *fname, int Nrow, double *row)
{
  int    i,j,ind, Nchar;
  char   word[MAX_STR_L], line[MAX_LINE_WIDTH];
  FILE   *ifp;
  
  if( (ifp = fopen(fname, "r")) == NULL){
    printf("\nError: Failed to open file \"%s\" in countLines!\n\n", fname);
    return(0);
    }
  // get to line at Nrow (Nrow=0) is the first line
  i=0;
  while( fgets( line, sizeof line, ifp) != NULL & i<Nrow)
    i++;
  
  ind=0;i=0;
  while( getWord( line+ind, word, &Nchar) )
  {
    ind+= Nchar;
    row[i] = atof(word);
    i++; // Count number of words
  }
  
  fclose(ifp);
  return(1);
}

int sreadRow(char *fname, int Nrow, float *row, int *Nword)
{
  int    i,j,ind, Nchar;
  char   word[MAX_STR_L], line[MAX_LINE_WIDTH];
  FILE   *ifp;
  
  if( (ifp = fopen(fname, "r")) == NULL){
    printf("\nError: Failed to open file \"%s\" in countLines!\n\n", fname);
    return(0);
    }
  // get to line at Nrow (Nrow=0) is the first line
  i=0;
  while( fgets( line, sizeof line, ifp) != NULL & i<Nrow)
    i++;
  
  ind=0;(*Nword)=0;
  while( getWord( line+ind, word, &Nchar) )
  {
    ind+= Nchar;
    row[(*Nword)] = (float)atof(word);
    (*Nword)++; // Count number of words
  }
  
  fclose(ifp);
  return(1);
}

int ireadRow(char *fname, int Nrow, int *row, int *Nword)
{
  int    i,j,ind, Nchar;
  char   word[MAX_STR_L], line[MAX_LINE_WIDTH];
  FILE   *ifp;
  
  if( (ifp = fopen(fname, "r")) == NULL){
    printf("\nError: Failed to open file \"%s\" in countLines!\n\n", fname);
    return(0);
    }
  // get to line at Nrow (Nrow=0) is the first line
  i=0;
  while( fgets( line, sizeof line, ifp) != NULL & i<Nrow)
    i++;
  
  ind=0;(*Nword)=0;
  while( getWord( line+ind, word, &Nchar) )
  {
    ind+= Nchar;
    row[(*Nword)] = atoi(word);
    (*Nword)++; // Count number of words
  }
  
  fclose(ifp);
  return(1);
}


int readLine(char *fname, char *line, int Nline)
{
  int i;
  FILE *ifp;
  if(NULL==(ifp = fopen(fname, "r")))
    {printf("Error: Failed to read file \'%s\'!\n", fname); return(0);}
  for(i=0; i<Nline; i++)
    fgets(line, MAX_LINE_WIDTH*sizeof(char), ifp);
  fclose(ifp);
  return(1);
}


int readMatrix(char *fname, double **mat, int Nlines, int Ncols)
{
  int    i,j,ind, Nchar;
  char   c, word[MAX_STR_L], line[MAX_LINE_WIDTH];
  FILE   *ifp;
  
  if( (ifp = fopen(fname, "r")) == NULL){
    printf("\nError: Failed to open file \"%s\" in countLines!\n\n", fname);
    return(0);
    }
  i=0;
  while( fgets( line, sizeof line, ifp) != NULL)
  {
    ind=0;j=0;
    // Skip white space before first column
    while( (c=line[ind]) == ' ' & c != '\0' & c != '\n')
      ind++;
        
    while( getWord( line+ind, word, &Nchar) )
    {
      ind+= Nchar;
      mat[i][j] = atof(word);
      j++; // Count number of words
    }
    i++;
  }
  fclose(ifp);
  return(1);
}

int readMatrix_short(char *fname, short **mat, int Nlines, int Ncols)
{
  int    i,j,ind, Nchar;
  char   word[MAX_STR_L], line[MAX_LINE_WIDTH];
  FILE   *ifp;
  
  if( (ifp = fopen(fname, "r")) == NULL){
    printf("\nError: Failed to open file \"%s\" in countLines!\n\n", fname);
    return(0);
    }
  i=0;
  while( fgets( line, sizeof line, ifp) != NULL)
  {
    ind=0;j=0;
    while( getWord( line+ind, word, &Nchar) )
    {
      ind+= Nchar;
      mat[i][j] = atof(word);
      j++; // Count number of words
    }
    i++;
  }
  fclose(ifp);
  return(1);
}

int readMatrix_int(char *fname, int **mat, int Nlines, int Ncols)
{
  int    i,j,ind, Nchar;
  char   word[MAX_STR_L], line[MAX_LINE_WIDTH];
  FILE   *ifp;
  
  if( (ifp = fopen(fname, "r")) == NULL){
    printf("\nError: Failed to open file \"%s\" in countLines!\n\n", fname);
    return(0);
    }
  i=0;
  while( fgets( line, sizeof line, ifp) != NULL)
  {
    ind=0;j=0;
    while( getWord( line+ind, word, &Nchar) )
    {
      ind+= Nchar;
      mat[i][j] = atoi(word);
      j++; // Count number of words
    }
    i++;
  }
  fclose(ifp);
  return(1);
}

// Column counter starts at zero
// cbuff1: array of length max_line_width
int count_occurences_of_word_in_file_column(char *fname, char *word, int col, char *cbuff1, char *cbuff2, int max_line_width)
{
  FILE *ifp;
  int Nchar;
  int count=0, offset=0, col_curr; // initial value
  char *line=cbuff1;
  char *str =cbuff2;
  if(col<0)
    {printf("Error: \"col\" should be a positive integer.\n"); return(-1);}

  if( (ifp = fopen(fname, "r")) == NULL)
    {printf("\nError: fopen() failed.\n", fname);return(-1);}

  while( fgets( line, max_line_width*sizeof(char), ifp) != NULL)
  {
    offset=0;
    if( getWord( line+offset, str, &Nchar) )
    {
      offset+=Nchar;
      col_curr=0; 
      while( (col_curr<col) )
      {
        getWord( line+offset, str, &Nchar);
        col_curr++;
        offset+=Nchar;
      }

      if(  !strcmp(str, word) )
        count++;
    }
  }
  fclose(ifp);

  return count; // on success 
}
