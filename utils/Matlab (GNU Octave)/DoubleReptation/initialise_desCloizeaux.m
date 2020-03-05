% Charley Schaefer 2020-02-18
% implementation of double repation
% REF: J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
function SET_DESCLOIZEAUX=initialise_desCloizeaux(tol, Nsamples)
  % Algorithm:
  % 1. get interval to calculate g(x) and F(U) functions 
  %    (using tolerance tol)
  % 2. do Nsamples calculations on these intervals
  % 3. outside interval extrapolate (using analytical solutions) 
  tol=1e-4; % Tolerance

    % Get interval
  G_desCloizeaux_boundaries=get_g_descloizeaux_boundaries( tol );
  F_desCloizeaux_boundaries=get_F_descloizeaux_boundaries( tol );

  % Get samples on interval
  G_desCloizeaux_intp=zeros(Nsamples,2);
  F_desCloizeaux_intp=zeros(Nsamples,2);

  % x values
  xL=G_desCloizeaux_boundaries(1);
  xU=G_desCloizeaux_boundaries(2);
  UL=F_desCloizeaux_boundaries(1);
  UU=F_desCloizeaux_boundaries(2);
  G_desCloizeaux_intp(:,1)= 10.^linspace(log10(xL),log10(xU), Nsamples);
  F_desCloizeaux_intp(:,1)= 10.^linspace(log10(UL),log10(UU), Nsamples);

  % y values
  xrow=G_desCloizeaux_intp(:,1);
  Urow=F_desCloizeaux_intp(:,1);
  for i=1:Nsamples
    G_desCloizeaux_intp(i,2)=g_descloizeaux(xrow(i),tol, xL, xU);
    F_desCloizeaux_intp(i,2)=F_descloizeaux(Urow(i),tol, UL, UU);
  end

  SET_DESCLOIZEAUX.tol=tol;
  SET_DESCLOIZEAUX.N=Nsamples;
  SET_DESCLOIZEAUX.xL=xL;
  SET_DESCLOIZEAUX.xU=xU;
  SET_DESCLOIZEAUX.UL=UL;
  SET_DESCLOIZEAUX.UU=UU;
  SET_DESCLOIZEAUX.GTABLE=G_desCloizeaux_intp;
  SET_DESCLOIZEAUX.FTABLE=F_desCloizeaux_intp;
end


%===========================================================
% J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
%  limiting behavior (applied if x is outside the interval [xL, xU]):
%  g(x)=sqrt(pi*x) for x<<1
%  g(x)=(pi^2/6)         for x>>1 ( in this limit g=\sum_{m=1}^{\infty}m^{-2} )
%  Reasonable values for xL and xU can be used by running 
%  the function get_g_descloizeaux_boundaries()
function g = g_descloizeaux(x,tol, xL, xU)
  if(x<xL)
    g=sqrt(pi*x);
  elseif(x>xU)
    g=(pi^2/6);
  else
    err=2*tol;
    g=0.0; m=0;
    while err>tol
      m=m+1;
      m2=m*m;
      dg=(1.0-exp(-m2*x))/m2;
      g=g+dg;
      err=dg/g;
    end
  end
end

% Get boundaries between which g_descloizeaux() should be evaluated,
% and outside which the limits
%   g(x)=sqrt(pi*x)  for x<<1
%   g(x)=(pi^2/6)    for x>>1
% should be applied 
function boundaries = get_g_descloizeaux_boundaries( tol )
  % Determine convergence tolerance = dg_num/g_num
  % required to obey tol = (g_exact-g_num)/g_num
  gexact=pi*pi/6;
  g=0.0;   err=2*tol;   m=0;
  while (err>tol)
    m=m+1;
    dg=1.0/(m*m);
    g=g+dg;
    err=(gexact-g)/g;
  end
  convTol=dg/g;
  

  % Find range to satisfy tolerance level lower boundary
  x=1.0;
  err=2*tol;
  while (err>tol && ~isnan(err))
    x=0.5*x;
    g=g_descloizeaux(x, convTol, -inf, inf);
    err=abs( ( g-sqrt(pi*x) ));%/g )
    if (isnan(err))
      x=x*2
    end
  end
  xL=x; % Result: Lower boundary 

  % Find range to satisfy tolerance level right boundary
  x=1.0;
  err=2*tol;
  while (err>tol && ~isinf(x))
    g=g_descloizeaux(x, convTol, -inf, inf);
    err=abs( ( g-(pi*pi/6) )/g );
    x=x*2;
  end
  xU=x;% Result: Upper boundary
  boundaries=[xL,xU];
end

%===========================================================
% J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
%  F(U)=1 for U<<1 
%  F(U)=(8/pi^2)*exp(-U) for U>>1
function F=F_descloizeaux(U, tol, UL, UU)
  if U<UL
    F=1;
  elseif U>UU
    F=(8/pi^2)*exp(-U);
  else
  err=2*tol;
  F=0;
  q=1;
  while (err>tol)
    q2=q*q;
    dF=exp(-q2*U)/q2;
    F+=dF;
    err=dF/F;
    q=q+2;
  end
  F=F*8/(pi*pi);
  end
end

function boundaries = get_F_descloizeaux_boundaries(tol)
  %Determine convergence tolerance = dg_num/g_num
  %required to obey tol = (g_exact-g_num)/g_num
  F=0.0;
  err=2*tol;
  Fexact=pi*pi/8;
  q=1;
  while(err>tol)
    dF=1.0/(q*q);
    F=F+dF;
    err=(Fexact-F)/F;
    q=q+2;
  end
  convTol=dF/F;
  % lower boundary
  U=1.0;
  err=2*tol;
  while (err>tol && ~isnan(err))
    U*=0.5;
    F=F_descloizeaux(U, convTol, -inf,inf);
    err=abs(( F-1.0 )/F );  
    if(isnan(err))
      U*=2;
    end
  end
  UL=U;
  % upper boundary
  U=1.0;
  err=2*tol;
  while (err>tol && ~isinf(U))
    F=F_descloizeaux(U, convTol, -inf,inf);
    err=abs( ( F-(8.0/(pi*pi)*exp(-U)) )/F);
    U*=2;
  end
  UU=U;
  boundaries=[UL,UU];
end

