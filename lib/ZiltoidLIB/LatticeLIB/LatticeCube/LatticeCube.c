#include "LatticeCube.h"


LATTICE_CUBE *make_lattice_cube(int Nx, int Ny, int Nz)
{
  int dim;
  LATTICE_CUBE *Lattice;

  if(Nx<1 || Ny<1 || Nz<1)
    {printf("Error: unexpected dimensionality\n"); return Lattice;}

  if (Nz>1)
    dim=3;
  else if (Ny>1)
    dim=2;
  else if (Nx>1)
    dim=1;

  Lattice=(LATTICE_CUBE *)malloc(sizeof(LATTICE_CUBE));

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


int free_lattice_cube(LATTICE_CUBE *Lattice)
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


// east  = x/i -direction
// north = y/j -direction
int NN_square_periodic(int Nx, int Ny, int site_in, int *p_site_out, int direction)
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
      printf("Error: direction (%d) should be 0=N, 1=S, 2=E, 3=W, 4=NE, 5=NW, 6=SE, 7=SW.\n", direction); return(-1);
    }
  }
  *p_site_out=site_out;
  return(1);
}

int ind2coor_square(LATTICE_CUBE *Lattice, int site, int *x, int *y)
{
  *y=site%Lattice->Ny;
  *x=(site-(*y))/Lattice->Ny;
}

int ind2coor_cube(LATTICE_CUBE *Lattice, int site, int *x, int *y, int *z)
{
  int sitexy = site%Lattice->Nxy;
  *z=(site-sitexy)/Lattice->Nxy;
  *y=sitexy%Lattice->Ny;
  *x=(sitexy-(*y))/Lattice->Ny;
}

int coor2ind_square(LATTICE_CUBE *Lattice, int x, int y)
{
  int xx=(x%Lattice->Nx); if(xx<0) {xx+=Lattice->Nx;};
  int yy=(y%Lattice->Ny); if(yy<0) {yy+=Lattice->Ny;};
  return (xx)*Lattice->Ny+(yy);
}
int coor2ind_cube(LATTICE_CUBE *Lattice, int x, int y, int z)
{
  int xx=(x%Lattice->Nx); if(xx<0) {xx+=Lattice->Nx;};
  int yy=(y%Lattice->Ny); if(yy<0) {yy+=Lattice->Ny;};
  int zz=(z%Lattice->Nz); if(zz<0) {zz+=Lattice->Nz;};
  return zz*Lattice->Nxy + xx*Lattice->Ny + yy;
}

/*
   Find sites of a cubic lattice within 
   a shell around coordinate R=(X,Y,Z) such that

     Rinner <= | r_site -  R | <= Router

   NOTE: The coordinate R is the position with respect
         to a site at coordinate (i,j,k)=(0,0,0)

*/
int get_sites_in_sphere_shell(double X, double Y, double Z, double R2inner, double R2outer, int *xout, int *yout, int *zout, int *Nsites)
{
  int i,j,k,x,y,z;
  double R=sqrt(R2outer), x2, y2, z2, r2;
  int    xmin, xmax, ymin, ymax, zmin, zmax, count;
  xmin= (int)(X-R);
  xmax= (int)(X+R);
  ymin= (int)(Y-R);
  ymax= (int)(Y+R);
  zmin= (int)(Z-R);
  zmax= (int)(Z+R);

  (*Nsites)=0;
  for(i=xmin; i<=xmax; i++)
  {
    x2 = (X-i)*(X-i);
    for(j=ymin; j<=ymax; j++)
    {
      y2 = (Y-j)*(Y-j);
      for(k=zmin; k<=zmax; k++)
      {
        z2 = (Z-k)*(Z-k);
        r2 = x2 + y2 + z2;
        if(R2inner <= r2 && r2 <= R2outer)
        {
          xout[(*Nsites)] = i;
          yout[(*Nsites)] = j;
          zout[(*Nsites)] = k;
          (*Nsites)++;   
        }
      }
    }
  }
}

int get_sites_in_spheres(int Nouter, double *Xrow, double *Yrow, double *Zrow, double R2outer,  int *xout, int *yout, int *zout, int *Nsites)
{
  double X,Y,Z;
  int i,j,k,iter,x,y,z, check;
  double R=sqrt(R2outer), x2, y2, z2, r2;
  int    xmin, xmax, ymin, ymax, zmin, zmax, count;

  X=Xrow[0];Y=Yrow[0];Z=Zrow[0];
  for(i=1; i<Nouter; i++)
  {
    X=(X<Xrow[i] ? X : Xrow[i]);
    Y=(Y<Yrow[i] ? Y : Yrow[i]);
    Z=(Z<Zrow[i] ? Z : Zrow[i]);
  }
  xmin= (int)(X-R);
  ymin= (int)(Y-R);
  zmin= (int)(Z-R);

  X=Xrow[0];Y=Yrow[0];Z=Zrow[0];
  for(i=1; i<Nouter; i++)
  {
    X=(X>Xrow[i] ? X : Xrow[i]);
    Y=(Y>Yrow[i] ? Y : Yrow[i]);
    Z=(Z>Zrow[i] ? Z : Zrow[i]);
  }
  xmax= (int)(X+R);
  ymax= (int)(Y+R);
  zmax= (int)(Z+R);

  (*Nsites)=0;
  for(i=xmin; i<=xmax; i++)
  {
    for(j=ymin; j<=ymax; j++)
    {
      for(k=zmin; k<=zmax; k++)
      {
        check=0;
        for(iter=0; iter<Nouter; iter++)
        {
          x2 = (Xrow[iter]-i)*(Xrow[iter]-i);
          y2 = (Yrow[iter]-j)*(Yrow[iter]-j);
          z2 = (Zrow[iter]-k)*(Zrow[iter]-k);
          r2 = x2 + y2 + z2 ;
          if(r2 <= R2outer)
          {check=1; break;}
        }

        if( check)
        {
          xout[(*Nsites)] = i;
          yout[(*Nsites)] = j;
          zout[(*Nsites)] = k;
          (*Nsites)++;   
        }
      }
    }
  }
}

int is_cube_vacant( LATTICE_CUBE *Lattice, int x0, int y0, int z0)
{
  int i,j,k,x,y,z,siteID;
  for (i=0; i<=1; i++)
  {
    x=x0+i;
    for (j=0; j<=1; j++)
    {
      y=y0+j;
      for (k=0; k<=1; k++)
      {
        z=z0+k;
        siteID=coor2ind_cube(Lattice, x, y, z);
        if( Lattice->site[siteID] != -1 )
        {
          return(0);
        }
      }
    }
  }
  return(1);
}


int get_cosangle_between_lattice_coordinates(LATTICE_CUBE *Lattice, int x0, int y0, int z0, int xm, int ym, int zm, int xp, int yp, int zp, double max_distance, float *cosangle)
{
  int Nx=Lattice->Nx, Ny=Lattice->Ny, Nz=Lattice->Nz;

  if(xm<x0-max_distance)
    xm+=Nx;
  else if (xm>x0+max_distance)
    xm-=Nx;

  if(ym<y0-max_distance)
    ym+=Ny;
  else if (ym>y0+max_distance)
    ym-=Ny;

  if(zm<z0-max_distance)
    zm+=Nz;
  else if (zm>z0+max_distance)
    zm-=Nz;

  if(xp<x0-max_distance)
    xp+=Nx;
  else if (xp>x0+max_distance)
    xp-=Nx;

  if(yp<y0-max_distance)
    yp+=Ny;
  else if (yp>y0+max_distance)
    yp-=Ny;

  if(zp<z0-max_distance)
    zp+=Nz;
  else if (zp>z0+max_distance)
    zp-=Nz;

  int ux=xm-x0, uy=ym-y0, uz=zm-z0;
  int vx=xp-x0, vy=yp-y0, vz=zp-z0;

  //----------------------------------
  // FOR TESTING
/*  if( (ux>0?ux:-ux)>max_distance)
    {printf("ERROR!\n");return(-1);}
  if( (uy>0?uy:-uy)>max_distance)
    {printf("ERROR!\n");return(-1);}
  if( (uz>0?uz:-uz)>max_distance)
    {printf("ERROR!\n");return(-1);}
  if( (vx>0?vx:-vx)>max_distance)
    {printf("ERROR!\n");return(-1);}
  if( (vy>0?vy:-vy)>max_distance)
    {printf("ERROR!\n");return(-1);}
  if( (vz>0?vz:-vz)>max_distance)
    {printf("ERROR!\n");return(-1);}*/
  //----------------------------------

  // dot product
   int u_dot_v =  ux*vx + uy*vy + uz*vz;
   int norm_usq = ux*ux + uy*uy + uz*uz;
   int norm_vsq = vx*vx + vy*vy + vz*vz;
  *cosangle = (  (float)u_dot_v/sqrt(norm_usq*norm_vsq)  );
  //*angle = acos(  (float)u_dot_v/sqrt(norm_usq*norm_vsq)  );
/*
  if(isnan(*angle))
  {
    printf("Error: nan encountered\n", u_dot_v, norm_usq*norm_vsq); exit(1);
  }*/

  return(1);
}

int get_cosangle_between_lattice_sites(LATTICE_CUBE *Lattice, int siteID, int nnID1, int nnID2, double max_distance, float *cosangle)
{
  int x0,y0,z0,xm,ym,zm,xp,yp,zp;
  int Nx=Lattice->Nx, Ny=Lattice->Ny, Nz=Lattice->Nz;
  ind2coor_cube(Lattice, siteID, &x0, &y0, &z0);
  ind2coor_cube(Lattice,  nnID1, &xm, &ym, &zm);
  ind2coor_cube(Lattice,  nnID2, &xp, &yp, &zp);
  get_cosangle_between_lattice_coordinates(Lattice, x0, y0, z0, xm, ym, zm, xp, yp, zp, max_distance, cosangle);

  return(1);
}



