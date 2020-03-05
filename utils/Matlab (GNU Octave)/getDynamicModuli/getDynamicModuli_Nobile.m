%==========================================
% GETDYNAMICMODULI - Charley Schaefer 2020
%   Get dynamic moduli G'(w) and G''(w) from 
%   relaxation modulus G(t).
%   Implementation of finite element method
%   by Nobile & Cocchini, Rheologica Acta, 
%   40, 111 (2001), doi:10.1007/s003970000141
% INPUT
%   trow: array - time
%   Grop: array - relaxation modulus at time t
%   nFEM: int   - number of elements to do Finite Element Analysis
% OUTPUT
%   G1G2: array of size nFEM x 3
%     G1G2(:, 1): frequencies
%     G1G2(:, 2): elastic modulus, G'
%     G1G2(:, 3): viscous modulus, G''
function G1G2=getDynamicModuli_Nobile(trow, Grow, nFEM)
  G1G2 = zeros(nFEM, 3); % time, G', G''

  % Frequencies
  wp = 10.^linspace( log10(1 / max(trow)), log10(1 / min(trow)), nFEM);
  G1G2(:, 1) = wp(:);

  for i=1:length(wp)
    wt=wp(i)*trow;
    G1G2(i, 2) = H1fnc(wt)*Grow'; % Elastic Modulus
    G1G2(i, 3) = H2fnc(wt)*Grow'; % Viscous Modulus
  end
end

function H1=H1fnc(wt)
  H1=zeros(size(wt));
  H1(1)  =h1fnc(wt(2),wt(1));
  for i=2:length(H1)-1
    H1(i)=h1fnc(wt(i+1), wt(i))-h1fnc(wt(i-1), wt(i));
  end
  H1(end)=-h1fnc(wt(end-1),wt(end));
end
function H2=H2fnc(wt)
  H2=zeros(size(wt));
  H2(1)  =h2fnc(wt(2),wt(1));
  for i=2:length(H2)-1
    H2(i)=h2fnc(wt(i+1), wt(i))-h2fnc(wt(i-1), wt(i));
  end
  H2(end)=-h2fnc(wt(end-1),wt(end));
end
function h1=h1fnc(wp,wq)
  h1= cos(wq)+(sin(wq)-sin(wp))/(wp-wq);
end
function h2=h2fnc(wp,wq)
  h2=-sin(wq)+(cos(wq)-cos(wp))/(wp-wq);
end
