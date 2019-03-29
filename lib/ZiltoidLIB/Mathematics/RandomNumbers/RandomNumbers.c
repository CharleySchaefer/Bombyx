/*
  CS - RANDOM NUMBERS LIBRARY
*/
#include "RandomNumbers.h"

// Generate random number in interval [0, 1, ..., max-1, max]
int  irand(int max)
{
  return rand()%(max+1);
}
double drand(double max)
{
  return (max*rand()/(RAND_MAX+1.0));
}

// Generate array of random numbers in interval [0, 1, ..., max-1, max]
void i1rand(int *arr, int N, int max)
{
  int i;
  for(i=0; i<N; i++)
    arr[i]=irand(max);
}
void d1rand(double *arr, int N, double max)
{
  int i;
  for(i=0; i<N; i++)
    arr[i]=drand(max);
}


// Get random permutation of some input array of integers
void randperm(int *arr, int N)
{
  int i, j;
  int itmp;

  for (i=0; i<N; i++)     // loop elements i
  {
    j = rand()%(N-i) + i; // pick random element j

    // swap arr[i] with arr[j]
    itmp   = arr[j]; 
    arr[j] = arr[i]; 
    arr[i] = itmp;
  }
}

// Generate random number in interval [0, 1, ..., max-1, max]
int randint(int max) 
{ 
  return rand()%(max+1);
}

// Generate random number in interval [min, min+1, ..., max-1, max]
int randint2(int min, int max) 
{ 
  return rand()%(max+1-min)+min;
}

double randf(double max)
{
  return (max*rand()/(RAND_MAX+1.0));
}

double randf2(double min, double max)
{
  return (  min + (max-min)*rand()/(RAND_MAX+1.0)  );
}

// normal distribution, centered on 0, std dev 1
double randn(void)
{
  double drand1, drand2;
  drand1 = (double)rand()/(double)RAND_MAX; // Random number uniform distribution, (0..1]
  drand2 = (double)rand()/(double)RAND_MAX; // Random number uniform distribution, (0..1]
  return sqrt(-2*log(1.0-drand1)) * cos(2*M_PI*drand2);
}

/*
  Get random deviation from parameter x_0 within interval [x_L0, x_U0]
*/
double rand_dev_double(double x_0, double x_L0, double x_U0, double dev)
{
  double x_L, x_U,  x_R, dx, tmpf;

  x_L=x_0*(1.0-dev);
  x_U=x_0*(1.0+dev);
  if( (x_L<x_L0) | (x_U0<x_U) ) // Too large deviation -> make smaller!
  {
    x_R = x_U0-x_L0; // range
    dx=(x_0-x_L0);
    dx=(dx<(tmpf=x_R-dx) ? dx : tmpf);
    x_L=x_0-dx;
    x_U=x_0+dx;
  }


         
  return randf2(x_L, x_U); // return random number from interval [x_L, x_U] 
}


/*-----------------------------------------------------------
  Create Array of length N with randomly distributed integers
  Nind is the number of different integers (0,1,2,..)
  indices is an array that gives the number of times each
  integer is placed in the array arr.

  Example: If Narr    = 100,
              Nind    = 3,
              indices = [20, 10, 5],
           then an array with 20 ones, 10 twos, 5 threes
           and 100-20-10-5=65 zeroes is generated.
*/
int getRandConfig( int *arr, int Narr, int *indices, int Nind )
{
  int i,j, check, coor, count=0, ind=1;
  int Ntot=0;

  // initialise
  for (i=0; i<Narr; i++)
    arr[i]=0;

  // Total number of occupied
  for (i=0; i<Nind; i++)
    Ntot += indices[i];
while(count==indices[ind-1])
    {
      ind++;
//      count=0;
    }

  for (i=0;i<Ntot;i++)
  {
    check =1;
    while (check>0)
    {	
      // select random point
      coor = (rand()%Narr); // random element in arr  
      if (arr[coor]==0)   
        check=0;
    }
    arr[coor]=ind;
    count++;
    
    while(count==indices[ind-1])
    {
      ind++;
      count=0;
    }
  }
  return(1);
}



//-------------------------------------------------------------------------------------
/*
  getNoiseField2D
	
  Get 2D array with uncorrelated noise.
  The noise is uniform and has mean value zero and a variance given by the user
*/
void getNoiseField2Ddouble(int N, double VAR, double **field)
{
  int i,j;
  double tmpf;

  tmpf = 0.0; // Average
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
    {
      field[i][j]  = (double)rand();
      tmpf += field[i][j];
    }
  tmpf /= N*N; 
	
  // Set average to zero  
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      field[i][j]  -= tmpf;
			
  // Calculate variance
  tmpf = 0.0; // Variance
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      tmpf += field[i][j]*field[i][j];
  tmpf /= N; 

  // Normalize random numbers to new variance
  tmpf = sqrt(VAR/tmpf);
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      field[i][j]  *= tmpf;
}

//-------------------------------------------------------------------------------------
/*
  getNoiseField2D

  Get 2D array with uncorrelated noise.
  The noise is uniform and has mean value zero and a variance given by the user
*/
void getNoiseField2Dfloat(int N, double VAR, float **field)
{
  int i,j;
  float tmpf;

  tmpf = 0.0; // Average
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
    {
      field[i][j]  = (float)rand();
      tmpf += field[i][j];
    }
  tmpf /= N*N; 
	
  // Set average to zero  
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      field[i][j]  -= tmpf;

  // Calculate variance
  tmpf = 0.0; // Variance
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      tmpf += field[i][j]*field[i][j];
  tmpf /= N; 
	
  // Normalize random numbers to new variance
  tmpf = sqrt(VAR/tmpf);
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      field[i][j] *= tmpf;
}		
