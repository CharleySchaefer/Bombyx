#!/bin/bash

if [ $# -lt 3 ] ; then
  echo "Usage: $0 <file name> <column number 1> <column number 2>"
  echo "  file name: e.g., path/bbx_convergence_filtered.out"
  echo "Purpose: get covariance between two columns."
  exit 1
fi

file=$1
col1=$2
col2=$3

script=$(readlink -f $0)
scriptpath=`dirname $script`



Nheader=`$scriptpath/get_Nheader.sh $file`


str=`$scriptpath/get_column_stats.sh $file $col1`
par1=`echo "$str" | awk 'NR==2 {print $3}'`
mean1=`echo "$str" | awk 'NR==2 {print $4}'`
sigma1=`echo "$str" | awk 'NR==2 {print $6}'`

str=`$scriptpath/get_column_stats.sh $file $col2`
par2=`echo "$str" | awk 'NR==2 {print $3}'`
mean2=`echo "$str" | awk 'NR==2 {print $4}'`
sigma2=`echo "$str" | awk 'NR==2 {print $6}'`





awk -vNheader=$Nheader -vcol1=$col1 -vmean1=$mean1 -vsigma1=$sigma1 -vcol2=$col2 -vmean2=$mean2 -vsigma2=$sigma2 <$file 'BEGIN {covar=0.0;}  sigma1>0 && sigma2>0 && NR>Nheader {  covar=covar+($(col1)-mean1)*($(col2)-mean2); } END {covar=((sigma1>0)&&(sigma2>0) ? covar/(NR-Nheader)/(sigma1*sigma2) : 0 ); print covar}'



