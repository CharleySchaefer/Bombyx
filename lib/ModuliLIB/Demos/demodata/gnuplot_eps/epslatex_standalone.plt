#   Bombyx: Modelling of linear viscoelasticity
#   Copyright (C) 2019 Charley Schaefer
#==================================
# TERMINAL SETTINGS

set terminal epslatex standalone
set output "tmp_figure.tex"
#==================================

set log xy
set xlabel "$\\omega$ [rad/s]"
set xrange [1e-4: 1e11]
set xtics 1e-6, 1e3, 1e12
set format x "$10^\{%T\}$"

set ylabel "G', G'' [Pa s]"
set yrange [1e-1:1e7]
set ytics 1e-3, 1e3, 1e9
set format y "$10^\{%T\}$"

set label "Rouse relaxation" at 0.5e5, 1.2e5 rotate by 35
set label "Crosslink dissociation" at 1e1, 0.4e5 rotate by 35
set label "Reptation" at 5e-3, 0.8e4 rotate by 35

set arrow from 2e-3,80 to 2,8 lt 1 lw 2 lc rgb "black" front
set label "increasingly strong fluctuations" at 3,6

set grid 

set key right bottom 

plot "../demo_StickyReptation.out" u 1:2 w l lt 1 lw 4 lc rgb "#AA4444" title "G'",""  u 1:3 w l lt 1 lw 4 lc rgb "#44AA44" title "G''", "" u 4:5 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 4:6 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 7:8 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 7:9 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 10:11 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 10:12 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 13:14 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 13:15 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 16:17 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 16:18 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 19:20 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 19:21 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 22:23 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 22:24 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 25:26 w l lt 1 lw 2 lc rgb "#AA4444" notitle,""  u 25:27 w l lt 1 lw 2 lc rgb "#44AA44" notitle, "" u 28:29 w l lt 1 lw 4 lc rgb "#AA4444" notitle,""  u 28:30 w l lt 1 lw 4 lc rgb "#44AA44" notitle





