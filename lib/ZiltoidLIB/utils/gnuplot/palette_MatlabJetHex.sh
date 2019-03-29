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
    printf "#%02x%02x%02x", 0*255, 0*255 , (0.5+f*4)*255
  }
  else if (f<=0.375)
  {
    printf "#%02x%02x%02x", 0*255, (f-0.125)*4*255, 1*255
  }
  else if (f<=0.625)
  {
    printf "#%02x%02x%02x", (f-0.375)*4*255, 1*255, (1-(f-0.375)*4)*255
  }
  else if (f<=0.875)
  {
    printf "#%02x%02x%02x", 1*255, (1-(f-0.625)*4)*255, 0*255
  }
  else if(f<=1.0)
  {
    printf "#%02x%02x%02x", (1-(f-0.875)*4)*255, 0*255, 0*255
  }
  else
  {
    print -1 f
  }
}'
