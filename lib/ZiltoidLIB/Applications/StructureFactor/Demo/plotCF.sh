#!/bin/bash

datafile=$1

plot="set terminal pngcairo enhanced
set output \"CF.png\"

#set log x
set xlabel \"R\"

#set log y
set ylabel \"C(R)\"

unset key

plot \"$datafile\" u 3:4 w l lt 1 lw 2 lc rgb \"#AA4444\"
"

printf "$plot" | gnuplot
