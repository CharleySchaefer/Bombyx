/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
#include "export.h"


int export_bbx_data(INPUT *Set)
{
  int i;
  FILE *ifp;
  ifp=fopen(DATAFILE, "w");
  // Header
  fprintf(ifp,"%-26s %d\n", "#DegreeOfPolymerisation:", Set->N             );
  fprintf(ifp,"%-26s %e\n", "#MonomerVolume:",          Set->v             );
  fprintf(ifp,"%-26s %f\n", "#VolumeFraction:",         Set->phi           );
  fprintf(ifp,"%-26s %f\n", "#Temperature:",            Set->T             );
  fprintf(ifp,"%-26s %d\n", "#Frequency:",              0); // Data was formatted to rad/s!
  // Data
  fprintf(ifp,"%15s %15s %15s %15s\n", "#w[rad/s]", "Gvisc[Pa]", "w[rad/s]", "Gelas[Pa]");
  fprintf(ifp,"%15s %15s %15s %15s\n", "------------", "------------", "------------", "------------");
  for(i=0; i<Set->Ndata; i++)
    fprintf(ifp,"%15e %15e %15e %15e\n", Set->w1[i], Set->Gvisc[i], Set->w2[i], Set->Gelas[i]); 

  fclose(ifp);
  return(1);
}
