#include "NN.h"

NN_STRUCT *new_nn_struct(int N, int dimensionality)
{
  NN_STRUCT *NN=(NN_STRUCT*)malloc(sizeof(NN_STRUCT));

  NN->N=N;
  NN->dim=dimensionality;

  NN->dx=(int*)malloc(NN->N*sizeof(int));
  if (NN->dim>1)
  {
    NN->dy=(int*)malloc(NN->N*sizeof(int));
    if(NN->dim>2)
      NN->dz=(int*)malloc(NN->N*sizeof(int));
  }
  return NN;
}

NN_STRUCT *copy_nn_struct(NN_STRUCT *NNin)
{
  int i;
  NN_STRUCT *NN=(NN_STRUCT*)malloc(sizeof(NN_STRUCT));

  NN->N            = NNin->N;
  NN->dim          = NNin->dim;
  NN->central_site = NN->central_site;

  NN->Nx=NNin->Nx; NN->xmin=NNin->xmin; NN->xmax=NNin->xmax;
  NN->dx=(int*)malloc(NN->N*sizeof(int));
  if (NN->dim>1)
  {
    NN->Ny=NNin->Ny; NN->ymin=NNin->ymin; NN->ymax=NNin->ymax;
    NN->dy=(int*)malloc(NN->N*sizeof(int));
    if(NN->dim>2)
    {
      NN->Nz=NNin->Nz; NN->zmin=NNin->zmin; NN->zmax=NNin->zmax;
      NN->dz=(int*)malloc(NN->N*sizeof(int));
    }
  }

  switch(NN->dim)
  {
    case 1:
    {
      for(i=0; i<NN->N; i++)
        NN->dx[i]=NNin->dx[i];
      break;
    }
    case 2:
    {
      for(i=0; i<NN->N; i++)
      {
        NN->dx[i]=NNin->dx[i];
        NN->dy[i]=NNin->dy[i];
      }
      break;
    }
    case 3:
    {
      for(i=0; i<NN->N; i++)
      {
        NN->dx[i]=NNin->dx[i];
        NN->dy[i]=NNin->dy[i];
        NN->dz[i]=NNin->dz[i];
      }
      break;
    }
  }

  return NN;
}

void free_nn_struct(NN_STRUCT *NN)
{
  free(NN->dx);
  if(NN->dim>1)
  {
    free(NN->dy);
    if(NN->dim>2)
      free(NN->dz);
  }
  free(NN);
}
int nn_analyse(NN_STRUCT *NN)
{
  int i;
  NN->xmin=NN->dx[ 0];
  NN->xmax=NN->dx[ 0];


  for(i=1; i<NN->N; i++)
  {
     NN->xmin=(NN->dx[i]<NN->xmin ? NN->dx[i] : NN->xmin);
     NN->xmax=(NN->dx[i]>NN->xmax ? NN->dx[i] : NN->xmax);
  }
  NN->Nx=NN->xmax-NN->xmin+1;

  if (NN->dim>1)
  {
    NN->ymin=NN->dy[ 0];
    NN->ymax=NN->dy[ 0];
    for(i=1; i<NN->N; i++)
    {
      NN->ymin=(NN->dy[i]<NN->ymin ? NN->dy[i] : NN->ymin);
      NN->ymax=(NN->dy[i]>NN->ymax ? NN->dy[i] : NN->ymax);
    }
    NN->Ny=NN->ymax-NN->ymin+1;

    if (NN->dim>2)
    {
      NN->zmin=NN->dz[ 0];
      NN->zmax=NN->dz[ 0];
      for(i=1; i<NN->N; i++)
      {
        NN->zmin=(NN->dz[i]<NN->zmin ? NN->dz[i] : NN->zmin);
        NN->zmax=(NN->dz[i]>NN->zmax ? NN->dz[i] : NN->zmax);
      }
      NN->Nz=NN->zmax-NN->zmin+1;
    }

  }

  NN->central_site=-1;
  switch (NN->dim)
  {
    case 1:
    {
      for(i=0; i<NN->N; i++)
        {if( NN->dx[i]==0 ) {NN->central_site=i;}}
      break;
    }
    case 2:
    {
      for(i=0; i<NN->N; i++)
        {if( NN->dx[i]==0 && NN->dy[i]==0 ) {NN->central_site=i;}}
      break;
    }
    case 3:
    {
      for(i=0; i<NN->N; i++)
        {if( NN->dx[i]==0 ) {NN->central_site=i;}}
      break;
    }
  }
  if(NN->central_site==-1)
    {return(-1);}  

  return(1);
}

// ibuff should be of length NN->Nx*max(1,NN->Ny)*max(1,NN->Nz)
void nn_print(NN_STRUCT *NN, int *ibuff)
{
  int i,j;
  int *matrix=ibuff;

  for(i=0; i<NN->Nx*NN->Ny; i++)
    matrix[i]=-1;

  for (i=0; i<NN->N; i++)
    matrix[ (NN->dx[i]-NN->xmin)*NN->Ny + (NN->dy[i]-NN->ymin) ] = i;

  for(j=0; j<NN->Ny; j++)
  {
    for(i=0; i<NN->Nx; i++)
    {
      if(matrix[i*NN->Ny + j]==-1)
        printf(" %2s", "x");
      else 
        printf(" %2d", matrix[i*NN->Ny + j]);
    }
    putchar('\n');
  }
}

void nn_translate_x(NN_STRUCT *NN, int ddx)
{
  int i,tmp;

  NN->xmin+=ddx;
  NN->xmax+=ddx;

  for (i=0; i<NN->N; i++)
    NN->dx[i] += ddx;
}

void nn_translate_y(NN_STRUCT *NN, int ddy)
{
  int i,tmp;

  NN->xmin+=ddy;
  NN->xmax+=ddy;

  for (i=0; i<NN->N; i++)
    NN->dy[i] += ddy;
}

void nn_translate_z(NN_STRUCT *NN, int ddz)
{
  int i,tmp;

  NN->xmin+=ddz;
  NN->xmax+=ddz;

  for (i=0; i<NN->N; i++)
    NN->dz[i] += ddz;
}

void nn_reflect_x(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->xmin; NN->xmin= NN->xmax; NN->xmax=tmp;

  for (i=0; i<NN->N; i++)
    NN->dx[i]=-NN->dx[i];
}

void nn_reflect_y(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->ymin; NN->ymin= NN->ymax; NN->ymax=tmp;

  for (i=0; i<NN->N; i++)
    NN->dy[i]=-NN->dy[i];
}

void nn_reflect_z(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->zmin; NN->zmin= NN->zmax; NN->zmax=tmp;

  for (i=0; i<NN->N; i++)
    NN->dz[i]=-NN->dz[i];
}

// ROTATIONS AROUND Z AXIS 
void nn_rotatez_right(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->Nx; NN->Nx=NN->Ny; NN->Ny=tmp;

  tmp=NN->xmin;
  NN->xmin= NN->ymin;
  NN->ymin=-NN->xmax;
  NN->xmax= NN->ymax;
  NN->ymax=-tmp;

  for (i=0; i<NN->N; i++)
    {tmp=NN->dx[i]; NN->dx[i]=NN->dy[i]; NN->dy[i]=-tmp;}
}

void nn_rotatez_180(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->xmin; NN->xmin=-NN->xmax; NN->xmax=tmp;
  tmp=NN->ymin; NN->ymin=-NN->ymax; NN->ymax=tmp;

  for (i=0; i<NN->N; i++)
    {NN->dx[i]=-NN->dx[i]; NN->dy[i]=-NN->dy[i];}
}

void nn_rotatez_left(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->Nx; NN->Nx=NN->Ny; NN->Ny=tmp;

  tmp=NN->ymax;
  NN->ymax= NN->xmax;
  NN->xmax=-NN->ymin;
  NN->ymin= NN->xmin;
  NN->xmin=-tmp;

  for (i=0; i<NN->N; i++)
    {tmp=NN->dx[i]; NN->dx[i]=-NN->dy[i]; NN->dy[i]=tmp;}
}

// ROTATIONS AROUND X AXIS 
void nn_rotatex_left(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->Nz; NN->Nz=NN->Ny; NN->Ny=tmp;

  tmp=NN->zmin;
  NN->zmin= NN->ymin;
  NN->ymin=-NN->zmax;
  NN->zmax= NN->ymax;
  NN->ymax=-tmp;

  for (i=0; i<NN->N; i++)
    {tmp=NN->dz[i]; NN->dz[i]=NN->dy[i]; NN->dy[i]=-tmp;}
}

void nn_rotatex_180(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->zmin; NN->zmin=-NN->zmax; NN->zmax=tmp;
  tmp=NN->ymin; NN->ymin=-NN->ymax; NN->ymax=tmp;

  for (i=0; i<NN->N; i++)
    {NN->dz[i]=-NN->dz[i]; NN->dy[i]=-NN->dy[i];}
}

void nn_rotatex_right(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->Nz; NN->Nz=NN->Ny; NN->Ny=tmp;

  tmp=NN->ymax;
  NN->ymax= NN->zmax;
  NN->zmax=-NN->ymin;
  NN->ymin= NN->zmin;
  NN->zmin=-tmp;

  for (i=0; i<NN->N; i++)
    {tmp=NN->dz[i]; NN->dz[i]=-NN->dy[i]; NN->dy[i]=tmp;}
}

// ROTATIONS AROUND Y AXIS 
void nn_rotatey_right(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->Nz; NN->Nz=NN->Nx; NN->Nx=tmp;

  tmp=NN->zmin;
  NN->zmin= NN->xmin;
  NN->xmin=-NN->zmax;
  NN->zmax= NN->xmax;
  NN->xmax=-tmp;

  for (i=0; i<NN->N; i++)
    {tmp=NN->dz[i]; NN->dz[i]=NN->dx[i]; NN->dx[i]=-tmp;}
}

void nn_rotatey_180(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->zmin; NN->zmin=-NN->zmax; NN->zmax=tmp;
  tmp=NN->xmin; NN->xmin=-NN->xmax; NN->xmax=tmp;

  for (i=0; i<NN->N; i++)
    {NN->dz[i]=-NN->dz[i]; NN->dx[i]=-NN->dx[i];}
}

void nn_rotatey_left(NN_STRUCT *NN)
{
  int i,tmp;

  tmp=NN->Nz; NN->Nz=NN->Nx; NN->Nx=tmp;

  tmp=NN->xmax;
  NN->xmax= NN->zmax;
  NN->zmax=-NN->xmin;
  NN->xmin= NN->zmin;
  NN->zmin=-tmp;

  for (i=0; i<NN->N; i++)
    {tmp=NN->dz[i]; NN->dz[i]=-NN->dx[i]; NN->dx[i]=tmp;}
}
