#include "LatticeHCP.h"


LATTICE_HCP *make_lattice_hcp(int Nx, int Ny, int Nz)
{
  int dim;
  LATTICE_HCP *Lattice;

  if(Nx<1 || Ny<1 || Nz<1)
    {printf("Error: unexpected dimensionality\n"); return Lattice;}

  if (Nz>1)
    dim=3;
  else if (Ny>1)
    dim=2;
  else if (Nx>1)
    dim=1;

  Lattice=(LATTICE_HCP *)malloc(sizeof(LATTICE_HCP));

  Lattice->is_periodic_x=1; // default
  Lattice->is_periodic_y=1; // default
  Lattice->is_periodic_z=1; // default
  Lattice->Nx   = Nx;
  Lattice->Ny   = Ny;
  Lattice->Nz   = Nz;
  Lattice->dim  = dim;
  Lattice->Nxy  = Lattice->Nx  * Lattice->Ny;
  Lattice->Nxyz = Lattice->Nxy * Lattice->Nz;

  Lattice->site=(int*)calloc(Lattice->Nxyz, sizeof(int)); // array with sites
  return Lattice;
}


int free_lattice_hcp(LATTICE_HCP *Lattice)
{
  if(Lattice!=NULL){
    if(Lattice->site!=NULL){
      free(Lattice->site);
      Lattice->site=NULL;
    }
    free(Lattice);
    Lattice=NULL;
  }
  return(1);
}
// TODO: function in standard C library?
int i1find( int target, int *array, int N) // Find value in array
{
  int i; 
  for(i=0; i<N; i++)
  {
    if(array[i]==target)
    {
      return(i); // on success
      break;
    }
  }
  return(-1);    // on fail
}
/*
// east  = x/i -direction
// north = y/j -direction
int NN_hex_periodic(int Nx, int Ny, int site_in, int *p_site_out, int direction)
{
  int site_out=*p_site_out;
  switch( direction )
  {
    case 0: // North
    {
      if(site_in%Ny==(Ny-1))     // boundary
        site_out=site_in-Ny+1;
      else
        site_out=site_in+1;
      break;
    }
    case 1: // South
    {
      if(site_in%Ny==0)
        site_out=site_in+Ny-1;
      else
        site_out=site_in-1;
      break;
    }
    case 2: // East
    {
      int Ntot=Nx*Ny;
      site_out=site_in+Ny;
      if(site_out>(Ntot-1))
        site_out-=Ntot;
      break;
    }
    case 3: // West
    {
      site_out=site_in-Ny;
      if(site_out<0)
        site_out+=Nx*Ny;
      break;
    }
    case 4: // North east
    {
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 0); site_in=site_out;
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 2);
      break;
    }
    case 5: // North west
    {
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 0); site_in=site_out;
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 3);
      break;
    }
    case 6: // South east
    {
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 1); site_in=site_out;
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 2);
      break;
    }
    case 7: // South west
    {
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 1); site_in=site_out;
      NN_square_periodic(Nx, Ny, site_in,  &site_out, 3);
      break;
    }
    default:
    {
      printf("Error: direction (%d) should be 0=N, 1=S, 2=E, 3=W, 4=NE, 5=NW, 6=SE, 6=SW.\n", direction); return(-1);
    }
  }
  *p_site_out=site_out;
  return(1);
}*/

int ind2coor_hex(LATTICE_HCP *Lattice, int site, int *x, int *y)
{
  *y=site%Lattice->Ny;
  *x=(site-(*y))/Lattice->Ny;
}

int ind2coor_hcp(LATTICE_HCP *Lattice, int site, int *x, int *y, int *z)
{
  int sitexy = site%Lattice->Nxy;
  *z=(site-sitexy)/Lattice->Nxy;
  *y=sitexy%Lattice->Ny;
  *x=(sitexy-(*y))/Lattice->Ny;
}

int coor2ind_hex(LATTICE_HCP *Lattice, int x, int y)
{
  int xx=(x%Lattice->Nx); if(xx<0) {xx+=Lattice->Nx;};
  int yy=(y%Lattice->Ny); if(yy<0) {yy+=Lattice->Ny;};
  return (xx)*Lattice->Ny+(yy);
}
int coor2ind_hcp(LATTICE_HCP *Lattice, int x, int y, int z)
{
  int xx=(x%Lattice->Nx); if(xx<0) {xx+=Lattice->Nx;};
  int yy=(y%Lattice->Ny); if(yy<0) {yy+=Lattice->Ny;};
  int zz=(z%Lattice->Nz); if(zz<0) {zz+=Lattice->Nz;};
  return zz*Lattice->Nxy + xx*Lattice->Ny + yy;
}


