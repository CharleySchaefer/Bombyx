#include "LatticeHCP.h"

int main(int argc, char *argv[])
{
  int siteID, siteID_test;
  int i,j,x,y,z;
  LATTICE_HCP *Lattice=make_lattice_hcp(3, 4, 5); // Nx=3, Ny=4, Nz=5
 
  printf("Dimensionality:              %d\n", Lattice->dim);
  printf("NX:                          %d\n", Lattice->Nx);
  printf("NY:                          %d\n", Lattice->Ny);
  printf("NZ:                          %d\n", Lattice->Nz);
  printf("Number of sites in total:    %d\n", Lattice->Nxyz);
  printf("Number of sites in xy plane: %d\n", Lattice->Nxy);
  printf("Periodic in x direction:     %s\n", (Lattice->is_periodic_x ? "yes" : "no"));
  printf("Periodic in y direction:     %s\n", (Lattice->is_periodic_y ? "yes" : "no"));
  printf("Periodic in z direction:     %s\n", (Lattice->is_periodic_z ? "yes" : "no"));

  printf("%4s: %3s, %3s, %3s\n", "site", "x", "y", "z");
  for (i=0; i<Lattice->Nxyz; i++) // Nxyz is the total number of sites
  {
    siteID=i;

    ind2coor_hcp(Lattice, siteID, &x, &y, &z);
    siteID_test = coor2ind_hcp(Lattice, x, y, z);

    printf("%4d: %3d, %3d, %3d\n", siteID, x, y, z);

    if(siteID_test!=siteID)
    {
      printf("Error: coor2ind() not inverse of ind2coor().\n"); exit(1);
    }
  }


  free_lattice_hcp(Lattice);

  return(0);

}
