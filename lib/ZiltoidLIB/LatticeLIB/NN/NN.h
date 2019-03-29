/*
  NN.h

  
           3----------7
          /|         /|
    ^    / |        / |
    |   1----------5  |
    |   |  |       |  |
    k   |  2-------|--6   ^
        | /        | /   /
        |/         |/   /
        0----------4   j
           i --->

  i: x-axis
  j: y-axis
  k: z-axis

  A -> B: A moves to B

  ROTATIONS
  ---------
  rotatez_left:  4->2, 5->3, etc.
  rotatez_right: 2->4, 3->5, etc.   

  rotatex_left:  2->1, 6->5, etc.
  rotatex_right: 1->2, 5->6, etc. 

  rotatey_left:  1->4, 3->6, etc. 
  rotatey_right: 4->1, 6->3, etc.

  REFLECTIONS
  -----------

*/
#ifndef NN_LIB_H
  #define NN_LIB_H 1

  #include <stdio.h>
  #include <stdlib.h>

  // TODO: Implement 3D matrix rotations
  // NN_STRUCT:
  //
  // input needed to create structure using new_nn_struct(): 
  //   N:            number of non-zero elements in sparse (boolean) matrix
  //                 Boolean in the sense that either an element is present/non-zero or not.
  //   dim:          either only (x), (xy) or all (xyz) coordinates are used, respectively
  //
  // data stored in the structure (user input): 
  //   dx,dy,dz:     coordinates (memory allocated using new_nn_struct() after 
  //                 N and dim are specified.
  //
  // Properties determined using nn_analyse():
  //   central_site: element number for which dx[central_site]=0 (same for dy,dz)
  //   xmin, etc:    minima and maxima
  //
  // Sparse matrix is transformed to dense matrix and printed using nn_print
  typedef struct {
    int N;   // number of elements
    int dim; // dimensionality: 1, 2, or 3
    int central_site;
    int *dx;
    int *dy;
    int *dz;
    int Nx, xmin, xmax;
    int Ny, ymin, ymax;
    int Nz, zmin, zmax;
  }  NN_STRUCT;


  //---------------------------------------
  // Create and free NN structure
  // input: 
  //   int N:   number of elements in the (sparse) matrix
  //   int dim: dimensionality of the matrix (1,2 or 3)
  NN_STRUCT  *new_nn_struct(int, int );
  NN_STRUCT *copy_nn_struct(NN_STRUCT *);
  void       free_nn_struct(NN_STRUCT *);

  //---------------------------------------
  // input: structure with specified coordinates
  // output:
  //    Nx, xmin, xmax (as well as Ny, etc.)
  //    returns 1 on succes and -1 if coordinate (0,0) is missing
  int  nn_analyse(NN_STRUCT *); 
        
  //---------------------------------------
  // print matrix
  // ibuff should be of length NN->N
  void nn_print(NN_STRUCT *NN, int *);


  void nn_translate_x(NN_STRUCT *, int);
  void nn_translate_y(NN_STRUCT *, int);
  void nn_translate_z(NN_STRUCT *, int);

  //---------------------------------------
  // matrix transformations (functional in 2D and 3D)
  void nn_reflect_x(    NN_STRUCT *);
  void nn_reflect_y(    NN_STRUCT *);
  void nn_reflect_z(    NN_STRUCT *);

  // Rotate around z axis (functional in 2D and 3D)
  void nn_rotatez_180(  NN_STRUCT *);
  void nn_rotatez_right(NN_STRUCT *);
  void nn_rotatez_left( NN_STRUCT *);

  // Rotate around x axis (functional in 3D)
  void nn_rotatex_180(  NN_STRUCT *);
  void nn_rotatex_right(NN_STRUCT *);
  void nn_rotatex_left( NN_STRUCT *);

  // Rotate around y axis (functional in 3D)
  void nn_rotatey_180(  NN_STRUCT *);
  void nn_rotatey_right(NN_STRUCT *);
  void nn_rotatey_left( NN_STRUCT *);

#endif
