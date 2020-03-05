% time in units of the sticker dissociation time tau_s*Zs^2
function Gt=Gt_StickyRouse(t,Ze, Zs )
  Gt=0; tt=t;
  count=0; dsum=0.0;
  for q=1:(1+floor(Zs))
    count=count+1;
    if q<Ze
      dsum=dsum+0.2;
      Gt=Gt+0.2*exp(-tt*q*q);
    else
      dsum=dsum+1;
      Gt=Gt+    exp(-tt*q*q);
    end
  end
  Gt=Gt*count/(Ze*dsum);
end


