#!/bin/bash

pth="../../.."
MainC="Demo_StructureFactorAndCorrelationFunction.c"
executable="StructureFactorAndCorrelationFunction.o"

# Compile
if gcc -o $executable $MainC $pth/Applications/StructureFactor/StructureFactor.c -L$pth -lZiltoidLIB -lm ; then
  echo "  $executable compiled."
else
  echo "  compilation $executable failed."
fi

