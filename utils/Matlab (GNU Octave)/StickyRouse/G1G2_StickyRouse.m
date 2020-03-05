% time in units of tau_s*Zs^2
function G1G2=G1G2_StickyRouse(w, Ze, Zs )
  M=size(w);
  if(M(1)>1) % transpose row to column 
    w=w';
  end
  
  Nw=length(w);
  G1G2=zeros(Nw,3);
  G1G2(:,1)=w(:);
  if Zs>0
  w2=w.^2;
  
  count=0; dsum=0.0;
  for q=1:(1+floor(Zs))
    count=count+1;
    q2i=1/(q*q);
    q4i=q2i*q2i;
    if q<Ze
      dsum=dsum+0.2;
      G1G2(:,2)=G1G2(:,2)+0.2*(w2'*q4i)./( (w2'*q4i)+1);
      G1G2(:,3)=G1G2(:,3)+0.2*( w'*q2i)./( (w2'*q4i)+1);
    else
      dsum=dsum+1;
      G1G2(:,2)=G1G2(:,2)+(w2'*q4i)./((w2'*q4i)+1);
      G1G2(:,3)=G1G2(:,3)+( w'*q2i)./((w2'*q4i)+1);
    end
  end
  G1G2(:,2:3)=G1G2(:, 2:3)*count/(Ze*dsum);
  end
end


