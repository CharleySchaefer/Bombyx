function log10Grep=Grep_desCloizeaux_interpolate(trow, H, DOUBLE_REPTATION_TAB)
  Nt=length(trow);
  log10Grep=zeros(size(trow));

  % Upper and lower boundaries
  HL=DOUBLE_REPTATION_TAB.HL;
  HU=DOUBLE_REPTATION_TAB.HU;
  tL=DOUBLE_REPTATION_TAB.tL;
  tU=DOUBLE_REPTATION_TAB.tU;
  if H<HL     % Extrapolate for small H values
    log10H=log10(HL);
    hfac=(1+1/H)/(1+1/HL);
    for i=1:Nt
      tt=trow(i)*hfac; %(renormalise time using h_fac)
      if tt<tL     % extrapolate for short times
        log10Grep(i)=0; 
      elseif tt>tU % extrapolate for long times (BETTER ESTIMATE POSSIBLE?)
        log10Grep(i)=-32;
      else         % interpolate 
        log10Grep(i)=interp2(DOUBLE_REPTATION_TAB.T, ...
                           DOUBLE_REPTATION_TAB.H, ...
                           DOUBLE_REPTATION_TAB.G, log10(tt), log10H, 'linear' ); % 'linear' ~6 times faster than 'spline'
      end
    end
  elseif H>HU % Extrapolate for large H valuesfor i=1:Nt
    log10H=log10(HU);
    for i=1:Nt
      tt=trow(i);
      if tt<tL     % extrapolate for short times
        log10Grep(i)=0; 
      elseif tt>tU % extrapolate for long times (BETTER ESTIMATE POSSIBLE?)
        log10Grep(i)=-32;
      else         % interpolate
        log10Grep(i)=interp2(DOUBLE_REPTATION_TAB.T, ...
                           DOUBLE_REPTATION_TAB.H, ...
                           DOUBLE_REPTATION_TAB.G, log10(tt), log10H, 'linear' ); % 'linear' ~6 times faster than 'spline'
      end
    end
  else        % Interpolate H
    log10H=log10(H); 

    for i=1:Nt
      tt=trow(i);
      if tt<tL     % extrapolate for short times
        log10Grep(i)=0; 
      elseif tt>tU % extrapolate for long times (BETTER ESTIMATE POSSIBLE?)
        log10Grep(i)=-32;
      else         % interpolate
        log10Grep(i)=interp2(DOUBLE_REPTATION_TAB.T, ...
                           DOUBLE_REPTATION_TAB.H, ...
                           DOUBLE_REPTATION_TAB.G, log10(tt), log10H, 'linear' ); % 'linear' ~6 times faster than 'spline'
      end
    end
  end
end
