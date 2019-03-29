/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/
/*

    This script reads the "bbx_convergence.out" file,
    which contains parameter sets and their fit quality.
    These data are obtained using an acceptance probability

    ~ exp(  - chi_square / 2 sigma_alpha^2   )

    In this script, data is filtered by accepting data using the probability

    ~ exp(  - chi_square / 2 sigma_beta^2   )

    which effectively generates a data set with acceptance probability

    ~ exp(  - chi_square / 2 sigma^2   ),

    with sigma^{-2} = 1.0/( sigma_alpha^{-2} + sigma_beta^{-2}   )

    If the chi square landscape is parabolic, 

    < (chi square-chi square_optimum) > = sigma^{-2}

    This script finds sigma_beta, such that this condition is met.
    Because the landscape is not perfectly parabolic, the chi square 
    distribution is truncated for chi square >  chi square_optimum + sigma.

*/
#ifndef BBX_MC_FILTER_SAMPLES_H
#define BBX_MC_FILTER_SAMPLES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int analyse_bbx_convergence_file(char *, double, double, int, int);


#endif // BBX_MC_FILTER_SAMPLES_H

