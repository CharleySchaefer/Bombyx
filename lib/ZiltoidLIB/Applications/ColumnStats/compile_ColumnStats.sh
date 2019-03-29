#!/bin/bash

lib=../..
executable=getColumnStats.o

if gcc -o $executable get_column_stats_from_file.c -L$lib -lZiltoidLIB -lm ; then
  echo "  $executable compiled."
else
  echo "  compilation $executable failed."
fi
