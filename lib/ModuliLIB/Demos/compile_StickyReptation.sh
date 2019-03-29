#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer
#!/bin/bash 

# expected run time (by running "time ./compile_StickyReptation"):
# real	0m1.403s
# user	0m1.360s
# sys	0m0.028s

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

executable=demo_StickyReptation.o
if gcc -o $executable demo_StickyReptation.c -L$lib/ModuliLIB -lModuliLIB -L$lib/ZiltoidLIB -lZiltoidLIB -lm
then
  echo "Compiled $executable"
else
  echo "Error: Failed to compile $executable - exiting"
  exit 1  
fi

echo "Running"
if [ $debug -eq 1 ]; then
  valgrind ./$executable > demo_StickyReptation.out
else
  ./$executable > demo_StickyReptation.out
fi


echo "Plotting StickyReptation"
plot="set terminal pngcairo enhanced
set output \"StickyReptation.png\"

set log xy
set xlabel \"omega [rad/s]\"
set xrange [1e-4: 1e11]
set xtics 1e-6, 1e3, 1e12
set format x \"10^\{%%T\}\"

set ylabel \"G', G'' [Pa s]\"
set yrange [1e-1:1e7]
set ytics 1e-3, 1e3, 1e9
set format y \"10^\{%%T\}\"

set label \"Rouse relaxation\" at 1e5, 2e5 rotate by 35
set label \"Crosslink dissociation\" at 1e1, 0.4e5 rotate by 35
set label \"Reptation\" at 5e-3, 0.8e4 rotate by 35

set arrow from 2e-3,80 to 2,8 lt 1 lw 2 lc rgb \"black\" front
set label \"increasingly strong fluctuations\" at 3,6

set grid 

set key right bottom 

plot \"demo_StickyReptation.out\" u 1:2 w l lt 1 lw 4 lc rgb \"#AA4444\" title \"G'\",\
\"\"  u 1:3 w l lt 1 lw 4 lc rgb \"#44AA44\" title \"G''\",\
"

for i in `seq 2 9`
do
  colx=$(( ( $i-1)*3 +1 ))
  coly1=$(( ( $i-1)*3 + 2 ))
  coly2=$(( ( $i-1)*3 + 3 ))
  plot="$plot \"\" u $colx:$coly1 w l lt 1 lw 2 lc rgb \"#AA4444\" notitle,\
\"\"  u $colx:$coly2 w l lt 1 lw 2 lc rgb \"#44AA44\" notitle,\
"
done
for i in `seq 10 10`
do
  colx=$(( ( $i-1)*3 +1 ))
  coly1=$(( ( $i-1)*3 + 2 ))
  coly2=$(( ( $i-1)*3 + 3 ))
  plot="$plot \"\" u $colx:$coly1 w l lt 1 lw 4 lc rgb \"#AA4444\" notitle,\
\"\"  u $colx:$coly2 w l lt 1 lw 4 lc rgb \"#44AA44\" notitle,\
"
done

printf "$plot"
printf "$plot" | gnuplot

echo "The output data in \"demo_StickyReptation.out\" should be identical to that in demodata/StickyReptation.txt."
echo "Checking differences..."
  diff demo_StickyReptation.out demodata/StickyReptation.txt
echo "...done."


