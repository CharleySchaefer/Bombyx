#   ReadTextFiles is part of ZiltoidLIB.
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
includeReadTextFiles=1
#===============================

echo "  >COMPILING ZiltoidRead"
mkdir -p build
pushd build >/dev/null


if [ $includeReadTextFiles -eq 1 ]; then
  if gcc -fPIC -c ../ReadTextFiles.c ; then
    echo "  ReadTextFiles compiled."
   else
    echo "Error: Failed to compile ReadTextFiles."
    exit 1
  fi
fi

if ar rcs libZiltoidRead.a ReadTextFiles.o
then
  echo "  libZiltoidRead.a created."
else
  echo "Error: failed to create library."
fi

popd >/dev/null
echo "  >FINISHED COMPILING ZiltoidRead"

