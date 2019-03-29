#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../LatticeLIB.h"

// Install: 
//   First build libary in parent directory ..
//   Compile (current directory): gcc demo_get_angle.c -L.. -lLatticeLIB -lm

int main(int argc, char *argv[])
{

  double max_distance=4;
  LATTICE_CUBE *Lattice=make_lattice_cube(10,10,10);

  int siteID=0, nnID1=2, nnID2=10;
  float theta;
  get_angle_between_lattice_sites(Lattice, siteID, nnID1, nnID2, max_distance,  &theta);

  printf("Angle: %f pi\n", theta/M_PI);
  return(0);
}

