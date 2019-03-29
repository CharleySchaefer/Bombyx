#include "mergesort.h"
/*
    Sort a double array using the mergesort algorithm
*/
void dmergesort(double *arr, int N){
  int    ind, i1,  i2;
  int         N1,  N2;
  double     *p1, *p2;
  double *dbuff;
  
  if( N>1 )
  {
    dbuff = (double*)malloc(N*sizeof(double));

    // split array of length N into arrays of length N1 and N2
    N1 = N/2;
    N2 = N-N1;
    p1 = arr;
    p2 = arr+N1;

    // mergesort
    dmergesort(p1, N1);
    dmergesort(p2, N2);
  
    // merge
    i1=0; i2=0;
    for (ind=0; ind<N; ind++)
    {
      if (i1==N1){            // end of array 1
        dbuff[ind] = p2[i2];   i2++;
      } else if (i2==N2){     // end of array 2
        dbuff[ind] = p1[i1];   i1++;
      } else {                // Get minimum from arrays 1 and 2
        if(  p1[i1]<p2[i2]  ){
          dbuff[ind] = p1[i1]; i1++;
        } else {
          dbuff[ind] = p2[i2]; i2++;
        }
      }
    }

    // copy sorted buffer to array and free buffer
    for(ind=0; ind<N; ind++)
      arr[ind]=dbuff[ind];
    free(dbuff);
  }
}
