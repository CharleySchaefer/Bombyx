#ifndef LATTICE_HCP_H
  #define LATTICE_HCP_H 1
  #include <stdio.h>
  #include <stdlib.h>

  /*
     HCP.h
     Hexagonal Closed Packing
     2D xy plane: Hexagonal lattice

              
     Connectivity of site in nearest neighbourhood 
     of site G in plane k:

     plane k+1:
                       A-----B
                        \   /
                         \ /
                          C
     
            (A; i-1, j  )   (B; i  , j  )

                   (C; i  , j-1) 

     plane k:
                       D-----E
                      / \   / \
                     /   \ /   \
                    F-----G-----H
                     \   / \   /
                      \ /   \ /
                       I-----J

            (D; i-1, j+1)   (E; i  , j+1)

      (F; i-1, j  )   (G; i  , j  )   (H; i+1, j  )
 
            (I; i  , j-1)   (J; i+1, j-1)

     plane k-1:
                       K-----L
                        \   /
                         \ /
                          M
     
            (K; i+1, j  )   (L; i+1, j+1)

                   (M; i+1, j  ) 

    Connectivities of central site G (12):
    G-A, G-B, G-C                  (connections to plane k+1)

    G-D, G-E, G-F, G-H, G-I, G-J   (connections in plane k)

    G-K, G-L, G-M                  (connections to plane k-1)

    Connectivities between nearest neighbours (24):
    A-B, A-C, B-C                   (in plane k+1)

    A-F, A-D, B-E, B-H, C-I, C-J    (between planes k and k+1

    D-E, E-H, H-J, J-I, I-F, F-D    (in plane k)

    K-F, K-D, L-E, L-H, M-I, M-J    (between planes k and k+1

    G-K, G-L, G-M                   (in plane k-1)

    Geometry of this set is a "triangular orthobicupola"

  */
  typedef struct{
    int dim;           // 2: in-plane hexagonal; 3: HCP packing 
    int Ncomponents;
    int *site;         // array with sites
    int is_periodic_x; 
    int is_periodic_y;
    int is_periodic_z;
    int Nx, Ny, Nz;    // Size
    int Nxy;           // Number of in-plane sites: Nxy    = Nx*Ny
    int Nxyz;          // Total number of sites:    Nsites = Nx*Ny*Nz
  } LATTICE_HCP;

  LATTICE_HCP *make_lattice_hcp(int, int, int);
  int free_lattice_hcp(LATTICE_HCP *);

  int coor2ind_hex( LATTICE_HCP *, int, int);                 // i,j    -> ind
  int coor2ind_hcp( LATTICE_HCP *, int, int,   int);          // i,j,k  -> ind
  int ind2coor_hex( LATTICE_HCP *, int, int *, int *);        // ind -> i,j
  int ind2coor_hcp( LATTICE_HCP *, int, int *, int *, int *); // ind -> i,j,k

#endif

