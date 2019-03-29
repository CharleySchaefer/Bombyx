#!/bin/bash

if [ $# != 1 ]; then
  echo "Usage:   $0 <file name>"
  echo "Purpose: Determine number of header lines in plaintext data file."
  exit 1; 
fi 
fname=$1

awk <$fname 'BEGIN {check=0; Nheader=0} NF>1 && check==0 { c=substr($1,0,1); check=(('0'<=c) && (c<='9')); Nheader=Nheader+1;    } END{print Nheader-1} '
