#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer
#!/bin/bash

#===============================
# CONFIGURATION
includeFourier=1
includePolynomial=1
includeRandomNumbers=1
includeLinearAlgebra=1
#===============================

echo "COMPILING ModuliLIB"
rm -rf build
mkdir -p build
pushd build >/dev/null


if gcc -fPIC -c ../DoubleReptation/doublereptation.c ; then
  echo "  DoubleReptation compiled."
else
  echo "Error: Failed to compile DoubleReptation."
  exit 1
fi

if gcc -fPIC -c ../MaxwellModel/MaxwellModel.c ; then
  echo "  MaxwellModel compiled."
else
  echo "Error: Failed to compile MaxwellModel."
  exit 1
fi

if gcc -fPIC -c ../StickyReptation/StickyReptation.c ; then
  echo "  StickyReptation compiled."
else
  echo "Error: Failed to compile StickyReptation."
  exit 1
fi

if gcc -fPIC -c ../StickyRouse/StickyRouse.c ; then
  echo "  StickyRouse compiled."
else
  echo "Error: Failed to compile StickyRouse."
  exit 1
fi

if gcc -fPIC -c ../NobileFEA/NobileFEA.c ; then
  echo "  NobileFEA compiled."
else
  echo "Error: Failed to compile NobileFEA."
  exit 1
fi

echo "  Object files created:"
for file in `find *.o`
do
  echo "    $file"
done


if ar rcs libModuliLIB.a *.o
then
  echo "  libModuliLIB.a created."
else
  echo "Error: failed to create ModuliLIB library."
fi

popd >/dev/null
cp build/libModuliLIB.a .
echo "FINISHED COMPILING ModuliLIB.a"

