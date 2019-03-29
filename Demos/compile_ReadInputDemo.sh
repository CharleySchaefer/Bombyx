#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer
#!/bin/bash

# Expected shell output:
#Compiling ./compile_ReadInputDemo.sh
#Running demo_ReadInput.o demodata/demodata_15_2.bbx
#Reading demodata/demodata_15_2.bbx
#File properties.
#              N: 5524
#              v: 1.300000e-28
#            phi: 0.228000
#              T: 288.000000
# FrequencyUnits: 1               # 0: Angular [rad/s]; 1: Hertz [1/s]
#  FirstDataLine: 13
#   LastDataLine: 27
#          Ndata: 15
#         Nlines: 30
#
#Reading data from file between lines 13 and 27
#Allocating memory.
#Finished allocating memory.
#Reading column data
#Input frequency in Hertz: convert to rad/s
#Sorting data in order of frequency (currently flipping data; TODO: mergesort()); Result:
#      #w[rad/s]       Gvisc[Pa]        w[rad/s]       Gelas[Pa]
#   ------------    ------------    ------------    ------------
#   6.283185e-01    1.256700e+03    6.283185e-01    4.187000e+02
#   8.859291e-01    1.624300e+03    8.859291e-01    6.965000e+02
#   1.250354e+00    2.013200e+03    1.250354e+00    1.110300e+03
#   1.771858e+00    2.344800e+03    1.771858e+00    1.663900e+03
#   2.500708e+00    2.613700e+03    2.500708e+00    2.291600e+03
#   3.531150e+00    2.834900e+03    3.531150e+00    3.004500e+03
#   4.982566e+00    2.974000e+03    4.982566e+00    3.723300e+03
#   7.037168e+00    3.125200e+03    7.037168e+00    4.447100e+03
#   9.933716e+00    3.165400e+03    9.933716e+00    5.182400e+03
#   1.403035e+01    3.253600e+03    1.403035e+01    5.949700e+03
#   1.981088e+01    3.275900e+03    1.981088e+01    6.685100e+03
#   2.797274e+01    3.254400e+03    2.797274e+01    7.512400e+03
#   3.950239e+01    3.131000e+03    3.950239e+01    8.287000e+03
#   5.578212e+01    2.875400e+03    5.578212e+01    8.933700e+03
#   7.877229e+01    2.636000e+03    7.877229e+01    9.748800e+03

lib=../lib
src=../src
executable=demo_ReadInput.o
inputfile=demodata/demodata_15_2.bbx

echo "Compiling $0"
gcc -o $executable demo_ReadInput.c $src/Import/ReadBBXdata.c -L$lib/ZiltoidLIB -lZiltoidLIB -lm

echo "Running $executable $inputfile"
./$executable $inputfile
