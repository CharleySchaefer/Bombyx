#include <stdio.h>
#include <stdlib.h>

#include "../../LinearAlgebra.h"

// compile: gcc demo_eigenvalues_real_symmetric_matrix.c -llapack


int main(int argc, char** argv)
{
  int       i,j,k,l,m;
  const int n=4;

  double eig;
  double dbuff1[2*n-1];
  double dbuff2[n-1];
  double dbuff3[n-1];
  double D[n];         // will contain the eigenvalues
  double *U;           // (length n*n). Memory buffer OR eigenvectors
  U=(double*)malloc(n*n*sizeof(double));

  double    **matrix;


  matrix=(double**)malloc(n*sizeof(double*));
  for(i=0;i<n; i++)
    matrix[i]=(double*)malloc(n*sizeof(double));
  matrix[0][0]= 1; matrix[0][1]= 1; matrix[0][2]= 1; matrix[0][3]= 1;
  matrix[1][0]= 1; matrix[1][1]= 1; matrix[1][2]= 1; matrix[1][3]= 1;
  matrix[2][0]= 1; matrix[2][1]= 1; matrix[2][2]= 3; matrix[2][3]= 1;
  matrix[3][0]= 1; matrix[3][1]= 1; matrix[3][2]= 1; matrix[3][3]= 4;

  printf("\nDemo matrix:\n");
  for(i=0;i<n; i++)
  {
    for(j=0;j<n; j++)
      printf("%d ", (int)matrix[i][j]);
    putchar('\n');
  }
  //--------------------------------------------------------------------
  // DEMO 1:  Calculate eigenvalues only (U acts as a memory buffer)
  putchar('\n');
  printf("Demo eigenproblem_real_symmetric_matrix() with compz='N':\n");

  if(eigenproblem_real_symmetric_matrix('N', matrix, n, U, dbuff1, dbuff2, dbuff3, D)!=1)
    { printf("Error: eigenvalues_and_eigenvectors_real_symmetric_matrix() failed.\n"); return(-1);}

  printf("Eigenvalues - NO eigenvectors!\n");

  for(i=0; i<n; i++)
  {
    printf("%8.6f", D[i]);  putchar('\n');
  }
  putchar('\n');


  //--------------------------------------------------------------------
  // DEMO 2: Calculate eigenvalues & eigenvectors (U will contain the eigenvectors)
  printf("Demo eigenproblem_real_symmetric_matrix() with compz='V'.\n");

  if(eigenproblem_real_symmetric_matrix('V', matrix, n, U, dbuff1, dbuff2, dbuff3, D)!=1)
    { printf("Error: eigenvalues_and_eigenvectors_real_symmetric_matrix() failed.\n"); return(-1);}

  printf("Eigenvalues : eigenvectors\n");
  for(i=0; i<n; i++)
  {
    printf("%8.6f    :", D[i]);
    for(j=0; j<n; j++)
      printf(" %9.6f", U[i*n+j]);

    // CHECK: 
    matrix_vector_product(matrix, n, n, U+n*i, dbuff1);  // M.u = eig*u = dbuff1
    vector_inner_product(dbuff1, U+n*i, n,  &eig);       // u.dbuff1 = eig*u.u = eig*1 = eig
    printf(" | check: %8.6f", eig);

    putchar('\n');
  }
  putchar('\n');


  

  return 0;
}
