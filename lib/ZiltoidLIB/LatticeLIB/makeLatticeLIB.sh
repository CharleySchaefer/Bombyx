#   LatticeLIB is part of ZiltoidLIB.
#   ZiltoidLIB: C library for physical modelling and simulation.
#   Copyright 2018, Charley Schaefer (charley.schaefer@york.ac.uk)
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#!/bin/bash


#================================================================
echo " "
echo "  >BUILDING LatticeLIB"

rm build -rf
mkdir -p build

pushd build >/dev/null

if gcc -fPIC -c ../LatticeCube/LatticeCube.c ; then
  echo "  LatticeCube compiled."
 else
  echo "Error: Failed to compile LatticeCube."
  exit 1
fi

if gcc -fPIC -c ../LatticeHCP/LatticeHCP.c ; then
  echo "  LatticeHCP compiled."
 else
  echo "Error: Failed to compile LatticeHCP."
  exit 1
fi

if gcc -fPIC -c ../NN/NN.c ; then
  echo "  NN compiled."
 else
  echo "Error: Failed to compile NN."
  exit 1
fi

if ar rcs libLatticeLIB.a LatticeCube.o NN.o
then
  echo "  libLatticeLIB.a created."
else
  echo "Error: failed to create libLatticeLIB.a."
fi


popd >/dev/null


echo "  >FINISHED COMPILING LatticeLIB"
