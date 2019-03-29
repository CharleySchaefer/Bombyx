set terminal pngcairo enhanced
set output "ColumnStats.png"

set xrange [-1:2];
plot \
"ColumnStats.out" u 0:9:7:6:10 w candlesticks lc rgb "red", \
"" u 0:8:8:8:8 w candlesticks lc rgb "black", \
"" u ($0+1):2:4 w errorbars lc rgb "blue"
