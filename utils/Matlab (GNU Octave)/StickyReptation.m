% Charley Schaefer 2020-03-05
%
% STICKY REPTATION
%
% 1. Precalculate G(t/tau_rep; H) double reptation model for 
%    time and H range --> interpolation table
%    OPTIONS:
%    initialise_DR=0: use previously calculated interpolation table
%                     in DoubleReptation/Precalculations
%    initialise_DR=1: calculate new interpolation table; store in
%                     DoubleReptation/Precalculations
% 2. Interpolate double repation G(t)
% 3. Transform G(t) --> G'(w), G''(w)
% TODO: Generate G' and G'' interpolation table
% 4. Calculate Sticky Rouse G', G'' and add to double reptation term.
function StickyReptation()
  close all; clc;
  addpath('StickyRouse') % Tranform G(t) to G'(w) and G''(w)
  addpath('DoubleReptation')  % 
  addpath('getDynamicModuli') % Tranform G(t) to G'(w) and G''(w)

  %==============================================================
  % USER SETTINGS
  % A. Polymer properties
  Ze=100;       % Number of entanglements per chain
  tau_e=1e-6;  % Rouse time of entanglement strand
               % NOTE: tau_e not used if Zs>0 
  Zs=10;        % Number of stickers per chain
  tau_s=1e-2;  % Sticker dissociation time

  % B. Materials parameters
  DR_alpha=4.0;
  DR_beta=2.00; % default value for which data is precalculated

  % C. Experimental settings
  tL = 1e-4; % Shortest time (1/highest frequency)
  tU = 1e4;  % Longest time  1/lowest frequency) 
  Nt= 40;
  % D. Numerical parameter values
  nFEM=40;           % Number of frequencies to calculate G(w)
  initialise_DR=0; % 0: load pre-calculated  Double Reptation
                   % 1: new  pre-calculation Double Reptation; 
  % DR precalculation parameters 
  DR_Nsamples=40; % Number of samples: g(x) and F(U)
  DR_tol=1e-4;    % Numerical tolerance for g(x) and F(U) calculation
  DR_NH =64;       % Number of samples H parameter
  DR_Nt =128;   % Number of data points
  DR_HL=1e-2;  DR_HU=1e2; % Interpolation interval: H
  DR_tL=1e-6;  DR_tU=1e2; % Interpolation interval: t
  % END DR precalculation parameters 
  %==============================================================

  if initialise_DR==0 % Load precalculated DR model
    load('DoubleReptation/Precalculations');
  elseif initialise_DR==1
    DOUBLE_REPTATION = initialise_DoubleReptation( DR_Nsamples, DR_Nt, DR_NH, DR_tol, DR_HL, DR_HU, DR_tL, DR_tU);
    save('DoubleReptation/Precalculations', 'DOUBLE_REPTATION');
  end

  % Time range for G(t)
  trow=10.^(linspace(log10(tL),log10(tU), Nt));

  % Physical time scales
  if Zs==0
    tau_d0=tau_e*Ze^3;    % Reptation time
  elseif (Zs>0)
    tau_d0=tau_s*Zs^2*Ze; % Sticky-reptation time
    tS=tau_s*(Zs)*(Zs);     % Relaxation time of sticker strand
  else
    error('Zs cannot be negative.\n');
  end
  fprintf('reptation time: %e\n', tau_d0);


  % REPTATION
  Grep=zeros(size(trow));  % In case reptation is switched off

  % Double reptation
  H    = (Ze/DR_alpha);
  tt   = (trow/tau_d0);
  GDR  =10.^((DR_beta/2)*Grep_desCloizeaux_interpolate(tt, H, DOUBLE_REPTATION));
  G1G2DR  =getDynamicModuli_EvansTassieri(trow, GDR, nFEM);
  w=G1G2DR(:,1); % Angular frequencies

  % Sticky Rouse
  if Zs>0
    G1G2SR=G1G2_StickyRouse(w, Ze, Zs );        % relaxation modulus
    GSR    = Gt_StickyRouse(trow/tS, Ze, Zs );  % dynamic moduli
  else 
    G1G2SR=0; GSR=0;
  end

  % Add contributions
  Gt  =GDR+GSR;        % relaxation modulus
  G1G2=G1G2DR+G1G2SR;  % dynamic moduli

   
  % derivative (Scaled as in Likhtman-McLeish 2002)
  trow_d=zeros(1,length(trow)-1);
  Gt_d =zeros(1,length(trow)-1);
  for i =1:length(trow)-1
    trow_d(i)=trow(i+1);
    dt=trow(i+1)-trow(i);
    Gt_d(i)=-4*Ze*tau_e^0.25*trow(i+1)^0.75*( Gt(i+1)-Gt(i) )/dt;
  end


  %===========================================================
  % Plot results
  figure
  subplot(2,2,1)
  loglog((trow(Gt>0)), (Gt(Gt>0)), 'k', 'LineWidth', 2); hold on
  loglog((trow(GSR>1e-16)), (GSR(GSR>1e-16)), 'b', 'LineWidth', 2); hold on
  loglog((trow(Gt>0)), (GDR(Gt>0)), 'g', 'LineWidth', 2); hold on
  axis([tau_e, 2*tau_d0, 1e-5, 2])
  legend(sprintf('Double Reptation; alpha=%.1f, beta=%.2f', DR_alpha, DR_beta), 'sticky rouse', 'double reptation', 'Location', 'SouthWest')
  xlabel('time')
  ylabel('log_{10}G(t)')

  subplot(2,2,2)
  plot( log10(trow_d(Gt_d>0)), Gt_d(Gt_d>0), 'k', 'LineWidth', 2 ); hold on;
  xlabel('time')
  ylabel('dG')

  %loglog(trow, Gsr, 'b', 'LineWidth', 2); hold on;
  %loglog(trow, Grep, 'g', 'LineWidth', 2); hold on;
 % legend('G_{tot}', 'G_{sr}') % , 'G_{rep}'


  subplot(2,2,3:4)
  loglog(G1G2(G1G2(:,2)>0,1), G1G2(G1G2(:,2)>0,2), 'r', 'LineWidth', 2); hold on;
  loglog(G1G2(G1G2(:,3)>0,1), G1G2(G1G2(:,3)>0,3), 'g', 'LineWidth', 2); hold on;
  %axis([0.5/tau_d0, 20/tau_s, 1e-7, 2])
  xlabel('frequency')
  ylabel('G`(w), G``(w)')

 
end

