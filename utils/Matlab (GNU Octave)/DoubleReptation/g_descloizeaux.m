%===========================================================
% Charley Schaefer 2020-02-18
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
