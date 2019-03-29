#include "LatticeCube.h"
int main(int argc, char *argv[])
{
  int i,j,Nx=4,Ny=3;
  int lattice[12]={1,2,3,4,5,6,7,8,9, 2, 3, 4};
  LATTICE_CUBE *Lattice=make_lattice_cube(Nx, Ny, 1);
  for(i=0; i<12; i++)
    Lattice->site[i]=lattice[i];

  printf("N: %d\n", Nx);

  printf("Matrix:\n");
  for (j=0; j<Ny; j++)
  {
    for (i=0; i<Nx; i++)
      printf(" %d", Lattice->site[i*Ny+j]);
    putchar('\n');
  }
  putchar('\n');
  printf("Nearest neighbours:\n");
  for (i=0; i<12; i++)
  {
    printf("NN of %d:", lattice[i]);
    NN_square_periodic(Nx,Ny, i, &j, 0);
    printf("   N: %d", lattice[j]);
    NN_square_periodic(Nx,Ny, i, &j, 1);
    printf("   S: %d", lattice[j]);
    NN_square_periodic(Nx,Ny, i, &j, 2);
    printf("   E: %d", lattice[j]);
    NN_square_periodic(Nx,Ny, i, &j, 3);
    printf("   W: %d", lattice[j]);
    putchar('\n');
  }


  return(1);
}
