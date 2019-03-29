#!/bin/bash

lib=../..
executable=getColumnStats.o
echo "Compile"
gcc -o $executable get_column_stats_from_file.c -L$lib -lZiltoidLIB -lm

echo "Run"
./$executable --file demodata.txt --column 1 > ColumnStats.out

echo "Plot"
gnuplot plotColumnStats.plt
