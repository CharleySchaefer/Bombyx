/*
  
  Developed by https://github.com/lawliet89/DoolittleDeterminant/

  Modified from 25 July 2018 as part of ZiltoidLIB
*/

/**
    Note: The arrays are implemented as Ragged Arrays
*/
#include "DoolittleAlgorithm.h"

// Returns 0 if it fails, returns 1 if success
int DL_LUdecomposition(float **matrix, float **matrix_out, int dimension)
{
  int i, j, k, p, swapCount=0, determinantFactor=1;
  float a, ajj, result, value;
  int swapee = 0;    // used to indicate row to swap
  float **m = matrix_out;

  // Let us copy the matrix first
  for(i=0; i<dimension; i++)
    for(j=0; j<dimension; j++)
      m[i][j]=matrix[i][j];

  // First step: perform LU Decomposition using Doolittle's Method
  // This algorithm will return, in the same matrix, a lower unit triangular matrix
  // (i.e. diagonals one)
  // and an upper trangular matrix
  // https://vismor.com/documents/network_analysis/matrix_algorithms/S4.SS2.php

  for (i = 0; i < dimension; i++)
  {
    for (j = 0; j < i; j++)
    {
      ajj = m[j][j];

      //---------------------------------------------
      // Partial pivot
      // a number we are dividing by is zero.
      // we thus have to do a partial pivot
      if (ajj == 0)
      {
        if (swapCount == dimension)
        {
          // Seems like we have exhaused all swaps
          return 0 ;//NAN;
        }

        // Let's find a swap row to swap
        result = -INFINITY;
        for (k = 0; k < dimension; k++)
        {
          value = m[k][j];
          if (value > result)
          {
            result = value;
            swapee = k;
          }
        }

        // Swap rows
        DL_swapRows(m, j, swapee);              
        swapCount++;
        determinantFactor *= -1;

        ajj = m[j][j];
      }
      // end partial pivot
      //---------------------------------------------

      a = m[i][j];
      for (p = 0; p < j; p++)
      {
        a -= m[i][p]*m[p][j];
      }
      m[i][j]= a/ajj;
    }

    for (j = i; j < dimension; j++)
    {
      a = m[i][j];
      for (p = 0; p < i; p++)
      {
        a -= m[i][p]*m[p][j];
      }
      m[i][j]=a;
    }
  }
  return(1); // success
}

int DL_eigenvalues(float **matrix, float **matrix_buffer, float *eigenvalues, int dimension)
{
  int i;
  // L and U triangular matrix are both stored in matrix_buffer
  // The diagonal represent the eigenvalues
  if(!DL_LUdecomposition(matrix, matrix_buffer, dimension))
  {printf("ERROR: DL_LUdecomposition() failed.\n");return(0);}

  for(i=0; i<dimension; i++)
    eigenvalues[i]=matrix_buffer[i][i];

  return(1);
}

float DL_determinant(float **matrix, float **matrix_buffer, int dimension)
{
  int i,j;
  float det;

  // First step: LU decomposition. 
  // L and U triangular matrix are both stored in matrix_buffer
  // The diagonal represent the eigenvalues
  DL_LUdecomposition(matrix, matrix_buffer, dimension);

  // Determinant is the product of the eigenvalues
  det = 1;
  for (i = 0; i < dimension; i++)
    det *= matrix_buffer[i][i]; // m[i][i] are the eigenvalues of the original matrix
  return det;
}

// swap rows -- ragged array. swap rows a and b
void DL_swapRows(float **matrix, int a, int b){
  float *swapRowTemp = *( matrix + a );
  *( matrix + a ) = *( matrix + b );
  *( matrix + b ) = swapRowTemp;
}

// Generate a n by n matrix consisting of numbers between -1 and 1.
float **DL_randomMatrix(int dimension)
{
  int i, j;
  float **matrix;
  float *row;
  float no;

  // let's first initialise the ragged array
  // initialise Ragged Array
  matrix = (float **) malloc(dimension*sizeof(float *));

  // Seed random number generator
  srand(time(NULL));

  for (i = 0; i < dimension; i++)
  {
    // Initialise this row
    row = (float *) malloc(dimension*sizeof(float));
    *(matrix + i) = row;

    // random no between -1 and 1
    for (j = 0; j < dimension; j++)
    {
      no = ((float) (rand()%20))/10-1;
      *(row + j) = no;
    }
  }
  return matrix;
}

// Outputs in a Matlab format, good for checking in Matlab
void DL_printMatrix(float **matrix, int dimension)
{
  int i, j;

  for (i = 0; i < dimension; i++)
  {
    if(i>0)
      printf(" ");
    else
      printf("[");

    for (j = 0; j < dimension; j++)
    {

      printf("%6.2f ", matrix[i][j]);
    }

    if(i<dimension-1)
      printf(";\n");
    else
      printf("];\n");
  }
}
