% Evaluate Sticky-Rouse model: G(t), G'(w), G''(w) are known analytically.
% Use two numerical schemes to determine G' and G'' from G(t) and compare
% to analytical result
function test_getModuli()
  close all; clc;
  addpath('StickyRouse') % Tranform G(t) to G'(w) and G''(w)
  addpath('LikhtmanMcLeish')  % G(t) for reptation - LikhtmanMcLeish
                              % does NOT include Rouse modes
  addpath('DoubleReptation')  % 
  addpath('getDynamicModuli') % Tranform G(t) to G'(w) and G''(w)

  %==============================================================
  % USER SETTINGS

  % A. Polymer properties
  Ze=10;       % Number of entanglements per chain
  tau_e=1e-6;  % Rouse time of entanglement strand
               % NOTE: tau_e not used if Zs>0 
  Zs=5;        % Number of stickers per chain
  tau_s=1e-2;  % Sticker dissociation time

  % B. Materials parameters
    % Likhtman-McLeish model
  LM_Cnu=0.7;  % Likhtman-McLeish: strength of contour-length fluctuations
    % Double-Reptation model
  DR_alpha=10;
  DR_beta=2;

  % Numerical parameter values
  nFEM=20;                 % Number of frequencies to calculate G(w)
  StickyLM_tolerance=1e-5; % Numerical tolerance for Sticky LikhtmanMcLeish
  initialise_DR=0; % 0: load pre-calculated  Double Reptation
                   % 1: new  pre-calculation Double Reptation; 
   % DR precalculation parameters 
   Nsamples=40; % Number of samples: g(x) and F(U)
   tol=1e-4;    % Numerical tolerance for g(x) and F(U) calculation
   Nt=64;       % Number of samples time 
   NH=32;       % Number of samples H parameter
   HL=1e-2;  HU=1e2; % Interpolation interval: H
   tL=1e-6;  tU=1e2; % Interpolation interval: t
   % END DR precalculation parameters 
  %==============================================================

  %if initialise_DR==0 % Load precalculated DR model
  %  load('DoubleReptation/Precalculations');
  %elseif initialise_DR==1
  %  DOUBLE_REPTATION = initialise_DoubleReptation( Nsamples, Nt, NH, tol, HL, HU, tL, tU);
  %  save('DoubleReptation/Precalculations', 'DOUBLE_REPTATION');
  %end

  if Zs==0
    tau_d0=3*tau_s*Ze^3;    % Reptation time
  elseif (Zs>0)
    tau_d0=3*tau_s*Zs^2*Ze; % Sticky-reptation time
    tS=tau_s*(Zs)*(Zs);     % Relaxation time of sticker strand
  else
    error('Zs cannot be negative.\n');
  end


  



  % Time range for G(t)
  trow=10.^(linspace(-log10(tau_s)-7,log10(tau_d0)+2, 40));

  %Grep   = Gt_StickyLikhtmanMcLeish(trow/tau_s, Ze, Zs, Cnu, StickyLM_tolerance);
  %trow=trow(Grep>0);
  %Grep=Grep(Grep>0);
  Grep=zeros(size(trow));

  % Sticky Rouse: Relaxation modulus
  Gsr    = Gt_StickyRouse(trow/tS, Ze, Zs );
  Gt  =Gsr+Grep;
  Gsr =Gsr( Gt>0);
  trow=trow(Gt>0);
  Gt  =Gt(  Gt>0);
  
  % Sticky Rouse: Exact dynamic moduli
  wrow=10.^(linspace(log10(0.1/max(trow)), log10(10/max(trow(1))), nFEM));
  G1G2_sr=G1G2_StickyRouse(wrow*tS, Ze, Zs );

  % Numerical dynamic moduli via two methods
  G1G2_A =getDynamicModuli_Nobile(       trow, Gt, nFEM);
  G1G2_B  =getDynamicModuli_EvansTassieri(trow, Gt, nFEM);



  %===========================================================
  % Plot results
  figure
  subplot(2,1,1)
  %trow=trow(Gt>0); Grep=Grep(Gt>0); Gt  =Gt(Gt>0);
  plot(log10(trow(Gt>0)), log10(Gt(Gt>0)), 'k', 'LineWidth', 2); hold on
  %loglog(trow, Gsr, 'b', 'LineWidth', 2); hold on;
  %loglog(trow, Grep, 'g', 'LineWidth', 2); hold on;
 % legend('G_{tot}', 'G_{sr}') % , 'G_{rep}'
  xlabel('time')
  ylabel('log_{10}G(t)')

  subplot(2,1,2)
  loglog(G1G2_sr(G1G2_sr(:,2)>0,1)/tS, G1G2_sr(G1G2_sr(:,2)>0,2), '.r', 'MarkerSize', 12); hold on;
  loglog(G1G2_sr(G1G2_sr(:,3)>0,1)/tS, G1G2_sr(G1G2_sr(:,3)>0,3), '.g', 'MarkerSize', 12); hold on;

  loglog(G1G2_A(G1G2_A(:,2)>0,1), G1G2_A(G1G2_A(:,2)>0,2), 'k', 'LineWidth', 2); hold on
  loglog(G1G2_A(G1G2_A(:,3)>0,1), G1G2_A(G1G2_A(:,3)>0,3), 'k', 'LineWidth', 2); hold on
  loglog(G1G2_B(G1G2_B(:,2)>0,1), G1G2_B(G1G2_B(:,2)>0,2), 'r', 'LineWidth', 1); hold on;
  loglog(G1G2_B(G1G2_B(:,3)>0,1), G1G2_B(G1G2_B(:,3)>0,3), 'g', 'LineWidth', 1); hold on;
  axis([0.5/tau_d0, 20/tau_s, 1e-7, 2])
  xlabel('frequency')
  ylabel('G`(w), G``(w)')

 
end

