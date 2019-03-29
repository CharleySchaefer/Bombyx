/*
  VSSM

  kinetic Monte Carlo: Variable step size method

  DEPENDENCE: uses find_index_above_y0_float() from ZiltoidLIB/NumericalMethods

  Algorithm:

    1. Initialise VSSM: 
      Initialisation function provided in VSSM.h
       - using VSSM_initialise
       - Set (intended) number of time steps, Vssm->max_iter,
         may be updated throughout the algorithm.
       - Set number of events / rates, Vssm->max_rates,
         may also be updated throughout the algorithm. 
       - allocate memory

    2. calculate rates
       - set the values in the array Vssm->rates
       - set the number of rates Vssm->Nrates

    --------------------------------------------------
    3. CORE (Functions independent of physical model)
      Input:  Rates, Nrates
      Output: event number
      3.A. calculate total rate
         - using VSSM_sum_rates

      3.B. determine (stochastic & adaptive) time step
         - using VSSM_get_time_step

      3.C. select event
         - using VSSM_select_event
         - sets int value VSSM->selected_event
    --------------------------------------------------

    4. execute event
   
    5. repeat from 2 until max_iter is reached

  TODO:
    - For large simulations, the event selection can be 
      significantly improved in terms of cpu time: 
      By grouping events in Ngroup groups where all
      events have the same rate may speed up the cpu 
      time from ~ log N_rates to ~ log Ngroup. 
*/
#include "VSSM.h"

int VSSM_initialise(VSSM **Vssm, int max_iter, int max_rates)
{
  *Vssm              = ( VSSM*)malloc(sizeof( VSSM));
  (*Vssm)->max_iter  = max_iter;  // maximum number of iterations
  (*Vssm)->max_rates = max_rates; // number of events/rates
  (*Vssm)->t         = 0.0;    // initialise time
  (*Vssm)->N_iter    = 0;      // number of kMC time steps
  (*Vssm)->N_rates   = max_rates;
  (*Vssm)->event_list=(int  *)malloc(max_rates*sizeof(int  ));
  (*Vssm)->rate      =(float*)malloc(max_rates*sizeof(float));
  (*Vssm)->S         =(float*)malloc(max_rates*sizeof(float));
  return(1);
}

int VSSM_sum_rates(VSSM *Vssm)
{
  int i;
  if(Vssm->N_rates==0)
  {
    Vssm->sum_rates=0.0;
  }
  else
  {
    Vssm->S[0]=Vssm->rate[0];
    for(i=1; i<Vssm->N_rates; i++)
    {
      Vssm->S[i] = Vssm->S[i-1] + Vssm->rate[i];
    }
    Vssm->sum_rates=Vssm->S[Vssm->N_rates-1];
  }
  return(1);
}

int VSSM_get_time_step(VSSM *Vssm)
{
  float u=(float)rand()/(RAND_MAX); // Uniform deviate on unit interval
  Vssm->dt=-log(u)/Vssm->sum_rates; // Time step
  return(1);
}

int VSSM_select_event(VSSM *Vssm)
{
  int selected_event;
  float r=Vssm->sum_rates*(float)rand()/RAND_MAX;
  find_index_above_y0_float(Vssm->S, Vssm->N_rates, r, &selected_event );
  Vssm->selected_event=selected_event;
  return(1);
}
