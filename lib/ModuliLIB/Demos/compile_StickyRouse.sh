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

executable=demo_StickyRouse.o

echo "Compiling"
if gcc -o $executable demo_StickyRouse.c -L$lib/ModuliLIB -lModuliLIB -L$lib/ZiltoidLIB -lZiltoidLIB -lm
then
  echo "Compiled $executable"
else
  echo "Error: Failed to compile $executable - exiting."
  exit 1
fi

#gcc -o $executable demo_StickyRouse.c -lm

echo "Running"
if [ $debug -eq 1 ]; then
  valgrind ./$executable > StickyRouse.out
else
  ./$executable > StickyRouse.out
fi


echo "Plotting"
plot="set terminal pngcairo enhanced
set output \"StickyRouse.png\"

set log xy
set xlabel \"omega\"
set ylabel \"G', G''\"
#set yrange [1e-5:10]
unset key 

plot \"StickyRouse.out\" u 1:2 w p ps 2 pt 6 lc rgb \"red\",\
\"\"  u 1:3 w p ps 2 pt 6 lc rgb \"green\",\
\"\"  u 1:4 w l lt 1 lw 2 lc rgb \"red\",\
\"\"  u 1:5 w l lt 1 lw 2 lc rgb \"green\",\
"

printf "$plot" | gnuplot
