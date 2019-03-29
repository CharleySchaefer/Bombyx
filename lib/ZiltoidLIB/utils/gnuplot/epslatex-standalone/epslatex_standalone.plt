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
FAC225=(1.0/0.021)**0.33 
FAC200=(1.0/0.045)**0.33 
FAC180=(1.0/0.128)**0.33
FAC176=(1.0/0.02)**0.33
FAC100=(1.0/0.128)**0.33
#==================================


#==================================
# MARGINS
#----------------------------------
set lmargin 8
#set tmargin 8
set bmargin 4
#set rmargin 4
#==================================


#==================================
# X AXIS
#----------------------------------
set xlabel "\\huge $\\nu_\{\\mathrm\{ads\}\}/\\nu_\{\\mathrm\{hop\}\}$"
set xrange [2e-7:0.002]
set log x
set xtics 1e-7,10,0.1
set format x "$10^\{%T\}$"
#==================================


#==================================
# Y AXIS
#----------------------------------
set ylabel "\\huge $R_0$"
set yrange [8:100]
set log y
# set ytics
set format y "$10^\{%T\}$"
#==================================


#==================================
# KEY
#----------------------------------
#set nokey 
#==================================


#==================================
# LABELS AND ARROWS
#----------------------------------
set label "\\Large \\textcolor[rgb]\{0.7333,0,0\}\{$-1/3$\}" at 1.6e-6, 70
set label "\\Large \\textcolor[rgb]\{0,0.7333,0\}\{$-1/4$\}" at 4e-4, 14
#set arrow from 2e-4,40 to 2e-4,2 lw 2 front
#==================================


#==================================
# PLOT
#----------------------------------
#set multiplot
#set grid

plot \
(x<3e-4?0.21*FAC225*x**(-1.0/3.0):1/0) notitle  w l lt 1 lw 8 lc rgb "white",\
(x<3e-4?0.21*FAC225*x**(-1.0/3.0):1/0) notitle  w l lt 1 lw 4 lc rgb "#BB0000",\
(x>4e-7?0.65*FAC200*x**(-1.0/4.0):1/0) notitle  w l lt 1 lw 8 lc rgb "white",\
(x>4e-7?0.65*FAC200*x**(-1.0/4.0):1/0) notitle  w l lt 1 lw 4 lc rgb "#00BB00"

#unset multiplot
#==================================





