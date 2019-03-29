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
#include "BBX_MC_FilterSamples.h"

int analyse_bbx_convergence_file(char *fname, double TSA, double tol, int Nchi, int verbose)
{
  int i,j;
  int Nlines, Ncol, Nheader, Ndata, colno;
  double *columndata, *chicol;
  
  int ind_best;
  double chi_best, a_best, b_best, c_best, d_best, e_best, a_mean, b_mean, c_mean, d_mean, e_mean;

  int N2, count;
  double *col2,*col3,*col4,*col5,*col6;
  double *cola,*colb,*colc,*cold,*cole,*colX2;
  double chitar;

  double chimin, chimax, SEM;
  FILE *ifp;

  if(!analyse_data_file_properties(fname, &Nlines, &Ncol, &Nheader, &Ndata, verbose))
  {printf("Error: analyse_data_file_properties() failed!\n"); return(0);}

  //-----------------------------------------------------
  printf("\n#Unfiltered data:\n");

  chicol    =(double*)malloc(Ndata*sizeof(double));
  int* chimask   =(int*)malloc(Ndata*sizeof(int));
  colX2 =(double*)malloc(Ndata*sizeof(double));
  col2  =(double*)malloc(Ndata*sizeof(double));
  col3  =(double*)malloc(Ndata*sizeof(double));
  col4  =(double*)malloc(Ndata*sizeof(double));
  col5  =(double*)malloc(Ndata*sizeof(double));
  col6  =(double*)malloc(Ndata*sizeof(double));
  cola  =(double*)malloc(Ndata*sizeof(double));
  colb  =(double*)malloc(Ndata*sizeof(double));
  colc  =(double*)malloc(Ndata*sizeof(double));
  cold  =(double*)malloc(Ndata*sizeof(double));
  cole  =(double*)malloc(Ndata*sizeof(double));
  columndata=(double*)malloc(Ndata*sizeof(double));

  if(!dreadColumn(fname, Ndata, Nheader, 1, chicol))
  {printf("Error: dreadColumn() failed!\n"); return(0);}
  if(!dreadColumn(fname, Ndata, Nheader, 2, col2))
  {printf("Error: dreadColumn() failed!\n"); return(0);}
  if(!dreadColumn(fname, Ndata, Nheader, 3, col3))
  {printf("Error: dreadColumn() failed!\n"); return(0);}
  if(!dreadColumn(fname, Ndata, Nheader, 4, col4))
  {printf("Error: dreadColumn() failed!\n"); return(0);}
  if(!dreadColumn(fname, Ndata, Nheader, 5, col5))
  {printf("Error: dreadColumn() failed!\n"); return(0);}
  if(!dreadColumn(fname, Ndata, Nheader, 7, col6))
  {printf("Error: dreadColumn() failed!\n"); return(0);}

  chi_best = d1min(    chicol, Ndata);
  for(i=0; i<Ndata; i++)
    chimask[i]=1;

  double varalpha=TSA, varbeta;
  double var;
  var=0; count=0;
  for(i=0; i<Ndata; i++)
  {
    if(chimask[i]==1)
    {
      count++;
      var  += pow(chicol[i]-chi_best, 2);
    }
  }
  var/=count;

  printf("#Ndata     = %d\n", Ndata);
  printf("#chibest   = %f\n", chi_best);
  printf("#chi_std_a = %f\n", sqrt(varalpha));
  printf("#chi_std   = %e\n", sqrt(var));

  if(var<varalpha)
  {
    printf("Error: chi_std unexpectedly smaller than chi_std_a; Possible solution is to rerun the software with a larger --sigma value.\n"); 
    exit(1);

  }
  //-----------------------------------------------------
  
  //-----------------------------------------------------
  double tolerance=0.1;
  printf("\n#Filtered data: \n");

  varbeta=1.0/(1.0/varalpha - 1.0/var);

  double err_sigma=2*tolerance, vartmp;
  while(err_sigma>tolerance )
  {
    for(i=0; i<Ndata; i++)
      chimask[i] = ( drand(1.0)< exp( - (chicol[i]-chi_best)/(2*varbeta) ) ? 1 : 0   );

    var=0; count=0;
    for(i=0; i<Ndata; i++)
    {
      if(chimask[i]==1)
      {
        count++;
        var  += pow(chicol[i]-chi_best, 2);
      }
    }
    var/=count;

    varbeta=1.0/(1.0/varbeta + (1.0/varalpha + 1.0/varbeta - 1.0/var));
    varbeta=fabs(varbeta);
    err_sigma=fabs(1.0/(1.0/varalpha + 1.0/varbeta)/var - 1);

    if(varbeta<=0)
     break;
  }


  printf("#Ndata              = %d\n", count);
  printf("#chibest            = %f\n", chi_best);
  printf("#chi_std_a          = %f\n", sqrt(varalpha));
  printf("#chi_std_b          = %e\n", sqrt(varbeta));
  printf("#chi_std            = %e\n", sqrt(var));
  printf("#(1/va+1/vb)^(-1/2) = %e\n", sqrt(1.0/(1.0/varalpha + 1.0/varbeta)));


  // Truncate
  var=1.0/(1.0/varalpha + 1.0/varbeta);
  for(i=0; i<Ndata; i++)
    if(chicol[i]-chi_best<sqrt(var))
      chimask[i]=1;
    else
      chimask[i]=0;

  //-----------------------------------------------------
 


  //-----------------------------------------------------
  printf("\n#Parameter values (filtered data truncated above chisquare=%e):\n", chi_best+sqrt(var));

  int count2=0;
//  count=0;
  for(i=0; i<Ndata; i++)
  {
      if(chicol[i]==chi_best)
      {
        a_best=col2[i];
        b_best=col3[i];
        c_best=col4[i];
        d_best=col5[i];
        e_best=col6[i];
      }
    if(chimask[i]==1)
    {
      colX2[count2]=chicol[i];
      cola[count2] = col2[i];
      colb[count2] = col3[i];
      colc[count2] = col4[i];
      cold[count2] = col5[i];
      cole[count2] = col6[i];
      count2++;
    }
  }
  count=count2;
  a_mean=d1mean(cola, count);
  b_mean=d1mean(colb, count);
  c_mean=d1mean(colc, count);
  d_mean=d1mean(cold, count);
  e_mean=d1mean(cole, count);

  printf("#Ndata = %d\n", count);
  printf(  "#par   %12s %12s %12s %12s %16s\n", "best_fit", "mean", "std", "sem", "normalised_sem");
    printf("#tauS  %12e %12e %12e %12e %16f\n", a_best, a_mean, sqrt(d1variance(cola, count)), sqrt(d1variance(cola, count)/(count-1)), sqrt(d1variance(cola, count)/(count-1))/a_mean);
    printf("#ZE    %12e %12e %12e %12e %16f\n", b_best, b_mean, sqrt(d1variance(colb, count)), sqrt(d1variance(colb, count)/(count-1)), sqrt(d1variance(colb, count)/(count-1))/b_mean);
    printf("#ZS    %12e %12e %12e %12e %16f\n", c_best, c_mean, sqrt(d1variance(colc, count)), sqrt(d1variance(colc, count)/(count-1)), sqrt(d1variance(colc, count)/(count-1))/c_mean);
    printf("#Ge    %12e %12e %12e %12e %16f\n", d_best, d_mean, sqrt(d1variance(cold, count)), sqrt(d1variance(cold, count)/(count-1)), sqrt(d1variance(cold, count)/(count-1))/d_mean);
    printf("#alpha %12e %12e %12e %12e %16f\n", e_best, e_mean, sqrt(d1variance(cole, count)), sqrt(d1variance(cole, count)/(count-1)), sqrt(d1variance(cole, count)/(count-1))/e_mean);

  ifp=fopen("bbx_convergence_filtered.out", "w");

  fprintf(ifp, "%12s %12s %12s %12s %12s %12s\n", "chisq", "log10(tauS)", "ZE", "ZS", "Ge", "alpha");

  for(i=0; i<count;i++)
  {
    fprintf(ifp, "%12e %12e %12e %12e %12e %12e\n", colX2[i], log10(cola[i]), colb[i], colc[i], cold[i], cole[i]);
  }


  fclose(ifp);


  putchar('\n');
  printf("#Checking results:\n");
  if(count<=5)
    printf("#WARNING: Sparse data set: only %d data point(s)!\n", count);
   
  if ( (a_mean<a_best? a_best-a_mean : a_mean-a_best) > sqrt(d1variance(cola, count)) ) 
    printf("#WARNING: mean and best value of \"tauS\" larger than STD.\n");
  else
    printf("#SUCCES: mean and best value of \"tauS\" are within STD.\n");
  if ( (b_mean<b_best? b_best-b_mean : b_mean-b_best) > sqrt(d1variance(colb, count)) ) 
    printf("#WARNING: mean and best value of \"ZE\" larger than STD.\n");
  else
    printf("#SUCCES: mean and best value of \"ZE\" are within STD.\n");
  if ( (c_mean<c_best? c_best-c_mean : c_mean-c_best) > sqrt(d1variance(colc, count)) )
    printf("#WARNING: mean and best value of \"ZS\" larger than STD.\n");
  else
    printf("#SUCCES: mean and best value of \"ZS\" are within STD.\n");
  if ( (d_mean<d_best? d_best-d_mean : d_mean-d_best) > sqrt(d1variance(cold, count)) )
    printf("#WARNING: mean and best value of \"Ge\" larger than STD.\n");
  else
    printf("#SUCCES: mean and best value of \"Ge\" are within STD.\n");
  if ( (e_mean<e_best? e_best-e_mean : e_mean-e_best) > sqrt(d1variance(cole, count)) )
    printf("#WARNING: mean and best value of \"alpha\" larger than STD.\n");
  else
    printf("#SUCCES: mean and best value of \"alpha\" are within STD.\n");
  //-----------------------------------------------------

  free(chicol);
  free(chimask);
  free(columndata);

  free(col2);free(col3);free(col4);free(col5);free(col6);
  free(cola);
  free(colb);
  free(colc);
  free(cold);
  free(cole);
  free(colX2);

  return(1);
}
