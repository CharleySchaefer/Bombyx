%===========================================================
% Charley Schaefer 2020-02-18
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
