#include "eigenproblem.h"

/*
  Solve HU=DU,

  with H a real symmetric square matrix of size n*n,
  D the eigenvalues and U the eigenvectors.
 
  INPUT Before running:
  1. memory should be allocated:
    matrix: n*n     2D array
    U:      n*n     1D array
    dbuff1: 2*n-1   1D array
    dbuff2: n-1     1D array
    dbuff3: n-1     1D array
    D:      n       1D array
  2. value compz should be set to 'N' or to 'V'
  3. matrix values should be set: matrix should be symmetric!

  OUTPUT:
  D: will contain the eigenvalues
  U: If compz='V', will contain the eigenvectors
*/
int eigenproblem_real_symmetric_matrix(char compz, double **matrix, int n, double *U, double *dbuff1, double *dbuff2, double *dbuff3, double *D)
{
  if( (compz!='N')&(compz!='V') )
  {
    printf("Error: compz character should be 'N' or 'V'.\n");
    return(-1);
  }

  int i,j;
  int       info;        // = 0: successful exit
	                 // < 0: if INFO = -i, the i-th argument had an illegal value
  char      uplo='L';    // U: Upper triangle of A is stored;
  int       lwork=2*n-1; // The dimension of the array WORK.  LWORK >= 1. For optimum 
                         // performance LWORK >= N*NB, where NB is the optimal blocksize.
  //char      compz='N'; // N:  Compute eigenvalues only
  int       ldz=(compz=='V' ? n : 1);//1;
  int LDA=n;       //The leading dimension of the array A. LDA >= max(1,N).


  double    *work=dbuff1; //On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
  double    *tau=dbuff2;      // The off-diagonal elements of the tridiagonal matrix T: E(i) =
	                 // A(i,i+1) if UPLO = 'U', E(i) = A(i+1,i) if UPLO = 'L'.
  double    *E=dbuff3;    // The scalar factors of the elementary reflectors (see Further
	                 // https://physics.oregonstate.edu/~landaur/nacphy/lapack/routines/ssytrd.html).
  //double    *D=eigenvalues;        // The diagonal elements of the tridiagonal matrix T: D(i) = A(i,i).


  // Copy matrix
  for(i=0; i<n; i++)
    for(j=0; j<n; j++)
      U[i*n + j] = matrix[i][j];

  //-------------------------------------
  // DECOMPOSE TO OBTAIN A TRIDIAGONAL MATRIX
  dsytrd_(&uplo, &n, U, &LDA, D, E, tau, work,&lwork, &info); // For floats, use ssytrd
  if (info!=0)   // check for errors
  {
    printf("Error: dsytrd_ returned error code %d.\n", info);
    return -1;
  }

  // OBTAIN ORTHOGONAL MATRIX
  // SUBROUTINE DORGTR( UPLO, N, A, LDA, TAU, WORK, LWORK, INFO )
  if(compz=='V')
  {
    dorgtr_(&uplo, &n, U, &LDA, tau, work,&lwork, &info);
    if(info!=0)
      {printf("Error: dsytrd() failed - returned %d.\n", info); return(-1);}
  }

  // DIAGONALISE THE TRIDIAGONAL MATRIX
  // SUBROUTINE DPTEQR( COMPZ, N, D, E, Z,	LDZ, WORK, INFO	)
  dsteqr_(&compz, &n, D, E, U, &ldz, work, &info);

  // check for errors
  if (info!=0)
  {
    printf("Error: dsteqr_ returned error code %d.\n", info);
    return -1;
  }
  // OUTPUT:
  //   D now contains the eigenvalues
  //   U now contains the eigenvectors
  return(1);
}
