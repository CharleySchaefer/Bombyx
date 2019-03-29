#!/bin/bash

pth="../../.."
MainC="Demo_StructureFactorAndCorrelationFunction.c"
executable="StructureFactorAndCorrelationFunction.o"

# Compile
echo "  Compiling $executable"
./compile_StructureFactor.sh

inputfile=psi_example.dat
lengthscale=10.0

# Run
echo "  Running ./$executable"
./$executable --file $inputfile --stretch-matrix --L $lengthscale > psi_example.out

# Plot (requires gnuplot installed)
echo "  Calling gnuplot:"
echo "    Running ../../utils/gnuplot/plotMatrix.sh psi_example.dat"
$pth/utils/gnuplot/plotMatrix.sh $inputfile

echo "    Running ./plotSF.sh psi_example.out"
./plotSF.sh psi_example.out

echo "    Running ./plotCF.sh psi_example.out"
./plotCF.sh psi_example.out

echo "  Done"








