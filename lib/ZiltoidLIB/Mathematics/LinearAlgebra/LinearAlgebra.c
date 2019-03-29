#include "LinearAlgebra.h"



int is_matrix_symmetric(double **matrix, int M, int N)
{
  if(M!=N)
    return(0);

  int m,n;
  int issymmetric=1;
  for(m=0; m<M; m++)
  {
    for(n=m; n<N; n++)
      if(matrix[n][m]!=matrix[m][n])
      {
        issymmetric=0; break;
      }
    if(!issymmetric)
      break;
  }
  return issymmetric;
}

/*
  matrix:  size M*N
  vec_in:  length N
  vec_out: length M
*/
int matrix_vector_product(double **matrix, int M, int N, double *vec_in,  double *vec_out)
{
  int i,j;
  double dtmp;
  for(i=0; i<M; i++)
  {
    vec_out[i]=0;
    for(j=0; j<N;j++)
    {
      if((dtmp=matrix[i][j])!=0)
        vec_out[i]+=dtmp*vec_in[j];
    }
  }  
  return(1);
}

int vector_inner_product(double *vec1_in, double *vec2_in, int N,  double *D)
{
  int i,j;
  double dtmp;
   // vec_out[i]=0;
  *D=0;
  for(j=0; j<N;j++)
  {
    *D+=vec2_in[j]*vec1_in[j];
  }
  return(1);
}


/*===========================================================================
// Original: https://github.com/jcchurch/C-Linear-Algebra
// Modified in ZiltoidLIB (2018-20-28)
 * gram_schmidt
 * This algorithm performs the Modified Gram-Schmidt algorithm for computing
 * the orthanormalized vectors of a matrix.
 *
 * This algorithm is explained in "Matrix Computations" (3rd Edition) by
 * Golub and Loan on pages 230-232. It closely follows the explicit
 * algorithm printed at the top of page 232.
 *=========================================================================*/
int gram_schmidt(double *a, int M, int N, double *buff, double *q, double *r)
{
  int i, j, k;
  double l2norm;
  double sum;
  double *vPtr;
  double *qPtr;
  double *rPtr;
  double *v=buff;

  if(M!=N)
  {printf("ERROR: Currently, QR decomposition only suports square matricies.\n"); return(0);}
//  assert(a->width == a->height, "Currently, QR decomposition only suports square matricies.");
//  assert(*q == NULL && *r == NULL, "The q and r matrices must be initalized to NULL.");

  // Because the Modified Gram-Schmidt algorithm overwrites and damages the original
  // 'A' matrix, we copy all of the values into matrix 'V' and use it instead.
  //v = copyMatrix(a);
  for(i=0; i<M*N; i++)
    //for(j=0; j<N; j++)
    v[i]=a[i];

//  *q = makeMatrix(a->width, a->height);
//  *r = makeMatrix(a->width, a->height);

  // For each column in A (now called V)
  for (k = 0; k < M; k++) 
  {
    vPtr = v + k; //v->data + k;
    // Step 1: Get the L2-Norm of column k
    l2norm = 0;
    for (i = 0; i < N; i++)
    {
      l2norm += *vPtr * *vPtr;
      vPtr += M;
    }
    l2norm = sqrt(l2norm);
    if(isnan(l2norm))
      l2norm=0;

    // Store this value in R(k,k)
    // The nice thing about the rPtr variable is that
    // it only has to be readjusted at the beginning of the
    // first 'for' loop. After each use, we just increment by 1.
    rPtr = r + (k * M) + k;
    *rPtr = l2norm;
    rPtr++;

    // Step 2: Normalize A's column k and store it in Q.
    vPtr = v+k; //v->data + k;
    qPtr = q+k; //(*q)->data + k;
    for (i = 0; i < N; i++)
    {
      *qPtr=(l2norm==0? 0 : *vPtr / l2norm);
      vPtr += M;
      qPtr += M;
    }

    // Step 3: 2 parts. For each column after K, do the following:
    for (j = k + 1; j < M; j++)
    {
      // Step 3a: Dot Product Q's column K with A's column J,
      // storing the result of the Dot Product at R(k,j)
      qPtr = q+k; //(*q)->data + k;
      vPtr = v+j; //v->data + j;
      sum = 0;
      for (i = 0; i < N; i++)
      {
        sum += *vPtr * *qPtr;
        vPtr += M;
        qPtr += M;
      }
      *rPtr = sum;
      rPtr++;

      // Step 3b: Multiply Q's column K with R(k,j)
      // (which is stored at *rPtr). Then take A's column J
      // and subtract from it Q's K * R(k,j). Take this
      // result and store it back into A's column J.
      // R(k,j) is represented by 'sum' calculated in step 3b.
      vPtr = v+j; //v->data + j;
      qPtr = q+k; //(*q)->data + k;
      for (i = 0; i <N ; i++)
      {
        *vPtr = *vPtr - (*qPtr * sum);
        vPtr += M;
        qPtr += M;
      }
    }
  }
  return(1);
}

/*===========================================================================
// Original : https://github.com/jcchurch/C-Linear-Algebra  francisQRstep()
// Modified in ZiltoidLIB (2018-20-28)
 * francisQRstep
 * This algorithm performs the Francis QR Step to find the eigen values
 * of a square matrix.
 *
 * I don't know what this algorithm should produce and I just read this
 * technique on wikipedia. Currently I have this method to test the
 * approach.
 *=========================================================================*/
int eigenvalues_QRalgorithm(double **A, int M, int N, int max_iter, double tolerance, double* q,double* r, double *buff1, double *buff2, double *out) {
    int i, j, k,l,m,n;
    int iter=0;
    int is_upper_triangle;
    double* a_k=buff1;
    double* ptr;
    double* outPtr;

  if(M!=N)
  {printf("ERROR: matrix must be square.\n"); return(0);}

  // Copy matrix a to a_k
  for(i=0; i<M; i++)
  {
    m=i*N;
    for(j=0; j<N; j++)
      a_k[m+j]=A[i][j];
  }

  while (iter<max_iter)
  {
    iter++;

    // Perform the QR decomposition of a_k
    if(!gram_schmidt(a_k, M, N, buff2, q, r))
      {printf("ERROR: gram_schmidt() failed.\n"); return(0);}

    // Matrix product a_k = r * q
    for(k=0;k<M;k++)
      for(l=0;l<N;l++)
      {
        n=k*N+l;
        a_k[n]=0;
        for(m=0; m<N; m++)
          a_k[n]+=r[k*N+m]*q[m*N+l];
      }

    // Test for completion.
    // We need to make sure this is an upper triangular matrix
    is_upper_triangle = 1; // TRUE
    for (i = 1; i < M; i++)
    {
      for (j = 0; j < i; j++)
      {
        if (fabs(a_k[i * N + j]) >= tolerance)
        {
          is_upper_triangle = 0; // FALSE
          break;
        }
      }
      if (!is_upper_triangle)
        break;
    }

    if (is_upper_triangle)
      break;
  } // end of while loop

  // Gather up all of the elements along the diagonal.
  ptr = a_k; //->data;
  outPtr = out; //->data;
  for (i = 0; i < N; i++)
  {
    *outPtr = *ptr;
    outPtr++;
    ptr += N + 1;
  }

  return(1);
}


/*
  Gauss-Jordan algorithm to sweep matrix to reduced row-echelon form
*/
void gauss_jordan(float **a, int m, int n )
{
  int ipv, i, j;
  float inv_pivot, temp;

  for(ipv = 0; ipv<m; ipv++) // Sweep over all rows
  {
    if(a[ipv][ipv]!=0)       // 
    {
      inv_pivot = 1.0 / a[ipv][ipv];

      for(j = 0; j<n; j++)
      {
        a[ipv][j] *= inv_pivot;
      }

      for(i = 0; i<n; i++)
      {
        if(i != ipv)
        {
          temp = a[i][ipv];
          for(j=0; j<n; j++)
          {
            a[i][j] -= temp*a[ipv][j];
          }
        }
      }
    }
  }
}

