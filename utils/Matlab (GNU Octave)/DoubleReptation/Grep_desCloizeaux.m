% Charley Schaefer 2020-02-18
% tt: time in units of the reptation time tau_rep
%       for conventional reptation: 
%        tau_rep=tau_e*Ze^3,
%                with tau_e Rouse time of an entangled strand, 
%                and Ze the number of entanglements per chain
%       for sticky reptation:       
%        tau_rep=tau_s*(Zs/2)^2*Ze
%                with tau_s the sticker dissociation time, 
%                and Zs the number of stickers per chain
% H:    Ze/alpha, with Ze as above, and alpha a constant
% Grep: Relaxation modulus
% SET_DESCLOIZEAUX: has to be pre-evaluated using initialise_desCloizeaux.m
function Grep=Grep_desCloizeaux(tt, H, SET_DESCLOIZEAUX)
  U = tt+g_descloizeaux_interpolate(H*tt, SET_DESCLOIZEAUX)/H;
  F =    F_descloizeaux_interpolate(U,    SET_DESCLOIZEAUX);
  Grep=F*F;
end
