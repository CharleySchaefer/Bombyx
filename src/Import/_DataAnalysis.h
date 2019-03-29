/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
int getCrossoverFrequency(double *w, double *Gvisc, double *Gelas, int Ndata, double *wcrossover)
{
  int i;
  double wp, wn;
  int check1=1;
  int check2=( Gelas[0]>Gvisc[0] ? 1 : -1);  wp=w[0];
  for(i=0; i<Ndata; i++)
  { 
    if(wn<wp)
    {printf("Error: frequencies should have ascending values!\n"); return(0);}
    if(check1)
    {
      if(check2!=( Gelas[i]>Gvisc[i] ? 1 : -1)) // change of sign
      {
        *wcrossover=w[i]; check1=0; // crossover frequency
      }
    }
  }
  return(1);
}

int calculateResnorm(INPUT *Set, SREP *Model, double *chisquare)
{
  int i;
  *chisquare=0.0;
  for(i=0; i<Model->Ndata; i++)
  {
    (*chisquare) += pow( (Model->Gelas[i]-Set->Gelas[i])/Set->Gelas[i], 2) + pow( (Model->Gvisc[i]-Set->Gvisc[i])/Set->Gvisc[i], 2);
  }
  (*chisquare)/=(Set->Ndata-Model->Npar);
  return(1);
}
