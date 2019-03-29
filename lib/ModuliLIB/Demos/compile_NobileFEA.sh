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

executable=demo_NobileFEA.o

echo "Compiling"
if gcc -o $executable demo_NobileFEA.c -L$lib/ModuliLIB -lModuliLIB -L$lib/ZiltoidLIB -lZiltoidLIB -lm
then
  echo "Compiled $executable"
else
  echo "Error: Failed to compile $executable - exiting."
  exit 1
fi


echo "Running"
if [ $debug -eq 1 ]; then
  valgrind ./$executable > NobileFEA.out
else
  ./$executable > NobileFEA.out
fi


echo "Plotting NobileFEA"
plot="set terminal pngcairo enhanced
set output \"NobileFEA.png\"

set log xy
set xlabel \"omega\"
set ylabel \"G', G''\"
set yrange [1e-5:10]
unset key 

plot \"NobileFEA.out\" u 1:2 w p ps 2 pt 6 lc rgb \"red\",\
\"\"  u 1:3 w p ps 2 pt 6 lc rgb \"green\"
"
printf "$plot" | gnuplot

echo "Plotting convergence_NobileFEA"
plot="set terminal pngcairo enhanced
set output \"convergence_NobileFEA.png\"

set log xy
set xlabel \"tauL\"
set ylabel \"G', G'' at tauL and tauU target\"
#set yrange [1e-5:10]
unset key 

plot \"convergence_NobileFEA.out\" u 1:2 w p ps 2 pt 6 lc rgb \"red\",\
\"\"  u 1:3 w p ps 2 pt 6 lc rgb \"green\",\
\"\"  u 1:5 w p ps 2 pt 8 lc rgb \"red\",\
\"\"  u 1:6 w p ps 2 pt 8 lc rgb \"green\"
"
printf "$plot" | gnuplot
