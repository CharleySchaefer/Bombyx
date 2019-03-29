#include "LatticeCube.h"

int main(int argc, char *argv[])
{

  int i,j,x,y;
  LATTICE_CUBE *Lattice=make_lattice_cube(10, 10, 1);
 
  i=0;
  ind2coor_square(   Lattice, i, &x, &y);
  printf("   %4d: %3d %3d\n", i, x, y);

  NN_square_periodic(Lattice->Nx,Lattice->Ny,  i, &j, 0);
  ind2coor_square(   Lattice, j, &x, &y);
  printf("N: %4d: %3d %3d\n", j, x, y );

  NN_square_periodic(Lattice->Nx,Lattice->Ny,  i, &j, 1);
  ind2coor_square(   Lattice, j, &x, &y);
  printf("S: %4d: %3d %3d\n", j, x, y );

  NN_square_periodic(Lattice->Nx,Lattice->Ny,  i, &j, 2);
  ind2coor_square(   Lattice, j, &x, &y);
  printf("E: %4d: %3d %3d\n", j, x, y );

  NN_square_periodic(Lattice->Nx,Lattice->Ny,  i, &j, 3);
  ind2coor_square(   Lattice, j, &x, &y);
  printf("W: %4d: %3d %3d\n", j, x, y );

  return(0);

}
