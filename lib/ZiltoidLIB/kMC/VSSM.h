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
#ifndef VSSM_H
  #define VSSM_H 1

  typedef struct {
    int 	max_iter;        // number of kMC time steps
    int 	max_rates;       // number of possible rates
    float *rate;           // all possible rates
    float *S;              // cummulative rates
    int   *event_list;     // list with labels that refer to events

    int   selected_event;  // label of event after selection
    int   N_iter;          // iteration counter
    int   N_rates;         // number of non-zero rates
    float sum_rates;       // sum over all rates *rate
    float t;               // time
    float dt;              // (adaptive) time step
  } VSSM;

  int VSSM_initialise(   VSSM **, int max_iter, int max_rates);
  int VSSM_sum_rates(    VSSM * );
  int VSSM_get_time_step(VSSM * );
  int VSSM_select_event( VSSM * );

  #include "../ZiltoidLIB.h"
#endif
