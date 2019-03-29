/*
  
  Developed by https://github.com/lawliet89/DoolittleDeterminant/

  Modified from 25 July 2018 as part of ZiltoidLIB
*/

/**
    Note: The arrays are implemented as Ragged Arrays
*/


#include "../DoolittleAlgorithm.h"

int main(int argc, char* argv[])
{
  int i,j,MatrixSize=3; // Size of square matrix is MatrixSize x MatrixSize 
  float **matrix;    // matrix is a pointer to pointer
  float **matrixLU;  // matrix is a pointer to pointer
  float *eig;       // matrix is a pointer to pointer
  float det;

  eig=(float *)malloc(MatrixSize*sizeof(float));
  matrixLU=(float **)malloc(MatrixSize*sizeof(float*));
  for(i=0; i<MatrixSize; i++)
  {
    matrixLU[i]=(float *)malloc(MatrixSize*sizeof(float));
  }
    
  // Generate a matrix of size MatrixSize with random values in the interval [-1, 1]
  matrix = DL_randomMatrix(MatrixSize);
  DL_printMatrix(matrix, MatrixSize);
  
  DL_LUdecomposition(matrix, matrixLU, MatrixSize);


  
  DL_eigenvalues(matrix, matrixLU, eig, MatrixSize);
  det = DL_determinant(matrix, matrixLU, MatrixSize);



  det = DL_determinant(matrix, matrixLU, MatrixSize);
    
  printf("Eigenvalues:"); 
  for(i=0; i<MatrixSize; i++)
    printf(" %f", eig[i]);
  putchar('\n'); 
  printf("Determinant: %f \n", det);   
  return 0;
}
