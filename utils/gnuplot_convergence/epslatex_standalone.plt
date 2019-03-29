#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer



#==================================
# TERMINAL SETTINGS
#set terminal pngcairo enhanced dashed
#set output "Fig_LvsR.png"

set terminal epslatex standalone
set output "tmp_figure.tex"
#==================================


#==================================
# FUNCTIONS AND DEFINITIONS
#----------------------------------
#FAC225=(1.0/0.021)**0.33 
#FAC200=(1.0/0.045)**0.33 
#FAC180=(1.0/0.128)**0.33
#FAC176=(1.0/0.02)**0.33
#FAC100=(1.0/0.128)**0.33
#==================================


#==================================
# MARGINS
#----------------------------------
#set lmargin 8
#set tmargin 8
#set bmargin 4
#set rmargin 4
#==================================


#==================================
# X AXIS
#----------------------------------
#set xlabel "\\huge $\\nu_\{\\mathrm\{ads\}\}/\\nu_\{\\mathrm\{hop\}\}$"
#set xrange [2e-7:0.002]
#set log x
#set xtics 1e-7,10,0.1
#set format x "$10^\{%T\}$"

set xlabel "\\Large $\\tau_S$ [s]" offset -0.5,-1

set xrange [1e-3:1]
set log x
set xtics offset 1.5,-0.7
#==================================


#==================================
# Y AXIS
#----------------------------------
set log y
set ytics offset 1.1,-0.4
set ylabel "\\Large $Z_\\mathrm\{e\}$" offset 1,-1
set yrange [1:100]

#set ylabel "\\huge $R_0$"
#set yrange [8:100]
#set log y
# set ytics
#set format y "$10^\{%T\}$"
#==================================

#==================================
# Z AXIS
#----------------------------------
set log z
set zlabel "\\Large $Z_\\mathrm\{s\}$" offset 2,0
set zrange [1e-1:100]

#set ylabel "\\huge $R_0$"
#set yrange [8:100]
#set log y
# set ytics
#set format y "$10^\{%T\}$"
#==================================


#==================================
# KEY
#----------------------------------
set key at screen 0.85,0.95
#==================================


#==================================
# LABELS AND ARROWS
#----------------------------------
#set label "\\Large \\textcolor[rgb]\{0.7333,0,0\}\{$-1/3$\}" at 1.6e-6, 70
#set label "\\Large \\textcolor[rgb]\{0,0.7333,0\}\{$-1/4$\}" at 4e-4, 14
#set arrow from 2e-4,40 to 2e-4,2 lw 2 front
#==================================


#==================================
# PLOT
#----------------------------------


set grid;
#set grid back
set border 4095
set view 60, 60, 1, 1.25
set xyplane at 0.1
#set palette color
#set colorbox user origin .9, .23, 0 size 0.03, 0.45, 0
#set tics out

set key at screen 0.85,0.95

set multiplot

ZFAC=0.1
splot "demo_bbx_convergence.out" u 3:4:(ZFAC*$5/$5) w l lc rgb "black" notitle,\
"" u 3:4:($2<2.0   ? ZFAC:1/0) w p pt 1 lc rgb "#88FF88" notitle,\
"" u 3:4:($2<1.0   ? ZFAC:1/0) w p pt 1 lc rgb "#88DD88" notitle,\
"" u 3:4:($2<0.5   ? ZFAC:1/0) w p pt 1 lc rgb "#88BB88" notitle,\
"" u 3:4:($2<0.2   ? ZFAC:1/0) w p pt 1 lc rgb "#889988" notitle,\
"" u 3:4:($2<0.1   ? ZFAC:1/0) w p pt 1 lc rgb "#998888" notitle,\
"" u 3:4:($2<0.05   ? ZFAC:1/0) w p pt 1 lc rgb "#BB8888" notitle,\
"" u 3:4:($2<0.02? ZFAC:1/0) w p pt 1 lc rgb "#DD8888" notitle,\
"" u 3:4:($2<0.01? ZFAC:1/0) w p pt 1 lc rgb "#FF8888" notitle

unset xlabel
unset ylabel
unset zlabel
unset grid
unset xtics
unset ytics
unset ztics

YFAC=100
splot "demo_bbx_convergence.out" u 3:($4/$4*YFAC):($5) w l lc rgb "black" notitle,\
"" u 3:($4/$4*YFAC):($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#88FF88" notitle,\
"" u 3:($4/$4*YFAC):($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#88DD88" notitle,\
"" u 3:($4/$4*YFAC):($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#88BB88" notitle,\
"" u 3:($4/$4*YFAC):($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#889988" notitle,\
"" u 3:($4/$4*YFAC):($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#998888" notitle,\
"" u 3:($4/$4*YFAC):($2<0.05   ? $5:1/0) w p pt 1 lc rgb "#BB8888" notitle,\
"" u 3:($4/$4*YFAC):($2<0.02? $5:1/0) w p pt 1 lc rgb "#DD8888" notitle,\
"" u 3:($4/$4*YFAC):($2<0.01? $5:1/0) w p pt 1 lc rgb "#FF8888" notitle

XFAC=1e-3
splot "demo_bbx_convergence.out" u ($3/$3*XFAC):($4):($5) w l lc rgb "black" notitle,\
"" u ($3/$3*XFAC):($4):($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#88FF88" notitle,\
"" u ($3/$3*XFAC):($4):($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#88DD88" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#88BB88" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#889988" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#998888" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.05   ? $5:1/0) w p pt 1 lc rgb "#BB8888" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.02? $5:1/0) w p pt 1 lc rgb "#DD8888" notitle,\
"" u ($3/$3*XFAC):($4):($2<0.01? $5:1/0) w p pt 1 lc rgb "#FF8888" notitle

splot "demo_bbx_convergence.out" u 3:4:5 w l lc rgb "black" notitle,\
"" u 3:4:($2<2.0   ? $5:1/0) w p pt 1 lc rgb "#44FF44" title "$\\chi^2=2$",\
"" u 3:4:($2<1.0   ? $5:1/0) w p pt 1 lc rgb "#44DD44" notitle,\
"" u 3:4:($2<0.5   ? $5:1/0) w p pt 1 lc rgb "#44BB44" notitle,\
"" u 3:4:($2<0.2   ? $5:1/0) w p pt 1 lc rgb "#449944" notitle,\
"" u 3:4:($2<0.1   ? $5:1/0) w p pt 1 lc rgb "#994444" notitle,\
"" u 3:4:($2<0.05   ? $5:1/0) w p pt 1 lc rgb "#BB4444" notitle,\
"" u 3:4:($2<0.02? $5:1/0) w p pt 1 lc rgb "#DD4444" notitle,\
"" u 3:4:($2<0.01? $5:1/0) w p pt 1 lc rgb "#FF4444" title "$\\chi^2=0.01$"



unset multiplot
#==================================





