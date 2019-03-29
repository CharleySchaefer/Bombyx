#!/bin/bash

if [ $# -lt 2 ] ; then
  echo "Usage: $0 <file name> <column number>"
  echo "  file name: e.g., path/bbx_convergence_filtered.out"
  exit 1
fi

file=$1
col=$2

script=$(readlink -f $0)
scriptpath=`dirname $script`

Nheader=`$scriptpath/get_Nheader.sh  $file`

xlabel=`awk  -v col=$col -v Nheader=$Nheader <$file 'NR==1 {print $(col)}'`

counts=`awk  -v col=$col -v Nheader=$Nheader <$file 'BEGIN {count=0} NR>Nheader {
                         if(1) 
                          {count=count+1;}
                         }
                         END {print count}'`


mean=`awk  -v col=$col -v Nheader=$Nheader <$file 'BEGIN {count=0; sum=0.0} NR>Nheader {
                         if(1) 
                          {sum=sum+$(col);
                           count=count+1;}
                         }
                         END {print sum/count}'`


var=`awk  -v M=$mean -v Nheader=$Nheader -v col=$col <$file 'BEGIN {count=0; sum=0.0} NR>Nheader {
                         if(1) 
                          {sum=sum+($(col)-M)**2;
                           count=count+1;}
                         }
                         END {print sum/count}'`
std=`awk -v var=$var 'BEGIN {print sqrt(var)}'`

printf "%8s %8s %8s %16s %16s %16s\n" "Nheader" "counts" "par" "mean" "var" "std"
printf "%8d %8d %8s %16.5e %16.5e %16.5e\n" $Nheader $counts $xlabel $mean $var $std

