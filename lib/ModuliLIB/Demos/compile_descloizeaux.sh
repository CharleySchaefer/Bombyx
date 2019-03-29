#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer
#!/bin/bash

lib=../..

debug=0;
arg=""
if [ $# -gt 0 ]; then
  if [ $1 == "--debug" ]; then
    arg=-g
    debug=1
  fi
fi

echo "Compiling"
gcc -o demo_descloizeaux.o demo_descloizeaux.c -L$lib/ModuliLIB -lModuliLIB -L$lib/ZiltoidLIB -lZiltoidLIB -lm $arg

echo "Running"
if [ $debug -eq 1 ]; then
  valgrind ./demo_descloizeaux.o
else
  ./demo_descloizeaux.o
fi

echo "Plotting g function"
plot="set terminal pngcairo enhanced
set output \"g_descloizeauxplot.png\"

set log xy
set xlabel \"x\"
set ylabel \"g(x)\"

set key left top

plot \"g_descloizeaux.out\" u 1:2 w p pt 6 ps 1 lc rgb \"black\" title \"g(x)\",\
sqrt(3.1415*x) w l lt 1 lw 1 lc rgb \"red\" title \"sqrt(pi*x)\",\
3.1415**2/6 w l lt 1 lw 1 lc rgb \"green\" title \"pi^2/6\"
"
printf "$plot" | gnuplot



echo "Plotting F function"
plot="set terminal pngcairo enhanced
set output \"F_descloizeauxplot.png\"

set log xy
set xlabel \"U\"
set ylabel \"F(U)\"

set key left bottom

plot \"F_descloizeaux.out\" u 1:2 w p pt 6 ps 1 lc rgb \"black\" title \"F(U)\",\
1 w l lt 1 lw 1 lc rgb \"red\" title \"1\",\
exp(-x) w l lt 1 lw 1 lc rgb \"green\" title \"exp(-U)\"
"
printf "$plot" | gnuplot

