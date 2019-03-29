#include <stdio.h>
#include <stdlib.h>
#include "../Fourier.h"

int main(int argc, char *argv[])
{
  printf("Demo Fourier.\n");
  int i;

  int N=16;
  complex *arr, *arr2; 

  arr =(complex*)malloc(  N*sizeof(complex));
  arr2=(complex*)malloc(2*N*sizeof(complex));

  // INITIALISE
  for(i=0; i<N; i++)
    arr[i]=sin( 2*M_PI*i/N );
  for(i=0; i<N; i++)
    printf("%8e %8e\n", creal(arr[i]), cimag(arr[i])  );
  putchar('\n'); 


  // FAST FOURIER TRANSFORM
  fft(arr, arr2, N);
  for(i=0; i<N; i++)
    printf("%8e %8e\n", creal(arr[i]), cimag(arr[i])  );
  putchar('\n'); 

  // INVERSE FAST TRANSFORM
  ifft(arr, arr2, N);
  for(i=0; i<N; i++)
    printf("%8e %8e\n", creal(arr[i]), cimag(arr[i])  );
  putchar('\n');


  // DISCRETE FOURIER TRANSFORM
  dft(arr, arr2, N);
  for(i=0; i<N; i++)
    printf("%8e %8e\n", creal(arr2[i]), cimag(arr2[i])  );
  putchar('\n');


  // INVERSE FAST TRANSFORM
  for(i=0; i<N; i++)
    arr[i]=arr2[i];
  ifft(arr, arr2, N);
  for(i=0; i<N; i++)
    printf("%8e %8e\n", creal(arr[i]), cimag(arr[i])  );
  putchar('\n');

  return(0);
}
