#   Bombyx: Modelling of linear viscoelasticity
#   Copyright (C) 2019 Charley Schaefer
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
set xlabel "\\Large $\\omega$ [rad/s]"
set log x
set xrange [0.4: 200]
#set xtics 1e-6, 1e3, 1e12
set format x "$10^\{%T\}$"
#==================================


#==================================
# Y AXIS
#----------------------------------
set ylabel "\\Large $G', G''$ [Pa s]"
set log y
set yrange [200:20000]
#set ytics 1e-3, 1e3, 1e9
set format y "$10^\{%T\}$"
#==================================



#==================================
# KEY
#----------------------------------
set key right bottom 
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

set multiplot 
set grid

plot "reformatted_data.bbx" u 3:4 w p pt 7 ps 3 lc rgb "#EE4444" title "$G'$",\
"bbx_model.out" u 1:3 w l lt 1 lw 4 lc rgb "#AA4444" notitle,\
"reformatted_data.bbx"  u 1:2 w p pt 9 ps 3 lc rgb "#44EE44" title "$G''$",\
"bbx_model.out"  u 1:2 w l lt 1 lw 4 lc rgb "#444444" title "model",\
"bbx_model.out"  u 1:2 w l lt 1 lw 4 lc rgb "#44AA44" notitle



#unset multiplot
#==================================





