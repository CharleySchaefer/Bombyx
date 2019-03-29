set terminal pngcairo enhanced
set output "img/ZSconvergence.png"


set log x
set xtics offset 1.5,-0.7
set xlabel "iterations"  font ", 24"
#set xrange [1e-3:1]

set log y
set ytics offset 1.1,-0.4
set ylabel "Z_s" font ", 24"
#set yrange [1:100]



set grid;
#set grid back
set border 4095
set view 60, 60, 1, 1.25
set xyplane at 0.1
#set palette color
#set colorbox user origin .9, .23, 0 size 0.03, 0.45, 0
#set tics out

set key at screen 0.85,0.95


plot "bbx_convergence.out" u 1:5 w l lt 1 lw 2 lc rgb "#444444",\
"" u 1:($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#44FF44" title "X^2=2",\
"" u 1:($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#44DD44" notitle,\
"" u 1:($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#44BB44" notitle,\
"" u 1:($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#449944" notitle,\
"" u 1:($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#994444" notitle,\
"" u 1:($2<0.05  ? $5:1/0) w p pt 1 lc rgb "#BB4444" notitle,\
"" u 1:($2<0.02  ? $5:1/0) w p pt 1 lc rgb "#DD4444" notitle,\
"" u 1:($2<0.01  ? $5:1/0) w p pt 1 lc rgb "#FF4444" title "X^2<=0.01"


