#!/bin/bash


if [ $# -eq 0 ]
then
  echo " "
  echo "  For help: run $0 -h"
  echo " "
  exit 
fi

matrixfile=$1
matrixfile=${matrixfile%/} # Input (remove "/" suffix)
matrixfile=${matrixfile#./} # remove "./" prefix

pngout="matrix.png"

#cbrmax=2   # default
#Ncolours=3 # default
palette=""

for i in `seq 1 $#`
do
  if [ "${@:$i:1}" == "-h" ] # help menu
  then
    echo "  Usage: $0 <file> [-h]"
    echo "  -h: help menu"
    exit 1
  elif [ "${@:$i:1}" == "-palette" ] # help menu
  then 
    palette=${@:$i+1:1}
  fi
done 
#---------------------------------------------------


nx=`awk <$matrixfile 'END {print NF}'`
ny=`awk <$matrixfile 'END {print NR}'`

let dimy="2*nx+1"
let dimx="2*ny+1"

#---------------------------------------------------
plot="set terminal pngcairo size $dimx, $dimy;
  set output '$pngout';
  set lmargin at screen 0.0;
  set bmargin at screen 0.0;
  set rmargin at screen 1.0;
  set tmargin at screen 1.0;
  set size ratio -1;
  unset colorbox;
  unset key;
  unset tics;
  unset border;
  #set cbrange [0:$cbrmax];
  #set pal maxcolors ($Ncolours);
  set pm3d map;
"
#---------------------------------------------------
if [ "$palette" == "Matlab" ]
then
  plot="$plot set palette defined (0 0.0 0.0 0.5, \
  1 0.0 0.0 1.0, \
  2 0.0 0.5 1.0, \
  3 0.0 1.0 1.0, \
  4 0.5 1.0 0.5, \
  5 1.0 1.0 0.0, \
  6 1.0 0.5 0.0, \
  7 1.0 0.0 0.0, \
  8 0.5 0.0 0.0 )
"
fi
#---------------------------------------------------
plot="$plot
  set pm3d interpolate 0,0;
  splot '$matrixfile' matrix
"
#---------------------------------------------------

printf "$plot" | gnuplot


