#   Applications is part of ZiltoidLIB.
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

#===============================
# CONFIGURATION
cnf=1
ModuleName=ZiltoidApps
#===============================

echo "  >COMPILING $ModuleName"
mkdir -p bin
pushd bin >/dev/null



if [ $cnf -eq 1 ]; then
  if gcc -fPIC -c ../PrincipleMomentsOfInertia/PrincipleMomentsOfInertia.c ; then
    echo "  PrincipleMomentsOfInertia compiled."
   else
    echo "Error: Failed to compile PrincipleMomentsOfInertia."
    exit 1
  fi
fi

pushd ../ColumnStats > /dev/null
  ./compile_ColumnStats.sh
popd > /dev/null
mv ../ColumnStats/getColumnStats.o .


pushd ../StructureFactor/Demo > /dev/null
  ./compile_StructureFactor.sh  
popd > /dev/null
mv ../StructureFactor/Demo/StructureFactorAndCorrelationFunction.o .


if ar rcs lib$ModuleName.a PrincipleMomentsOfInertia.o 
then
  echo "  lib$ModuleName.a created."
else
  echo "Error: failed to create $ModuleName module."
fi



popd >/dev/null
echo "  >FINISHED COMPILING $ModuleName"

