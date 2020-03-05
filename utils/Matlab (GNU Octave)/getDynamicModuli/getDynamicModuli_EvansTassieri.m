%==========================================
% GETDYNAMICMODULI - Charley Schaefer 2020
function G1G2=getDynamicModuli_EvansTassieri(trow, Gt, nFEM)
  % Evans, R. M. L.; Tassieri, M.; Auhl, D.; Waigh, T. A. Directconversion of rheological compliance measurements into storage andloss moduli.Phys. Rev. E2009,80, 012501.(13) Evans, R. M. L. Transforming from time to frequency withoutartefacts.Br. Soc. Rheol. Bull.2009,50, 76.
  wrow=10.^(linspace(-log10(max(trow)), -log10(max(trow(1))), nFEM)); 
  G1G2=zeros(length(wrow), 3);
  G1G2(:,1)=wrow(:);
  G1=zeros(size(wrow));
  G2=zeros(size(wrow));
  Gt0=interp1(trow, Gt, 0, 'cubic', 'extrap');

  for w =1:length(wrow); 
    wt=wrow(w)*trow;
    wt1=wt(1);
    G1G2(w, 2)=Gt0 + (  sin(wt1))*(Gt(1)-Gt0)/wt1;
    G1G2(w, 3)=    + (1-cos(wt1))*(Gt(1)-Gt0)/wt1;
    for k=2:length(trow)
      coeff=(Gt(k)-Gt(k-1))/(wt(k)-wt(k-1));
      G1G2(w, 2)=G1G2(w, 2)+coeff*(sin(-wt(k-1))-sin(-wt(k)));
      G1G2(w, 3)=G1G2(w, 3)-coeff*(cos(-wt(k-1))-cos(-wt(k)));
    end
  end
end

