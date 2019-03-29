% https://github.com/CharleySchaefer/ZiltoidLIB
% bisection(): Search for scalar x0 in an array
%
% INPUT
%   x:  Array with monotonously ascending or descending numbers
%   x0: Scalar value
% OUTPUT
%   k:  Index in the array, such that x(k)<=x0<=x(k+1)
%   f:  Fraction such that x0 = (1-f)*x(k) + f*x(k+1)
% DEMO:
%   see demo_bisection()  
function [k, f] = bisection(x, x0) % SEE BELOW FOR DEMO
  N=length(x);
  if x(N)>x(1) % Ascending numbers
    mode=1;
  elseif x(N)<x(1)
    mode=2;    % Descending numbers
  else
    mode=0; k=1; f=1;
  end

  if mode==1      % Ascending numbers
    iL=1; iU=N;
    while (iU-iL)>1
      inew=round( 0.5*(iL+iU) );
      if x(inew)<x0
        iL=inew;
      elseif x(inew)>x0
        iU=inew;
      else
        iU=inew;iL=inew;
      end
    end
    k=iL;
    f= (x0-x(iL))/(x(iU)-x(iL));

  elseif mode==2  % Descending numbers
    iU=1; iL=N;
    while (iL-iU)>1
      inew=round( 0.5*(iL+iU) );
      if x(inew)<x0
        iL=inew;
      elseif x(inew)>x0
        iU=inew;
      else
        iU=inew;iL=inew;
      end
    end
    k=iU; 
    f= (x0-x(iU))/(x(iL)-x(iU));

  end
end


