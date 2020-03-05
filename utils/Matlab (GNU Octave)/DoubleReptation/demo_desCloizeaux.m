% Charley Schaefer 2020-02-18
% 
% Implementation of double-reptation model
% J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
%
% ALGORITHM
% 1. initialise_desCloizeaux(tol, Nsamples):
%    Precalculate values of g(x) and F(U) within intervals [xL, xU]
%    and [UL, UU], respectively. The number of precalculated 
%    values is 'Nsamples', and the intervals are automatically
%    set using a numerical tolerance 'tol'
% 2. g_descloizeaux_interpolate(), F_descloizeaux_interpolate(): 
%    For values of x and U within the intervals interpolate.
%    Outside these intervals, extrapolate using the asymptotic limits
%    of g(x) and F(U), which are known analytically.
function demo_desCloizeaux()
  close all; clc;
  % User Settings to pre-calculate
  tol=1e-4;    % Tolerance
  Nsamples=40; % 

  % Pre-calculate values
  SET_DESCLOIZEAUX=initialise_desCloizeaux(tol, Nsamples);
  x=SET_DESCLOIZEAUX.GTABLE(:,1);
  g=SET_DESCLOIZEAUX.GTABLE(:,2);
  U=SET_DESCLOIZEAUX.FTABLE(:,1);
  F=SET_DESCLOIZEAUX.FTABLE(:,2);

  % Interpolated and extrapolated values
  xL=SET_DESCLOIZEAUX.xL; % lower boundary for interpolation
  xU=SET_DESCLOIZEAUX.xU; % upper boundary for interpolation
  UL=SET_DESCLOIZEAUX.UL; % lower boundary for interpolation
  UU=SET_DESCLOIZEAUX.UU; % upper boundary for interpolation

  xint=10.^linspace(log10(0.1*xL), log10(10*xU), 200);
  Uint=10.^linspace(log10(0.1*UL), log10(10*UU), 200);
  for i=1:200
    gint(i)=g_descloizeaux_interpolate(xint(i), SET_DESCLOIZEAUX);
    Fint(i)=F_descloizeaux_interpolate(Uint(i), SET_DESCLOIZEAUX);
  end

  % Show results
  figure
  subplot(1,2,1)
  loglog(x, g, '.k', 'MarkerSize', 12); hold on
  loglog(xint, gint, 'r'); hold on
  loglog(x, -x+sqrt(x.*(x+sqrt(pi*x)+pi)), 'k');
  legend('pre-calculated', 'interpolated', '-x+sqrt(x.*(x+sqrt(pi*x)+pi))')
  xlabel('x')
  ylabel('g(x)')
  subplot(1,2,2)
  loglog(U, F, '.k', 'MarkerSize', 12); hold on
  loglog(Uint, Fint, 'r'); hold on
  legend('pre-calculated', 'interpolated', 'Location', 'SouthWest')
  xlabel('U')
  ylabel('F(U)')

  % Evaluate relaxation modulus G_rep(t, H),
  %  t: time in units of the reptation time tau_rep:
  %               tau_rep = tau_e*Ze^3 for regular polymers
  %               tau_rep = tau_s*Zs^2*Ze for sticky polymers
  %  H: parameter; equals Ze/alpha
  %
  %  OBSERVATIONS: 
  %    1. large values H (small alpha): G(t)   reaches asymptotic limit
  %    2. small values H (large alpha): G(H*t) reaches asymptotic limit
  %    Two show both limits, I scale time as t/(1+1/H) in the plots

  HL=1e-2; HU=1e2; % Boundary for interpolation
  NH=16; % Number of samples for interpolation
  tL=1e-6;  tU=1e2;
  Nt=64;

  Hrow=10.^linspace(log10(HL), log10(HU), NH);
  trow=10.^linspace(log10(tL), log10(tU), Nt);
  T_intp=zeros(NH, Nt );
  H_intp=zeros(NH, Nt );
  G_intp=zeros(NH, Nt );

  [T_int, H_int]=meshgrid( trow, Hrow);

  for j=1:NH
    h =Hrow(j);
    for i=1:Nt
      tt=trow(i); % Change t-range
      T_intp(j,i)=log10(tt);
      H_intp(j,i)=log10(h);
      G_intp(j,i)=log10(Grep_desCloizeaux(tt, h, SET_DESCLOIZEAUX)); % /(1+1/h)
    end
  end


  DOUBLE_REPTATION_TAB.NH=NH;
  DOUBLE_REPTATION_TAB.HL=HL;
  DOUBLE_REPTATION_TAB.HU=HU;
  DOUBLE_REPTATION_TAB.Nt=Nt;
  DOUBLE_REPTATION_TAB.tL=tL;
  DOUBLE_REPTATION_TAB.tU=tU;
  DOUBLE_REPTATION_TAB.T=T_intp;
  DOUBLE_REPTATION_TAB.H=H_intp;
  DOUBLE_REPTATION_TAB.G=G_intp;

  figure
  for j=1:NH
    h =Hrow(j);
    %  tt=trow(i);
    %  T_intp(j,i)=log10(tt);
    %  H_intp(j,i)=h;
    %  G_intp(j,i)=log10(Grep_desCloizeaux(tt, h, SET_DESCLOIZEAUX)); % /(1+1/h)

    subplot(2,1,1)
    loglog(10.^T_intp(j,G_intp(j,:)>-20), 10.^G_intp(j,G_intp(j,:)>-20), ...
           '-k', 'LineWidth', 1); hold on
    loglog(10.^T_intp(j,G_intp(j,:)>-20), 10.^G_intp(j,G_intp(j,:)>-20), ...
           '.k', 'MarkerSize', 12); hold on
    xlabel('time/tau_{rep}')
    ylabel('G(t)')
    subplot(2,1,2)
    loglog(10.^T_intp(j,G_intp(j,:)>-20)*(1+1/h), 10.^G_intp(j,G_intp(j,:)>-20), ...
           '-k', 'LineWidth', 1); hold on
    loglog(10.^T_intp(j,G_intp(j,:)>-20)*(1+1/h), 10.^G_intp(j,G_intp(j,:)>-20), ...
           '.k', 'MarkerSize', 10); hold on
    xlabel('(1+1/H)time/tau_{rep} ')
    ylabel('G(t)')
  end

  % INTERPOLATE
  trow=10.^linspace((-3), (1), 20);
  h=0.005;

  tic
  G=Grep_desCloizeaux_interpolate(trow, h, DOUBLE_REPTATION_TAB);
  toc


  subplot(2,1,1)
  loglog(trow(G>1e-20), G(G>1e-20), '-r', 'LineWidth', 3)
  subplot(2,1,2)
  loglog(trow(G>1e-20)*(1+1/h), G(G>1e-20), '-r', 'LineWidth', 3)
  %legend('H=0.0001','0.001','0.01', '0.1', '1', '10', '100', '1000', 'Location', 'SouthWest')
end
