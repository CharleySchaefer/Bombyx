#include "Mathematics.h"


// MIN MAX ABS
double maximum(double a, double b)
{
  return (a > b ? a : b);	
}
double minimum(double a, double b)
{
  return (a < b ? a : b);
}
int    maximum_int(int a, int b)
{
  return (a > b ? a : b);	
}
int    minimum_int(int a, int b){
  return (a < b ? a : b);
}
double absolute(double x)
{
  return (x > 0.0 ? x : -x);
}

int dec2bin(int dec, int *bin, int Nbin) 
{
  int remainder; 
  int binary = 0, i = 1, j=0;
  
  while(dec != 0) 
  {
    remainder = dec%2;
    dec = dec/2;
    binary= binary + (remainder*i);
    i = i*10;

    bin[Nbin-1-j]=remainder; j++;
  }
  while(j<Nbin)
  {
    bin[Nbin-1-j]=0; j++;
  }
  return 1;
}

int bin2dec(int *n, int *bin, int Nbin)
{
  int i,j=1;
  *n=0;
  for(i=0; i<Nbin; i++)
  {
    *n+=bin[Nbin-1-i]*j;
    j*=2;
  }
  return(1);
}



int isPowerOfTwo (unsigned int x)
{
 while (((x % 2) == 0) && x > 1) /* While x is even and > 1 */
   x /= 2;
 return (x == 1);
}


double d1min(double *arr, int N) // Minimum of array
{
  int i; double min=arr[0];
  for (i=1; i<N; i++)
    min=(arr[i]<min ? arr[i] : min);
  return min;
}
double d1max(double *arr, int N) // Maximum of array
{
  int i; double max=arr[0];
  for (i=1; i<N; i++)
    max=(arr[i]>max ? arr[i] : max);
  return max;
}

double d1mean(double *arr, int N) // Mean of array
{
  int i; double mean=0;
  for (i=0; i<N; i++)
    mean += arr[i];
  mean/=N;
  return mean;
}

double d1variance(double *arr, int N) // variance of array
{
  int i; 
  double mean=d1mean(arr, N);
  double variance=0;
  for (i=0; i<N; i++)
    variance += pow(arr[i]-mean,2);
  variance/=N;
  return variance;
}

float f1min(float *arr, int N) // Minimum of array
{
  int i; float min=arr[0];
  for (i=1; i<N; i++)
    min=(arr[i]<min ? arr[i] : min);
  return min;
}
float f1max(float *arr, int N) // Maximum of array
{
  int i; float max=arr[0];
  for (i=1; i<N; i++)
    max=(arr[i]>max ? arr[i] : max);
  return max;
}

float f1mean(float *arr, int N) // Mean of array
{
  int i; float mean=0;
  for (i=0; i<N; i++)
    mean += arr[i];
  mean/=N;
  return mean;
}

float f1variance(float *arr, int N) // variance of array
{
  int i; 
  float mean=f1mean(arr, N);
  float variance=0;
  for (i=0; i<N; i++)
    variance += pow(arr[i]-mean,2);
  variance/=N;
  return variance;
}

double di1mean(int *arr, int N) // Mean of array
{
  int i; double mean=0;
  for (i=0; i<N; i++)
    mean += arr[i];
  mean/=N;
  return mean;
}

double di1variance(int *arr, int N) // variance of array
{
  int i; 
  double mean=di1mean(arr, N);
  double variance=0;
  for (i=0; i<N; i++)
    variance += pow(arr[i]-mean,2);
  variance/=N;
  return variance;
}



int i1min(int *arr, int N) // Minimum of array
{
  int i; int min=arr[0];
  for (i=1; i<N; i++)
    min=(arr[i]<min ? arr[i] : min);
  return min;
}
int i1max(int *arr, int N) // Maximum of array
{
  int i; int max=arr[0];
  for (i=1; i<N; i++)
    max=(arr[i]>max ? arr[i] : max);
  return max;
}

/*
  x1find:
    Find value in an array
    Output:
      -1: Did not find the value
     >-1: Smallest array index where value was found
*/
int i1find( int target, int *array, int N) // Find value in array
{
  int i; 
  for(i=0; i<N; i++)
  {
    if(array[i]==target)
    {
      return(i); // on success
      break;
    }
  }
  return(-1);    // on fail
}

int f1find( float target, float *array, int N) // Find value in array
{
  int i; 
  for(i=0; i<N; i++)
  {
    if(array[i]==target)
    {
      return(i); // on success
      break;
    }
  }
  return(-1);    // on fail
}

int d1find( double target, double *array, int N) // Find value in array
{
  int i; 
  for(i=0; i<N; i++)
  {
    if(array[i]==target)
    {
      return(i); // on success
      break;
    }
  }
  return(-1);    // on fail
}


//-----------------------------------------------------------------------------------------------
// First order derivative of a 2D array 
// Periodic boundary conditions
// Uses central difference
//direction: 'x' or 'y'
//ddi Inverted grid spacing squared in 'direction' (e.g. 1/(2*dx))
void gradarray2D(double **Array2D, char direction, int N, double ddi, int i, int j, double *grad)
{
  switch(direction)
  {
    case 'x':
      if(i==0)
        *grad = ddi*(Array2D[1][j] - Array2D[N-1][j]);
      else if (i==N-1)
        *grad = ddi*(Array2D[0][j] - Array2D[N-2][j]);
      else
        *grad = ddi*(Array2D[i+1][j] - Array2D[i-1][j]);
      break;
    case 'y':
      if(j==0)
        *grad = ddi*(Array2D[i][1] - Array2D[i][N-1]);
      else if (j==N-1)
        *grad = ddi*(Array2D[i][0] - Array2D[i][N-2]);
      else
        *grad = ddi*(Array2D[i][j+1] - Array2D[i][j-1]);
      break;
  }
}


/*-------------------- Global Function Description Block ----------------------
 *
 *     ***QUINTIC************************************************25.03.98
 *     Solution of a quintic equation by a hybrid method:
 *     first real solution is obtained numerically by the Newton method,
 *     the remaining four roots are obtained analytically by QUARTIC
 *     NO WARRANTY, ALWAYS TEST THIS SUBROUTINE AFTER DOWNLOADING
 *     ******************************************************************
 *     dd(0:4)     (i)  vector containing the polynomial coefficients
 *     sol(1:4)    (o)  results, real part
 *     soli(1:4)   (o)  results, imaginary part
 *     Nsol        (o)  number of real solutions 
 *	17-Oct-2004 / Raoul Rausch
 *		Conversion from Fortran to C
 *
 *
 *-----------------------------------------------------------------------------
 */
int quintic(double dd[6], double sol[5], double soli[5], int *Nsol, double xstart)
{
	double  dd4[5], sol4[4], soli4[4], xnew, xs;//, soli4[4];/*dd[6], sol[5], soli[5],*/
	double sum, sum1, eC;
	const double eps = 1.e-8;
	int i, Nsol4;

	*Nsol = 0;

	// printf("\n Quintic!\n");

	if (dd[5] == 0.0)
	{ 
		printf("\n ERROR: NOT A QUINTIC EQUATION");
		return 0;
	}

	// Newton iteration of one real root
	xs= xstart;
	xnew = xstart;	//added rr
	do
	{
		xs = xnew;	//added rr
		sum = dd[0];
		for (i=1;i<6;i++)	sum += dd[i]*pow(xs,i);	// Don't know what ** means
		sum1 = dd[1];
		for (i=1;i<5;i++)	sum1 += (double)(i+1)*dd[i+1]*pow(xs,i);
		xnew = xs - sum/sum1;
		//if (fabs(xnew-xs) > eps)
		//xs =xnew;
	//	printf("\n %f\t%f!", xs, xnew);
	}while (fabs(xnew-xs) > eps);

	eC = xnew;
	//
	// "eC" is one real root of quintic equation
	// reduce quintic to quartic equation using "eC"
	dd4[4] = dd[5];
	for (i=4;i>0;i--)	dd4[i-1] = dd[i] + eC*dd4[i];

	quartic(dd4, sol4, soli4, &Nsol4);

	
	sol[0] = eC;
	soli[0] = 0.0;

	for (i=0;i<4;i++)
	{
		sol[i+1] =sol4[i];
		soli[i+1] = soli4[i];
	}
	*Nsol = Nsol4 + 1;

	return 0;
}

/*-------------------- Global Function Description Block ----------------------
 *
 *     ***QUARTIC************************************************25.03.98
 *     Solution of a quartic equation
 *     ref.: J. E. Hacke, Amer. Math. Monthly, Vol. 48, 327-328, (1941)
 *     NO WARRANTY, ALWAYS TEST THIS SUBROUTINE AFTER DOWNLOADING
 *     ******************************************************************
 *     dd(0:4)     (i)  vector containing the polynomial coefficients
 *     sol(1:4)    (o)  results, real part
 *     soli(1:4)   (o)  results, imaginary part
 *     Nsol        (o)  number of real solutions 
 *     ==================================================================
 *  	17-Oct-2004 / Raoul Rausch
 *		Conversion from Fortran to C
 *
 *
 *-----------------------------------------------------------------------------
 */
 int quartic(double dd[5], double sol[4], double soli[4], int* Nsol)
 {
	double AA[4], z[3];
	double a, b, c, d, f, p, q, r, zsol, xK2, xL, xK, sqp, sqm;
	int ncube, i;
	*Nsol = 0;

	if (dd[4] == 0.0)
	{
		printf("\n ERROR: NOT A QUARTIC EQUATION");
		return 0;
	}

	a = dd[4];
	b = dd[3];
	c = dd[2];
	d = dd[1];
	f = dd[0];

	p = (-3.0*pow(b,2) + 8.0 *a*c)/(8.0*pow(a,2));
	q = (pow(b,3) - 4.0*a*b*c + 8.0 *d*pow(a,2)) / (8.0*pow(a,3));
	r = (-3.0*pow(b,4) + 16.0 *a*pow(b,2)*c - 64.0 *pow(a,2)*b*d + 256.0 *pow(a,3)*f)/(256.0*pow(a,4));
	
	// Solve cubic resolvent
	AA[3] = 8.0;
	AA[2] = -4.0*p;
	AA[1] = -8.0*r;
	AA[0] = 4.0*p*r - pow(q,2);

	//printf("\n bcubic %.4e\t%.4e\t%.4e\t%.4e ", AA[0], AA[1], AA[2], AA[3]);
	cubic(AA, z, &ncube);
	//printf("\n acubic %.4e\t%.4e\t%.4e ", z[0], z[1], z[2]);
	
	zsol = - 1.e99;
	for (i=0;i<ncube;i++)	zsol = (zsol > z[i] ? zsol : z[i]);	
	z[0] =zsol;
	xK2 = 2.0*z[0] -p;
	xK = sqrt(xK2);
	xL = q/(2.0*xK);
	sqp = xK2 - 4.0 * (z[0] + xL);
	sqm = xK2 - 4.0 * (z[0] - xL);

	for (i=0;i<4;i++)	soli[i] = 0.0;
	if ( (sqp >= 0.0) && (sqm >= 0.0))
	{
	//	printf("\n case 1 ");
		sol[0] = 0.5 * (xK + sqrt(sqp));
		sol[1] = 0.5 * (xK - sqrt(sqp));
		sol[2] = 0.5 * (-xK + sqrt(sqm));
		sol[3] = 0.5 * (-xK - sqrt(sqm));
		*Nsol = 4;
	}
	else if ( (sqp >= 0.0) && (sqm < 0.0))
	{
	//	printf("\n case 2 ");
		sol[0] = 0.5 * (xK + sqrt(sqp));
		sol[1] = 0.5 * (xK - sqrt(sqp));
		sol[2] = -0.5 * xK;
		sol[3] = -0.5 * xK;
		soli[2] =  sqrt(-.25 * sqm);
		soli[3] = -sqrt(-.25 * sqm);
		*Nsol = 2;
	}
	else if ( (sqp < 0.0) && (sqm >= 0.0))
	{
	//	printf("\n case 3 ");
		sol[0] = 0.5 * (-xK + sqrt(sqm));
		sol[1] = 0.5 * (-xK - sqrt(sqm));
		sol[2] = 0.5 * xK;
		sol[3] = 0.5 * xK;
		soli[2] =  sqrt(-0.25 * sqp);
		soli[3] = -sqrt(-0.25 * sqp);
		*Nsol = 2;
	}
	else if ( (sqp < 0.0) && (sqm < 0.0))
	{
	//	printf("\n case 4 ");
		sol[0] = -0.5 * xK;
		sol[1] = -0.5 * xK;
		soli[0] =  sqrt(-0.25 * sqm);
		soli[1] = -sqrt(-0.25 * sqm);
		sol[2] = 0.5 * xK;
		sol[3] = 0.5 * xK;
		soli[2] =  sqrt(-0.25 * sqp);
		soli[3] = -sqrt(-0.25 * sqp);
		*Nsol = 0;
	}
	
	for (i=0;i<4;i++)	sol[i] -= b/(4.0*a);
	return 0;

 }

 /*-------------------- Global Function Description Block ----------------------
  *
  *     ***CUBIC************************************************08.11.1986
  *     Solution of a cubic equation
  *     Equations of lesser degree are solved by the appropriate formulas.
  *     The solutions are arranged in ascending order.
  *     NO WARRANTY, ALWAYS TEST THIS SUBROUTINE AFTER DOWNLOADING
  *     ******************************************************************
  *     A(0:3)      (i)  vector containing the polynomial coefficients
  *     X(1:L)      (o)  results
  *     L           (o)  number of valid solutions (beginning with X(1))
  *     ==================================================================
  *  	17-Oct-2004 / Raoul Rausch
  *		Conversion from Fortran to C
  *
  *
  *-----------------------------------------------------------------------------
  */
int cubic(double A[4], double X[3], int* L)
{
//	const double PI = 3.1415926535897932;
	const double THIRD = 1./3.;
	double U[3],W, P, Q, DIS, PHI, tmp, tmp2, tmp3;
	int i;

	//define cubic root as statement function
	// In C, the function is defined outside of the cubic fct

	// ====determine the degree of the polynomial ====

	if (A[3] != 0.0)
	{
		//cubic problem
		W = A[2]/A[3]*THIRD;
		P = pow((A[1]/A[3]*THIRD - pow(W,2)),3);
		Q = -.5*(2.0*pow(W,3)-(A[1]*W-A[0])/A[3] );
		DIS = pow(Q,2)+P;
		if ( DIS < 0.0 )
		{
			//three real solutions!
			//Confine the argument of ACOS to the interval [-1;1]!
			tmp = (-1.0 > Q/sqrt(-P) ? -1.0 : Q/sqrt(-P));	// Maximum
			PHI = acos(   (1.0<tmp? 1.0 : tmp  )   );		// Minimum
			P=2.0*pow((-P),(5.e-1*THIRD));
			for (i=0;i<3;i++)	U[i] = P*cos((PHI+2*((double)i)*M_PI)*THIRD)-W;
			
			tmp = (U[1]<U[2] ? U[1] : U[2]); // Minimum
			X[0] = (U[0]<tmp ? U[0] : tmp); // Minimum 
			
			tmp  = (U[0] < U[1] ? U[0] : U[1]); // Minimum
			tmp2 = (U[0] < U[2] ? U[0] : U[2]); // Minimum
			tmp3 = (U[1] < U[2] ? U[1] : U[2]); // Minimum
			tmp2 = (tmp2 > tmp3 ? tmp2 : tmp3); // Maximum
			X[1] = (tmp > tmp2 ? tmp : tmp2);   // Maximum
	//		X[1] = max(min(U[0], U[1]),max( min(U[0], U[2]), min(U[1], U[2])));
	
			tmp  = (U[1] > U[2] ? U[1] : U[2]); // Maximum
			X[2] = (U[0] > tmp ? U[0] : tmp);   // Maximum
	//		X[2] = max(U[0], max(U[1], U[2]));
			*L = 3;
		}
		else
		{
			// only one real solution!
			DIS = sqrt(DIS);
			X[0] = CBRT(Q+DIS)+CBRT(Q-DIS)-W;
			*L=1;
		}
	}
	else if (A[2] != 0.0)
	{
		// quadratic problem
		P = 0.5*A[1]/A[2];
		DIS = pow(P,2)-A[0]/A[2];
		if (DIS > 0.0)
		{
			// 2 real solutions
			X[0] = -P - sqrt(DIS);
			X[1] = -P + sqrt(DIS);
			*L=2;
		}
		else
		{
			// no real solution
			*L=0;
		}
	}
	else if (A[1] != 0.0)
	{
		//linear equation
		X[0] =A[0]/A[1];
		*L=1;
	}
	else
	{
		//no equation
		*L=0;
	}
 /*
  *     ==== perform one step of a newton iteration in order to minimize
  *          round-off errors ====
  */
	for (i=0;i<*L;i++)
	{
		X[i] = X[i] - (A[0]+X[i]*(A[1]+X[i]*(A[2]+X[i]*A[3])))/(A[1]+X[i]*(2.0*A[2]+X[i]*3.0*A[3]));
	//	printf("\n X inside cubic %.15e\n", X[i]);
	}

	return 0;
}

int signR(double Z)
{
	int ret;
	if (Z > 0.0)	ret = 1;
	if (Z < 0.0)	ret = -1;
	if (Z == 0.0)	ret =0;

	return ret;
}

double CBRT(double Z)
{
	double ret;
	const double THIRD = 1./3.;
	//define cubic root as statement function
	//SIGN has different meanings in both C and Fortran
	// Was unable to use the sign command of C, so wrote my own
	// that why a new variable needs to be introduced that keeps track of the sign of
	// SIGN is supposed to return a 1, -1 or 0 depending on what the sign of the argument is
	ret = fabs(pow(fabs(Z),THIRD)) * signR(Z);
	return ret;
}


