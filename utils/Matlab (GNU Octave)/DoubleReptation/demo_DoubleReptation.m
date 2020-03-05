% Charley Schaefer 2020-02-18
% 
% Implementation of double-reptation model (excluding Rouse terms)
% J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
%
% MODEL
% -----
%   Double reptation model only depends on two parameters:
%   Grep = Grep(tt, H)
%     tt :=  t/tau_rep
%     H  := Ze/alpha
%   Here, t is time, Ze is the number of entanglements per
%   polymer chain and alpha is a constant (usually order 10)
%   tau_rep is the reptation time. For regular polymers it is
%     tau_rep = tau_e*Ze^3, with tau_e the Rouse time of
%                           an entanglement
%   For sticky polymers it is
%     tau_rep = tau_s*Zs^2*Ze, with tau_s the sticker
%                              dissociation time and Zs
%                              the number of stickers per chain
%   Asymptotic Properties:
%     For H-->inf, Grep(tt,  H) reaches an  limit
%     For H-->0,   Grep(tt/H,H) reaches an asymptotic limit
%     In the second limit, the reptation time decreases with H^{-1} 
%
% ALGORITHM
% ---------
% > Do pre-calculation for various values of tt and H
%   and store in an interpolation table.
% > Interpolate or extrapolate.
%   To extrapolate for out-of-bound H values, the asymptotic
%   limits mentioned above in 'MODEL' are exploited.

function demo_DoubleReptation()
  close all; clc;

  %====================================================
  % PRECALCULATE DOUBLE-REPTATION MODEL
  % User Settings to pre-calculate
  Nsamples=40; % Number of samples: g(x) and F(U)
  tol=1e-4;    % Numerical tolerance for g(x) and F(U) calculation
  Nt=64;       % Number of samples time 
  NH=32;       % Number of samples H parameter
  HL=1e-2;  HU=1e2; % Interpolation interval: H
  tL=1e-6;  tU=1e2; % Interpolation interval: t

  % HOW TO CHOOSE THE VALUES:
  %  - Nsamples, tol, HL, HU, tL, tU are already optimised
  %  - Nt and NH may be varied to change the size of 
  %    the interpolation table; for a good interpolation
  %    scheme, the cpu time increases as log(Nt*NH)

  fprintf('INITIALISE.\n');
  tic
  DOUBLE_REPTATION = initialise_DoubleReptation( Nsamples, Nt, NH, tol, HL, HU, tL, tU);
  toc
  % PRECALCULATION DONE
  %====================================================

  % SHOW PRECALCULATED VALUES
  hfig=figure;
  plotDoubleReptationSamples(hfig, DOUBLE_REPTATION, -20)


  % INTERPOLATE
  trow=10.^linspace((-3), (1), 20);
  h=0.005;
  fprintf('INTERPOLATE.\n');
  for h=[0.002, 0.04, 0.1, 1, 4, 10, 20, 200]
    tic
    G=Grep_desCloizeaux_interpolate(trow, h, DOUBLE_REPTATION);
    toc

    subplot(2,1,1)
    loglog(trow(G>-20), 10.^G(G>-20), '-r', 'LineWidth', 3)
      xlabel('time/tau_{rep}')
     ylabel('G(t)')
     title('Interpolation')
     subplot(2,1,2)
    loglog(trow(G>-20)*(1+1/h), 10.^G(G>-20), '-r', 'LineWidth', 3)
    xlabel('(1+1/H)time/tau_{rep} ')
    ylabel('G(t)')

  end

  %=============================================
  %CASE EXAMPLE
  fprintf('EXAMPLE.\n');
  tau_e=1e-6; % Rouse relaxation time of entanglement strand 
  Ze   =14;     % Number of entanglements per chain
  tau_s=1e-2;
  Zs   =12;
  Ge   =8000;
  alpha=10;
  timerange=10.^linspace(-4,log10(30),100);

  
  % TRANSFORM PARAMETERS AND CALCULATE
  H=Ze/alpha;
  if Zs==0    % Double-reptation of regular polymer
    tau_rep = tau_e*Ze^3;
    fprintf('No stickers, reptation time = %e\n', tau_rep);
  elseif Zs>0 % Double-reptation of sticky polymer
    tau_rep = tau_s*Zs^2*Ze;
    fprintf('Sticky-reptation time = %e\n', tau_rep);
  end
  tt = timerange/tau_rep;
  log10G=log10(Ge) + Grep_desCloizeaux_interpolate(tt, h, DOUBLE_REPTATION);

  % PLOT
  figure
  loglog(timerange(log10G>-20), 10.^(log10G(log10G>-20)), 'k', 'LineWidth', 3);
  xlabel('time')
  ylabel('G(t)')
end


%==========================================================
function plotDoubleReptationSamples(hfig, DOUBLE_REPTATION, cutoff)
  figure(hfig);
  for j=1:DOUBLE_REPTATION.NH
    h =10^DOUBLE_REPTATION.H(j,1);

    subplot(2,1,1)
    loglog(10.^DOUBLE_REPTATION.T(j,DOUBLE_REPTATION.G(j,:)>cutoff), ...
           10.^DOUBLE_REPTATION.G(j,DOUBLE_REPTATION.G(j,:)>cutoff), ...
           '-k', 'LineWidth', 1); hold on
    loglog(10.^DOUBLE_REPTATION.T(j,DOUBLE_REPTATION.G(j,:)>cutoff), ...
           10.^DOUBLE_REPTATION.G(j,DOUBLE_REPTATION.G(j,:)>cutoff), ...
           '.k', 'MarkerSize', 12); hold on
    xlabel('time/tau_{rep}')
    ylabel('G(t)')
    subplot(2,1,2)
    loglog(10.^DOUBLE_REPTATION.T(j,DOUBLE_REPTATION.G(j,:)>cutoff)*(1+1/h),...
           10.^DOUBLE_REPTATION.G(j,DOUBLE_REPTATION.G(j,:)>cutoff), ...
           '-k', 'LineWidth', 1); hold on
    loglog(10.^DOUBLE_REPTATION.T(j,DOUBLE_REPTATION.G(j,:)>cutoff)*(1+1/h),...
           10.^DOUBLE_REPTATION.G(j,DOUBLE_REPTATION.G(j,:)>cutoff), ...
           '.k', 'MarkerSize', 10); hold on
    xlabel('(1+1/H)time/tau_{rep} ')
    ylabel('G(t)')
  end
end
