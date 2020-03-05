% Charley Schaefer 2020-02-18
% implementation of double repation
% REF: J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
% Calculate F(U) - first initialise SET_DESCLOIZEAUX using 
%                  initialise_desCloizeaux.m
function F=F_descloizeaux_interpolate(U, SET_DESCLOIZEAUX)
  if U<SET_DESCLOIZEAUX.UL || U>SET_DESCLOIZEAUX.UU
    F=F_descloizeaux(U,SET_DESCLOIZEAUX.tol, ...
                       SET_DESCLOIZEAUX.UL,  ...
                       SET_DESCLOIZEAUX.UU);
  else
    F=interp1(SET_DESCLOIZEAUX.FTABLE(:,1), ...
              SET_DESCLOIZEAUX.FTABLE(:,2), U, 'cubic');
  end
end

