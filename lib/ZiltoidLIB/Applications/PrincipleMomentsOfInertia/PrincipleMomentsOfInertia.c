#include "PrincipleMomentsOfInertia.h"
/*
  input:
    M:      mass
    X,Y,Z:  arrays of length N with positions of mass
    N:      length of arrays M, X, Y and Z
    d1buff: buffer with length 4*3-3
    d2buff: buffer of size 3 x 3

  output: 
    PMI (array length 3): Principal moments of inertia
    PAI (array length 9): Principal axes of inertia
*/
int getPrincipalMomentsOfInertia(double *M, double  *X, double  *Y, double  *Z, int N, double *d1buff,double **d2buff,  double *PMI, double *PAI)
{
    //INERTIA TENSOR
  int i,j;
  int nmat=3; // Size of 3D inertia tensor

  double xmean = d1mean(    X, N); // Mean of array
  double ymean = d1mean(    Y, N); // Mean of array
  double zmean = d1mean(    Z, N); // Mean of array

  double I11=0, I22=0, I33=0,
         I12, I21, I13, I31, I23, I32;

  int x,y,z;
  double R, d, RdotU, theta;

  // Get inertia tensor
  for(i=0; i<N; i++)
  {
    I11 += M[i]*(  (Y[i]-ymean)*(Y[i]-ymean) + (Z[i]-zmean)*(Z[i]-zmean)  );
    I12 -= M[i]*(  (X[i]-xmean)*(Y[i]-ymean)  );
    I13 -= M[i]*(  (X[i]-xmean)*(Z[i]-zmean)  );

    I22 += M[i]*(  (X[i]-xmean)*(X[i]-xmean) + (Z[i]-zmean)*(Z[i]-zmean)  );
    I23 -= M[i]*(  (Z[i]-zmean)*(Y[i]-ymean)  );

    I33 += M[i]*(  (X[i]-xmean)*(X[i]-xmean) + (Y[i]-ymean)*(Y[i]-ymean)  );
  }
  I21=I12;
  I31=I13;
  I32=I23;

  double **Imat=d2buff;
  Imat[0][0]=I11;  Imat[0][1]=I12;  Imat[0][2]=I13;
  Imat[1][0]=I21;  Imat[1][1]=I22;  Imat[1][2]=I23;
  Imat[2][0]=I31;  Imat[2][1]=I32;  Imat[2][2]=I33;

  // Do eigendecomposition I = Q PMI Q^T  (ZiltoidLIB/Mathematics/lapack)
  double *d1buffA = d1buff;
  double *d1buffB = &(d1buff[2*nmat-1]);
  double *d1buffC = &(d1buff[3*nmat-2]);
  eigenproblem_real_symmetric_matrix('V', Imat, nmat, PAI, d1buffA, d1buffB, d1buffC, PMI);

  return(1);
}
