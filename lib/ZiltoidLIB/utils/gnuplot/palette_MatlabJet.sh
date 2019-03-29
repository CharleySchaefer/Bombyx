#!/bin/bash

n=$1
denom=$2

awk -vn=$n  -vdenom=$denom 'BEGIN {

  f=1.0*n/((denom))

  if (f<0)
  {
    print -1 f
  }
  else if (f<=0.125)
  {
    printf "%8f %8f %8f", 0, 0,0.5+f*4
  }
  else if (f<=0.375)
  {
    printf "%8f %8f %8f", 0, (f-0.125)*4, 1
  }
  else if (f<=0.625)
  {
    printf "%8f %8f %8f", (f-0.375)*4, 1, 1-(f-0.375)*4
  }
  else if (f<=0.875)
  {
    printf "%8f %8f %8f", 1, 1-(f-0.625)*4, 0
  }
  else if(f<=1.0)
  {
    printf "%8f %8f %8f", 1-(f-0.875)*4, 0, 0
  }
  else
  {
    print -1 f
  }
}'
