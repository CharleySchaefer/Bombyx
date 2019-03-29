#!/bin/bash

pth="../.."
MainC="demo_SF3D.c"
executable="demo_SF3D"




# Compile
echo "  Compiling $executable"
gcc -o $executable $MainC $pth/Mathematics/Fourier/Fourier.c $pth/Mathematics/Mathematics.c $pth/ReadTextFiles/ReadTextFiles.c $pth/StringOperations/StringOperations.c $pth/StructureFactor/StructureFactor.c -lm

# Run
echo "  Running ./$executable --file PolymerChain.pdb > PolymerChain_SFCF.out"
./$executable --file PolymerChain.pdb > PolymerChain_SFCF.out

# Plot (requires gnuplot installed)
echo "  Calling gnuplot:"
#echo "    Running ../../utils/gnuplot/plotMatrix.sh config100.dat"
#../../utils/gnuplot/plotMatrix.sh config100.dat

echo "    Running ./plotSF.sh PolymerChain_SFCF.out"
./plotSF.sh PolymerChain_SFCF.out

echo "    Running ./plotCF.sh PolymerChain_SFCF.out"
./plotCF.sh PolymerChain_SFCF.out

echo "  Done"








