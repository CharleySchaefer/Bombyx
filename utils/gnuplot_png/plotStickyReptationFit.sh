#!/bin/bash

reformatteddata="bbx_data_in.bbx"
bbxmodelfile="bbx_model.out"

plot="set terminal pngcairo enhanced
set output \"img/StickyReptation.png\"

set log xy
set xlabel \"omega [rad/s]\"
#set xrange [1e-4: 1e11]
#set xtics 1e-6, 1e3, 1e12
set format x \"10^\{%%T\}\"

set ylabel \"G', G'' [Pa s]\"
#set yrange [1e-1:1e7]
#set ytics 1e-3, 1e3, 1e9
set format y \"10^\{%%T\}\"

#set label \"Rouse relaxation\" at 1e5, 2e5 rotate by 35
#set label \"Crosslink dissociation\" at 1e1, 0.4e5 rotate by 35
#set label \"Reptation\" at 5e-3, 0.8e4 rotate by 35

#set arrow from 2e-3,80 to 2,8 lt 1 lw 2 lc rgb \"black\" front
#set label \"increasingly strong fluctuations\" at 3,6

set grid 

set key right bottom 

plot \"$reformatteddata\" u 3:4 w p pt 7 ps 2 lc rgb \"#DD4444\" title \"G'\",\
\"\"  u 1:2 w p pt 7 ps 2 lc rgb \"#44DD44\" title \"G''\",\
\"$bbxmodelfile\" u 1:3 w l lt 1 lw 3 lc rgb \"#AA4444\" title \"G'\",\
\"\"  u 1:2 w l lt 1 lw 3 lc rgb \"#44AA44\" title \"G''\"
"

printf "$plot" | gnuplot

