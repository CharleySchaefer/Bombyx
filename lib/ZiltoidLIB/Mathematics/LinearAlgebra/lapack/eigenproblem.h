/*
  
  Eigenvalues & Eigenvectors of a real symmetric matrix using LAPACK

  Routines: DSYTRD, DORGTR, DSTEQR are detailed below.

  USAGE:
    Install LAPACK: apt-get install libblas-dev liblapack-dev
    Compile script: gcc -o executable.o eigenvalues_real_symmetric_matrix.c -llapack
  
  ALGORITHM:

  Eigenvalues ( compz='N' ):
    0. Define real symmetric matrix A
    1. Decompose A = Q * T * Q', with Q an orthogonal 
       matrix (Q=Q'), with Q' the transposed Q,
       and T a tridiagonal matrix. Our script does this 
       using the DSTRD routine.
    2. Decompose T = Q * D * Q', with Q again an 
       orthogonal matrix. D is now a diagonal matrix,
       which contains the eigenvalues. Our script does 
       this using DSTEQR

  Eigenvectors ( compz='V' ):
    0. Define real symmetric matrix A
    1. Decompose A = Q * T * Q', with Q an orthogonal 
       matrix (Q=Q'), with Q' the transposed Q,
       and T a tridiagonal matrix. Our script does this 
       using  DSTRD.
    2. Explicitly obtain Q from step 1.
       This is done using DORGTR 
    3. Decompose T = Q * D * Q', with Q again an 
       orthogonal matrix. D is now a diagonal matrix,
       which contains the eigenvalues. Our script does 
       this using DSTEQR
          
    Alternatives to DSTEQR 
    (see https://physics.oregonstate.edu/~landaur/nacphy/lapack/eigen.html) are: 
    dsteqr: Computes all eigenvalues and eigenvectors 
            of a real symmetric tridiagonal matrix, 
            using the implicit QL or QR algorithm
    dsterf: Computes all eigenvalues of a real symmetric 
            tridiagonal matrix, using a root-free variant 
            of the QL or QR algorithm
    dstebz: Computes selected eigenvalues of a real symmetric 
            tridiagonal matrix by bisection
    dstein: Computes selected eigenvectors of a real symmetric 
            tridiagonal matrix by inverse iteration
    dpteqr: Computes all eigenvalues and eigenvectors of a 
            real symmetric positive definite tridiagonal 
            matrix, by computing the SVD of its bidiagonal 
            Cholesky factor
*/
#include <stdio.h>
int eigenproblem_real_symmetric_matrix(char,  double **, int, double *, double *, double *, double *, double *);


/*
  https://physics.oregonstate.edu/~landaur/nacphy/lapack/routines/dsytrd.html

  DSYTRD(l)		LAPACK routine (version	1.1)		    DSYTRD(l)

NAME
  DSYTRD - reduce a real symmetric matrix A to real symmetric tridiagonal
  form T by an orthogonal similarity transformation

SYNOPSIS

  SUBROUTINE DSYTRD( UPLO, N, A, LDA, D, E, TAU, WORK, LWORK, INFO )

      CHARACTER	     UPLO

      INTEGER	     INFO, LDA,	LWORK, N

      DOUBLE	     PRECISION A( LDA, * ), D( * ), E( * ), TAU( * ), WORK( *
		     )

PURPOSE
  DSYTRD reduces a real	symmetric matrix A to real symmetric tridiagonal form
  T by an orthogonal similarity	transformation:	Q**T * A * Q = T.

ARGUMENTS

  UPLO	  (input) CHARACTER*1
	  = 'U':  Upper	triangle of A is stored;
	  = 'L':  Lower	triangle of A is stored.

  N	  (input) INTEGER
	  The order of the matrix A.  N	>= 0.

  A	  (input/output) DOUBLE	PRECISION array, dimension (LDA,N)
	  On entry, the	symmetric matrix A.  If	UPLO = 'U', the	leading	N-
	  by-N upper triangular	part of	A contains the upper triangular	part
	  of the matrix	A, and the strictly lower triangular part of A is not
	  referenced.  If UPLO = 'L', the leading N-by-N lower triangular
	  part of A contains the lower triangular part of the matrix A,	and
	  the strictly upper triangular	part of	A is not referenced.  On
	  exit,	if UPLO	= 'U', the diagonal and	first superdiagonal of A are
	  overwritten by the corresponding elements of the tridiagonal matrix
	  T, and the elements above the	first superdiagonal, with the array
	  TAU, represent the orthogonal	matrix Q as a product of elementary
	  reflectors; if UPLO =	'L', the diagonal and first subdiagonal	of A
	  are over- written by the corresponding elements of the tridiagonal
	  matrix T, and	the elements below the first subdiagonal, with the
	  array	TAU, represent the orthogonal matrix Q as a product of ele-
	  mentary reflectors. See Further Details.  LDA	    (input) INTEGER
	  The leading dimension	of the array A.	 LDA >=	max(1,N).

  D	  (output) DOUBLE PRECISION array, dimension (N)
	  The diagonal elements	of the tridiagonal matrix T: D(i) = A(i,i).

  E	  (output) DOUBLE PRECISION array, dimension (N-1)
	  The off-diagonal elements of the tridiagonal matrix T: E(i) =
	  A(i,i+1) if UPLO = 'U', E(i) = A(i+1,i) if UPLO = 'L'.

  TAU	  (output) DOUBLE PRECISION array, dimension (N-1)
	  The scalar factors of	the elementary reflectors (see Further
	  Details).

  WORK	  (workspace) DOUBLE PRECISION array, dimension	(LWORK)
	  On exit, if INFO = 0,	WORK(1)	returns	the optimal LWORK.

  LWORK	  (input) INTEGER
	  The dimension	of the array WORK.  LWORK >= 1.	 For optimum perfor-
	  mance	LWORK >= N*NB, where NB	is the optimal blocksize.

  INFO	  (output) INTEGER
	  = 0:	successful exit
	  < 0:	if INFO	= -i, the i-th argument	had an illegal value

FURTHER	DETAILS
  If UPLO = 'U', the matrix Q is represented as	a product of elementary
  reflectors

     Q = H(n-1)	. . . H(2) H(1).

  Each H(i) has	the form

     H(i) = I -	tau * v	* v'

  where	tau is a real scalar, and v is a real vector with
  v(i+1:n) = 0 and v(i)	= 1; v(1:i-1) is stored	on exit	in
  A(1:i-1,i+1),	and tau	in TAU(i).

  If UPLO = 'L', the matrix Q is represented as	a product of elementary
  reflectors

     Q = H(1) H(2) . . . H(n-1).

  Each H(i) has	the form

     H(i) = I -	tau * v	* v'

  where	tau is a real scalar, and v is a real vector with
  v(1:i) = 0 and v(i+1)	= 1; v(i+2:n) is stored	on exit	in A(i+2:n,i), and
  tau in TAU(i).

  The contents of A on exit are	illustrated by the following examples with n
  = 5:

  if UPLO = 'U':		       if UPLO = 'L':

    (  d   e   v2  v3  v4 )		 (  d		       )
    (	   d   e   v3  v4 )		 (  e	d	       )
    (	       d   e   v4 )		 (  v1	e   d	       )
    (		   d   e  )		 (  v1	v2  e	d      )
    (		       d  )		 (  v1	v2  v3	e   d  )

  where	d and e	denote diagonal	and off-diagonal elements of T,	and vi
  denotes an element of	the vector defining H(i).

*/

/*
https://physics.oregonstate.edu/~landaur/nacphy/lapack/routines/dorgtr.html

DORGTR(l)		LAPACK routine (version	1.1)		    DORGTR(l)

NAME
  DORGTR - generate a real orthogonal matrix Q which is	defined	as the pro-
  duct of n-1 elementary reflectors of order N,	as returned by DSYTRD

SYNOPSIS

  SUBROUTINE DORGTR( UPLO, N, A, LDA, TAU, WORK, LWORK,	INFO )

      CHARACTER	     UPLO

      INTEGER	     INFO, LDA,	LWORK, N

      DOUBLE	     PRECISION A( LDA, * ), TAU( * ), WORK( LWORK )

PURPOSE
  DORGTR generates a real orthogonal matrix Q which is defined as the product
  of n-1 elementary reflectors of order	N, as returned by DSYTRD:

  if UPLO = 'U', Q = H(n-1) . .	. H(2) H(1),

  if UPLO = 'L', Q = H(1) H(2) . . . H(n-1).

ARGUMENTS

  UPLO	  (input) CHARACTER*1
	  = 'U': Upper triangle	of A contains elementary reflectors from
	  DSYTRD; = 'L': Lower triangle	of A contains elementary reflectors
	  from DSYTRD.

  N	  (input) INTEGER
	  The order of the matrix Q. N >= 0.

  A	  (input/output) DOUBLE	PRECISION array, dimension (LDA,N)
	  On entry, the	vectors	which define the elementary reflectors,	as
	  returned by DSYTRD.  On exit,	the N-by-N orthogonal matrix Q.

  LDA	  (input) INTEGER
	  The leading dimension	of the array A.	LDA >= max(1,N).

  TAU	  (input) DOUBLE PRECISION array, dimension (N-1)
	  TAU(i) must contain the scalar factor	of the elementary reflector
	  H(i),	as returned by DSYTRD.

  WORK	  (workspace) DOUBLE PRECISION array, dimension	(LWORK)
	  On exit, if INFO = 0,	WORK(1)	returns	the optimal LWORK.

  LWORK	  (input) INTEGER
	  The dimension	of the array WORK. LWORK >= max(1,N-1).	 For optimum
	  performance LWORK >= (N-1)*NB, where NB is the optimal blocksize.

  INFO	  (output) INTEGER
	  = 0:	successful exit
	  < 0:	if INFO	= -i, the i-th argument	had an illegal value

*/

/*
  https://physics.oregonstate.edu/~landaur/nacphy/lapack/routines/dsteqr.html
  DSTEQR(l)		LAPACK routine (version	1.1)		    DSTEQR(l)

NAME
  DSTEQR - compute all eigenvalues and,	optionally, eigenvectors of a sym-
  metric tridiagonal matrix using the implicit QL or QR	method

SYNOPSIS

  SUBROUTINE DSTEQR( COMPZ, N, D, E, Z,	LDZ, WORK, INFO	)

      CHARACTER	     COMPZ

      INTEGER	     INFO, LDZ,	N

      DOUBLE	     PRECISION D( * ), E( * ), WORK( * ), Z( LDZ, * )

PURPOSE
  DSTEQR computes all eigenvalues and, optionally, eigenvectors	of a sym-
  metric tridiagonal matrix using the implicit QL or QR	method.	 The eigen-
  vectors of a full or band symmetric matrix can also be found if DSYTRD or
  DSPTRD or DSBTRD has been used to reduce this	matrix to tridiagonal form.

ARGUMENTS

  COMPZ	  (input) CHARACTER*1
	  = 'N':  Compute eigenvalues only.
	  = 'V':  Compute eigenvalues and eigenvectors of the original sym-
	  metric matrix.  On entry, Z must contain the orthogonal matrix used
	  to reduce the	original matrix	to tridiagonal form.  =	'I':  Compute
	  eigenvalues and eigenvectors of the tridiagonal matrix.  Z is	ini-
	  tialized to the identity matrix.

  N	  (input) INTEGER
	  The order of the matrix.  N >= 0.

  D	  (input/output) DOUBLE	PRECISION array, dimension (N)
	  On entry, the	diagonal elements of the tridiagonal matrix.  On
	  exit,	if INFO	= 0, the eigenvalues in	ascending order.

  E	  (input/output) DOUBLE	PRECISION array, dimension (N-1)
	  On entry, the	(n-1) subdiagonal elements of the tridiagonal matrix.
	  On exit, E has been destroyed.

  Z	  (input/output) DOUBLE	PRECISION array, dimension (LDZ, N)
	  On entry, if	COMPZ =	'V', then Z contains the orthogonal matrix
	  used in the reduction	to tridiagonal form.  On exit, if  COMPZ =
	  'V', Z contains the orthonormal eigenvectors of the original sym-
	  metric matrix, and if	COMPZ =	'I', Z contains	the orthonormal
	  eigenvectors of the symmetric	tridiagonal matrix.  If	an error exit
	  is made, Z contains the eigenvectors associated with the stored
	  eigenvalues.	If COMPZ = 'N',	then Z is not referenced.

  LDZ	  (input) INTEGER
	  The leading dimension	of the array Z.	 LDZ >=	1, and if
	  eigenvectors are desired, then  LDZ >= max(1,N).

  WORK	  (workspace) DOUBLE PRECISION array, dimension	(max(1,2*N-2))
	  If COMPZ = 'N', then WORK is not referenced.

  INFO	  (output) INTEGER
	  = 0:	successful exit
	  < 0:	if INFO	= -i, the i-th argument	had an illegal value
	  > 0:	the algorithm has failed to find all the eigenvalues in	a
	  total	of 30*N	iterations; if INFO = i, then i	elements of E have
	  not converged	to zero; on exit, D and	E contain the elements of a
	  symmetric tridiagonal	matrix which is	orthogonally similar to	the
	  original matrix.

Back to the listing of computational routines for eigenvalue problems 
*/
