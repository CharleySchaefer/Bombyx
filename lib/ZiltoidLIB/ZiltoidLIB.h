/*
   ZiltoidLIB: C library for physical modelling and simulation.
   Copyright 2018, Charley Schaefer (charley.schaefer@york.ac.uk)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef ZiltoidLIB_LOADED
  #define ZiltoidLIB_LOADED 1

  #include <stdio.h>
  #include <stdlib.h>

  #include <string.h> // used by ReadTextFiles
  #include <math.h>

  #ifndef MAX_LINE_WIDTH
    #define MAX_LINE_WIDTH 25000
  #endif
  #ifndef MAX_STR_L
    #define  MAX_STR_L 200
  #endif

  #include "kMC/VSSM.h"
  #include "Mathematics/Mathematics.h"
  #include "LatticeLIB/LatticeLIB.h"
  #include "NumericalMethods/NumericalMethods.h"
  #include "ReadTextFiles/ReadTextFiles.h"
  #include "StringOperations/StringOperations.h"

  #include "Applications/StructureFactor/StructureFactor.h"
  #include "Applications/PrincipleMomentsOfInertia/PrincipleMomentsOfInertia.h"
#endif
