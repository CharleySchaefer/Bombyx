#!/bin/bash

datafile=$1

plot="set terminal pngcairo enhanced
set output \"SF.png\"

set log x
set xlabel \"q\"

set log y
set ylabel \"S(q)\"

unset key

plot \"$datafile\" u 1:2 w l lt 1 lw 2 lc rgb \"#AA4444\"
"

printf "$plot" | gnuplot
