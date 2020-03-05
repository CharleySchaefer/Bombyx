% Charley Schaefer 2020-02-18
% implementation of double repation
% REF: J. des Cloizeaux, Macromolecules 1990, 23, 4618-4687
% Calculate g(x) - first initialise SET_DESCLOIZEAUX using 
%                  initialise_desCloizeaux.m
function g=g_descloizeaux_interpolate(x, SET_DESCLOIZEAUX)
  if x<SET_DESCLOIZEAUX.xL || x>SET_DESCLOIZEAUX.xU
    g=g_descloizeaux(x,SET_DESCLOIZEAUX.tol, ...
                       SET_DESCLOIZEAUX.xL,  ...
                       SET_DESCLOIZEAUX.xU);
  else
    g=interp1(SET_DESCLOIZEAUX.GTABLE(:,1), ...
              SET_DESCLOIZEAUX.GTABLE(:,2), x, 'cubic');
  end
end

