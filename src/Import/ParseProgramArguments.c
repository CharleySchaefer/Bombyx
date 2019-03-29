/*
    Bombyx: Modelling of linear viscoelasticity
    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
*/

#include "ParseProgramArguments.h"

int ParseProgramArguments(int argc, char *argv[], MCSET **MCSet_P, SREP **Model_P, int *verbose)
{
  int i;

  (*MCSet_P)=make_MC_SET();
  MCSET *MCSet=(*MCSet_P);

  (*Model_P)=make_SREP();
  SREP *Model=(*Model_P);

  if(argc<1)
  {printf("Error: No input file specified - terminating. Run %s --help for help.\n", argv[0]); return(-1);}
  for(i=1; i<argc; i++)
  {
    if(!strcmp(argv[i], "--copying")) 
    {
      system("cat ../COPYING.txt"); exit(1);
    }

    // MONTE CARLO SETTINGS
    if(!strcmp(argv[i], "--MCseeds"))
      MCSet->Nseeds = atoi(argv[i+1]);
    else if(!strcmp(argv[i], "--MCiter"))
      MCSet->Niter = atoi(argv[i+1]);
    else if(!strcmp(argv[i], "--MCdelta"))
      MCSet->delta = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--MCsigma"))
      MCSet->TSA = 2*pow( atof(argv[i+1]), 2 );
    else if(!strcmp(argv[i], "--rseed"))
      MCSet->rseed = atof(argv[i+1]);

    // MODELLING SETTINGS
    else if (!strcmp(argv[i], "--alpha"))
      Model->alpha = atof(argv[i+1]);
    else if (!strcmp(argv[i], "--alphaL"))
      MCSet->alpha_L = atof(argv[i+1]);
    else if (!strcmp(argv[i], "--alphaU"))
      MCSet->alpha_U = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--tauSL"))
      MCSet->tauS_L = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--tauSU"))
      MCSet->tauS_U = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--ZeL"))
      MCSet->Ze_L = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--ZeU"))
      MCSet->Ze_U = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--ZsL"))
      MCSet->Zs_L = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--ZsU"))
      MCSet->Zs_U = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--GeL"))
      MCSet->Ge_L = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--GeU"))
      MCSet->Ge_U = atof(argv[i+1]);
    else if(!strcmp(argv[i], "--Ge-fac"))
    {
      MCSet->Ge_fac = atof(argv[i+1]);
      Model->Ge_fac=MCSet->Ge_fac;
    }


    // PROGRAM SETTINGS
    else if(!strcmp(argv[i], "--verbose"))
      (*verbose)      = 1;
    else if( (!strcmp(argv[i], "--version"))||(!strcmp(argv[i], "-h") ) )
    {
      printf("Bombyx %.1f\n", BBX_VERSION); 
      free(MCSet);
      free(Model);
      exit(1);
    }
    else if( (!strcmp(argv[i], "--help"))||(!strcmp(argv[i], "-h") ) )
    {
      printf("\n");
      printf("  Bombyx %f Copyright (C) 2019 Charley Schaefer\n", BBX_VERSION);
      printf("  This program comes with ABSOLUTELY NO WARRANTY;\n");
      printf("  for details type \"%s --copying\".\n", argv[0]);
      printf("  This is free software, and you are welcome to redistribute it\n");
      printf("  under certain conditions; type \"%s --copying\" for details.\n", argv[0]);
      printf("\n");
      printf("  Usage: %s <datafile.bbx> [arguments]\n\n", argv[0]);
      printf("  datafile.bbx: file with physical properties (degree of polymerisation,\n");
      printf("    monomer volume, volume fraction and temperature), and dynamic modulus\n");
      printf("    measurements. See example \'Demos/demodata/demodata.bbx\'.\n\n");
      printf("  Arguments:\n\n");
      printf("    PROGRAM SETTINGS:\n");
      printf("    --verbose:         verbose shell output. \n\n");
      printf("    MODELLING SETTINGS:\n");
      printf("                       value Ge ~ rho x kT x Ze. (default Ge_fac = 1.0).\n");
      printf("    --(...)   <value>  Controlling parameter boundaries:\n");
      printf("                       --tauSL,  --tauSU,  (sticker dissociation time)\n");   
      printf("                       --ZsL,    --ZsU,    (number of stickers)\n");   
      printf("                       --ZeL,    --ZeU,    (number of entanglements)\n");   
      printf("                       --GeL,    --GeU,    (prefactor elasticity modulus)\n");   
      printf("                       --alphaL, --alphaU. (contour-length fluctuations)\n");  
      printf("    Lower and upper bounds may be set to the same value, or, in case\n");
      printf("    of Ge or alpha, may be fixed using\n");   
      printf("    --alpha   <value>  - Fixed strength of contour-length fluctuations.\n");
      printf("    --Ge-fac  <value>  - Fixed modulus prefactor with respect to the estimated.\n\n");
      printf("    MONTE CARLO SETTINGS (see \'doc/user_manual.pdf\' for details):\n");
      printf("    --MCseeds <value>   Number of randomised initial parameter sets.\n");
      printf("    --rseed   <value>   Random number seed. -1: use time stamp.\n");   
      printf("                                            >0: user-defined integer.\n");
      printf("    --MCiter  <value>   Number of MC steps per initial parameter set.\n");
      printf("    --MCdelta <value>   Controls MC step size .\n");
      printf("    --MCsigma <value>   Controls acceptance probability.\n");  
      printf("                        P ~ exp( - chi_square / 2 MCsigma^2 ).\n"); 
      printf("                        Large value: larger exploration of parameter space,\n");
      printf("                                     but more data will be removed from the\n");  
      printf("                                     distribution.\n");  
      printf("                        Small value: potential trapping in local minimum.\n");    
      putchar('\n');
      free(MCSet);
      free(Model);
      exit(1);
      return(2);
    }
  }

  

  Model->Npar   = 0;
  if (MCSet->tauS_L<MCSet->tauS_U)
    Model->Npar++;
  else if(MCSet->tauS_L>MCSet->tauS_U)
  {
    printf("Error: tauS lower value should be smaller than the upper value.\n");
  }

  if (MCSet->Zs_L<MCSet->Zs_U)
    Model->Npar++;
  else if(MCSet->Zs_L>MCSet->Zs_U)
  {
    printf("Error: Zs lower value should be smaller than the upper value.\n");
  }

  if (MCSet->Ze_L<MCSet->Ze_U)
    Model->Npar++;
  else if(MCSet->Ze_L>MCSet->Ze_U)
  {
    printf("Error: Ze lower value should be smaller than the upper value.\n");
  }

  if (MCSet->Ge_L<MCSet->Ge_U)
    Model->Npar++;
  else if(MCSet->Ge_L>MCSet->Ge_U)
  {
    printf("Error: Ge-prefactor lower value should be smaller than the upper value.\n");
  }

  if (MCSet->alpha_L<MCSet->alpha_U)
    Model->Npar++;
  else if( MCSet->alpha_L==-1 && MCSet->alpha_U==-1 )
  {
    MCSet->alpha_L = Model->alpha;
    MCSet->alpha_U = Model->alpha;
  }
  else if(MCSet->alpha_L>MCSet->alpha_U)
  {
    printf("Error: alpha lower value should be smaller than the upper value.\n");
  }
  return(1);
}

