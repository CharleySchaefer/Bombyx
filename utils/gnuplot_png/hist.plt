set terminal pngcairo enhanced
set output "test_hist.png"


binwidth=1
bin(x,width)=width*floor(x/width)

plot 'bbx_convergence.out' using ($2<0.1 ? bin($4,binwidth) : 1/0):(1.0) smooth freq with boxes
