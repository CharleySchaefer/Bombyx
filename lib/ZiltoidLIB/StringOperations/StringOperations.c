#include "StringOperations.h"

//==============================================================================================
//
//    A. NUMBER TO STRING CONVERSION								
//
//==============================================================================================


//----------------------------------------------------------------------------------------------
//  Create string str such as "0781" from integer N=781 and Nmax=4
//				
//  str: string with length Nmax (memory: length str has to be at least 4)
//  N:   integer number									
void numStr(char* str, int Nmax, int N)
{
  sprintf(str, "%%0%dd", Nmax);
  sprintf(str, str, N);
}

//----------------------------------------------------------------------------------------------
//  Increase number in a string such as "0781" from "0780"
//
//  str   - initial string "0780" which is returned as "0781" 				
//  Nmax  - size of string									
//  num   - Number added to the string.
int countStr(char* str, int num)
{
  int Nmax=strlen(str);
  int N=atoi(str)+num;
  sprintf(str, "%%0%dd", Nmax);
  sprintf(str, str, N);
  
  if(N<0)
    {printf("ERROR: countStr() does not support negative numbers!\n"); return(0);}
  if(N<pow(10,Nmax))
    numStr(str, Nmax, N);
  else
    {printf("ERROR: Length of number exceeds maximum string length!\n"); return(0);}

  return(1);
}  

//---------------------------------------------------------------------
//
//  getWord (alternative to strok

//  Get word from string.
//  In this function a word is defined as the string of characters, 
//  which contains no tabs, new lines or white spaces.
//  Nchar is the number of characters from the start of the string
//  until the end of the string, so includes possible whitespace
//  before the word.
//
int getWord(char *string, char *word, int *Nchar)
{
  int	i=0,j=0;
  char	c;

  while( ((c=string[i]) == ' ' | c == '\t') & c!='\n' & c!='\0') // Skip white space
    i++;

  if( c == '\n' | c == '\0' ) // No word found
    return(0);
	
  // Get word from string
  word[j] = string[i+j]; 
  while( (c=string[i+j]) != ' ' & c!='\t' & c!='\n' & c!='\0')
  {
    word[j] = string[i + j];
    j++;
  }
  word[j] = '\0'; // End of word

  *Nchar = i+j+1; // Number of characters used including whitespace +1
	
  return(1);
}

//------------------------------------------------------------------------------------------
//
//  fileNumber
//
//  fileName is modified with a number.
//  e.g. file.txt becomes file087.txt
//
//  - fileName is the original filename
//  - Num is the number by which the name is modified (87)
//  - Nmax is the number of characters used (3)
//  - newName is the new filename
//
int fileNumber(char *fileName, int Num, int Nmax, char *newName)
{

  int  i,
       ext_check; // Check for file name extensions (gif, txt)
  char c,
       *str, 
       *token;

  if( !(str = (char*)malloc((Nmax>4?Nmax:4)*sizeof(char)) ))
  {
    printf("ERROR: Allocation of str in fileNumber function failed!\n"); 
    return(0);
  }

  token = strsep(&fileName, ".");
  newName[0]='\0';
  newName=strcat(newName, token);
  numStr(str, Nmax,Num);
  newName=strcat(newName, str);
  newName=strcat(newName, ".");
  token = strsep(&fileName, ".");
  newName=strcat(newName, token);

  return(1);
}

//------------------------------------------------------------------------------------
//
//  countWords
//
//  Count number of words in a line.
//
int countWords(char *line, char *buffer, int *Nwords)
{
  int i, Nchar;
  char *word;

  word = buffer;
  (*Nwords)=0; i=0;
  while( getWord( line+i, word, &Nchar) )
  {
    i+= Nchar;
    (*Nwords)++; // Count number of words
  }

  return(1);
}

int str_2_dir_and_file(const char *str, char *dir, char *file)
{
  int i,ind1,ind2;
  char c;
//  dir =(char*)malloc(MAX_STR_L*sizeof(char));
//  file=(char*)malloc(MAX_STR_L*sizeof(char));

  ind1=0;ind2=-1;
  
  while( (c=str[ind1]) != '\0' & (( c=='.' )|(c=='/')|(c=='\\' )) )
    ind1++;

  i=ind1; 
  while( (c=str[i]) != '\0' )
  {
    i++;
    if(c=='/' | c=='\\')
    {   
      if(ind2!=-1)
        ind1=ind2;
      ind2=i;
    }   
  }

  if(ind2==-1)
    ind2=i+1;
  else
  {
    file[0]='\0';
    strcat(file, str+ind2);
  }
  for(i=ind1; i<ind2-1;i++)
    dir[i-ind1]= *(str+i);

  return(1);
}


