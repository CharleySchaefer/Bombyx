set terminal pngcairo enhanced dashed
set output "img/Chi2Landscape.png"

# INPUT DATA COLUMNS:
#     Index ChiSquare Param1 Param2 Param3
#     
#  x,y,z axes are given by Param1, Param2 and Param3
#   colorscale is given by ChiSquare



set log x
set xtics offset 1.5,-0.7
set xlabel "tau_S [s]" 
XMIN=1e-3
XMAX=1
set xrange [XMIN:XMAX]

set log y
set ytics offset 1.1,-0.4
set ylabel "Ze"; 
YMIN=1
YMAX=100
set yrange [YMIN:YMAX]

set log z
set zlabel "Zs";
ZMIN=1e-1
ZMAX=100
set zrange [1e-1:100]


set for [y = 10:10:10] arrow from XMIN, y, ZMIN to XMIN,   y, ZMAX nohead lc rgb "black" lt 0 back
set for [log10x = (log10(XMIN)):(log10(XMAX)):(1)] arrow from (10**(log10x)), YMAX, ZMIN to (10**(log10x)), YMAX, ZMAX nohead lc rgb "black" lt 0 back

set grid;
set grid ztics
set grid back

set border 4095
set view 60, 60, 1, 1.25
set xyplane at 0.1
#set palette color
#set colorbox user origin .9, .23, 0 size 0.03, 0.45, 0
#set tics out

set key at screen 0.85,0.95

set multiplot

ZFAC=0.1
splot "bbx_convergence.out" u 3:4:(ZFAC*$5/$5) w l lc rgb "black" notitle,\
"" u 3:4:($2<2.0   ? ZFAC:1/0) w p pt 1 lc rgb "#88FF88" title "X^2=2",\
"" u 3:4:($2<1.0   ? ZFAC:1/0) w p pt 1 lc rgb "#88DD88" notitle,\
"" u 3:4:($2<0.5   ? ZFAC:1/0) w p pt 1 lc rgb "#88BB88" notitle,\
"" u 3:4:($2<0.2   ? ZFAC:1/0) w p pt 1 lc rgb "#889988" notitle,\
"" u 3:4:($2<0.1   ? ZFAC:1/0) w p pt 1 lc rgb "#998888" notitle,\
"" u 3:4:($2<0.05   ? ZFAC:1/0) w p pt 1 lc rgb "#BB8888" notitle,\
"" u 3:4:($2<0.02? ZFAC:1/0) w p pt 1 lc rgb "#DD8888" notitle,\
"" u 3:4:($2<0.01? ZFAC:1/0) w p pt 1 lc rgb "#FF8888" title "X^2<=0.01"

YFAC=100
splot "bbx_convergence.out" u 3:($4/$4*YFAC):($5) w l lc rgb "black" notitle,\
"" u 3:($4/$4*YFAC):($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#88FF88" title "X^2=2",\
"" u 3:($4/$4*YFAC):($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#88DD88" notitle,\
"" u 3:($4/$4*YFAC):($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#88BB88" notitle,\
"" u 3:($4/$4*YFAC):($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#889988" notitle,\
"" u 3:($4/$4*YFAC):($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#998888" notitle,\
"" u 3:($4/$4*YFAC):($2<0.05   ? $5:1/0) w p pt 1 lc rgb "#BB8888" notitle,\
"" u 3:($4/$4*YFAC):($2<0.02? $5:1/0) w p pt 1 lc rgb "#DD8888" notitle,\
"" u 3:($4/$4*YFAC):($2<0.01? $5:1/0) w p pt 1 lc rgb "#FF8888" title "X^2<=0.01"

XFAC=1e-3
splot "bbx_convergence.out" u ($3/$3*XFAC):($4):($5) w l lc rgb "black" notitle,\
"" u ($3/$3*XFAC):($4):($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#88FF88" title "X^2=2",\
"" u ($3/$3*XFAC):($4):($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#88DD88" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#88BB88" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#889988" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#998888" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.05   ? $5:1/0) w p pt 1 lc rgb "#BB8888" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.02? $5:1/0) w p pt 1 lc rgb "#DD8888" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.01? $5:1/0) w p pt 1 lc rgb "#FF8888" title "X^2<=0.01"

splot "bbx_convergence.out" u 3:4:5 w l lc rgb "black" notitle,\
"" u 3:4:($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#44FF44" title "X^2=2",\
"" u 3:4:($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#44DD44" notitle,\
"" u 3:4:($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#44BB44" notitle,\
"" u 3:4:($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#449944" notitle,\
"" u 3:4:($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#994444" notitle,\
"" u 3:4:($2<0.05   ? $5:1/0) w p pt 1 lc rgb "#BB4444" notitle,\
"" u 3:4:($2<0.02? $5:1/0) w p pt 1 lc rgb "#DD4444" notitle,\
"" u 3:4:($2<0.01? $5:1/0) w p pt 1 lc rgb "#FF4444" title "X^2<=0.01"



unset multiplot

