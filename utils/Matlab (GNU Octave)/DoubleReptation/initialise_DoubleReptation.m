% Charley Schaefer 2020-02-18
% implementation of double repation
% REF: J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687

%  Nsamples=40; % Number of samples: g(x) and F(U)
%  Nt=64;       % Number of samples time 
%  NH=16;       % Number of samples H parameter
%  tol=1e-4;    % Numerical tolerance for g(x) and F(U) calculation
%  HL=1e-2;  HU=1e2; % Interpolation interval: H
%  tL=1e-6;  tU=1e2; % Interpolation interval: H
function DOUBLE_REPTATION = initialise_DoubleReptation(Nsamples, Nt, NH, tol, HL, HU, tL, tU)
    % Pre-calculate values
  SET_DESCLOIZEAUX=initialise_desCloizeaux(tol, Nsamples);


  Hrow=10.^linspace(log10(HL), log10(HU), NH);
  trow=10.^linspace(log10(tL), log10(tU), Nt);
  T_intp=zeros(NH, Nt );
  H_intp=zeros(NH, Nt );
  G_intp=zeros(NH, Nt );

  [T_int, H_int]=meshgrid( trow, Hrow);

  for j=1:NH
    h =Hrow(j);
    for i=1:Nt
      tt=trow(i); 
      T_intp(j,i)=log10(tt);
      H_intp(j,i)=log10(h);
      G_intp(j,i)=log10(Grep_desCloizeaux(tt, h, SET_DESCLOIZEAUX));
    end
  end

  DOUBLE_REPTATION.NH=NH;
  DOUBLE_REPTATION.HL=HL;
  DOUBLE_REPTATION.HU=HU;
  DOUBLE_REPTATION.Nt=Nt;
  DOUBLE_REPTATION.tL=tL;
  DOUBLE_REPTATION.tU=tU;
  DOUBLE_REPTATION.T=T_intp;
  DOUBLE_REPTATION.H=H_intp;
  DOUBLE_REPTATION.G=G_intp;
end
