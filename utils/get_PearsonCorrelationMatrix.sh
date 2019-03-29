#!/bin/bash

if [ $# -lt 1 ] ; then
  echo "Usage: $0 <file name>"
  echo "  file name: e.g., path/bbx_convergence_filtered.out"
  exit 1
fi

file=$1

script=$(readlink -f $0)
scriptpath=`dirname $script`

labels=`awk <$file 'NR==1 {print $0}'`

# print header
str="            "
for j in `seq 2 6` ; do
  lab=`echo "$labels" | awk -vj=$j '{print $(j)}'`
  str=`printf "$str %12s" "$lab"`
done
printf "$str\n"

# print label + covariances
for j in `seq 2 6` ; do
  lab=`echo "$labels" | awk -vj=$j '{print $(j)}'`
  str=`printf "%12s" "$lab"`

  for i in `seq 2 6` ; do
    covar=`$scriptpath/get_column_covar.sh $file $j $i`
    str=`printf "$str %12.4f" $covar`
  done
  printf "$str\n"
done


