#include "stdio.h"
#include "stdlib.h"
#include "../Mathematics.h"

int main(int argc, char *argv[])
{
  int i,j;
  int M=3, N=3;
  double **A, *v, *buff1, *buff2, *q, *r, *out;
  int    max_iter=10000;
  double tolerance=1e-6;

  A=(double**)malloc(M*sizeof(double*));
  for(i=0; i<M; i++)
    A[i]=(double*)malloc(N*sizeof(double));

  v=(double*)malloc(M*N*sizeof(double));
  buff1=(double*)malloc(M*N*sizeof(double));
  buff2=(double*)malloc(M*N*sizeof(double));
  q=(double*)malloc(M*N*sizeof(double));
  r=(double*)malloc(M*N*sizeof(double));
  out=(double*)malloc(M*sizeof(double));

  A[0][0]=1; A[0][1]=2; A[0][2]=1; 
  A[1][0]=2; A[1][1]=2; A[1][2]=3; 
  A[2][0]=2; A[2][1]=4; A[2][2]=2; 

  printf("A:\n");
  for(i=0; i<M; i++)
  {
    for(j=0; j<N; j++)
      printf("%2f ", A[i][j]);
    putchar('\n');
  } 
  putchar('\n');
/*
  printf("GRAM-SCHMIDT METHOD:\n");
  if( !gram_schmidt(a, M, N, buff1, q, r) )
    {printf("ERROR: gram_schmidt() failed.\n"); exit(1);}

  printf("Q:\n");
  for(i=0; i<M; i++)
  {
    for(j=0; j<N; j++)
      printf("%2f ", q[M*i+j]);
    putchar('\n');
  } 
  putchar('\n');
  printf("R:\n");
  for(i=0; i<M; i++)
  {
    for(j=0; j<N; j++)
      printf("%2f ", r[M*i+j]);
    putchar('\n');
  } 
  putchar('\n');
*/

  printf("GRAM-SCHMIDT METHOD:\n");
  if(!eigenvalues_QRalgorithm(A, M, N, max_iter, tolerance, q, r, buff1, buff2, out))
   {printf("ERROR: francisQRstep() failed.\n"); exit(1);}

  printf("Q:\n");
  for(i=0; i<M; i++)
  {
    for(j=0; j<N; j++)
      printf("%2f ", q[M*i+j]);
    putchar('\n');
  } 
  putchar('\n');
  printf("R:\n");
  for(i=0; i<M; i++)
  {
    for(j=0; j<N; j++)
      printf("%2f ", r[M*i+j]);
    putchar('\n');
  } 
  putchar('\n');


  printf("Eigenvalues:\n");
  for(i=0; i<M; i++)
    printf("%e\n", out[i]);

  return(0);
}
