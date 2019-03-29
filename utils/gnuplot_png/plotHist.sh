#!/bin/bash

if [ $# -lt 2 ] ; then
  echo "Usage: $0 <file name> <column number>"
  echo "  file name: e.g., path/bbx_convergence_filtered.out"
  exit 1
fi

file=$1
col=$2
outpath="img"

mkdir -p $outpath
Nheader=1
xlabel=`awk  -v col=$col -v Nheader=$Nheader <$file 'NR==1 {print $(col)}'`

counts=`awk  -v col=$col -v Nheader=$Nheader <$file 'BEGIN {count=0} NR>Nheader {
                         if(1) 
                          {count=count+1;}
                         }
                         END {print count}'`


mean=`awk  -v col=$col -v Nheader=$Nheader <$file 'BEGIN {count=0; sum=0.0} NR>Nheader {
                         if(1) 
                          {sum=sum+$(col);
                           count=count+1;}
                         }
                         END {print sum/count}'`


var=`awk  -v M=$mean -v Nheader=$Nheader -v col=$col <$file 'BEGIN {count=0; sum=0.0} NR>Nheader {
                         if(1) 
                          {sum=sum+($(col)-M)**2;
                           count=count+1;}
                         }
                         END {print sum/count}'`
std=`awk -v var=$var 'BEGIN {print sqrt(var)}'`

if [ $col -eq 1 ]; then
printf "%12s %12s %12s %12s\n" "par" "mean" "var" "std"

fi
printf "%12s %12.3e %12.3e %12.3e\n" $xlabel $mean $var $std

if [ $col -eq 1 ]; then
plot="set terminal pngcairo enhanced
set output \"$outpath/hist_$xlabel.png\"
binwidth=$std/4
bin(x,width)=width*floor(x/width)
set xlabel \"$xlabel\"   font \", 24\"
set xrange [0:$mean+3*$std]
#set log y
unset key
set style fill solid 0.5 # fill style
plot \\
\"<(awk <$file '{{print \$0}}')\" using (bin(\$$col,binwidth)):(1.0) smooth freq with boxes lc rgb \"#66BB66\"
"

elif [ $std != "0" ]; then
plot="set terminal pngcairo enhanced
set output \"$outpath/hist_$xlabel.png\"
binwidth=$std/4
bin(x,width)=width*floor(x/width)
set xlabel \"$xlabel\"  font \", 24\"
set xrange [$mean-3*$std:$mean+3*$std]
unset key
set style fill solid 0.5 # fill style
plot \\
\"<(awk <$file '{{print \$0}}')\" using (bin(\$$col,binwidth)):(1.0) smooth freq with boxes lc rgb \"#66BB66\",\\
exp( -(x-$mean)**2/( 2*$var )  )*$counts*binwidth/sqrt(  2*3.14*$var  ) w l lt 1 lw 3 lc rgb \"black\"
"
else
plot="set terminal pngcairo enhanced
set output \"$outpath/hist_$xlabel.png\"
binwidth=$mean/10
bin(x,width)=width*floor(x/width)
set xlabel \"$xlabel\"  font \", 24\"
set xrange [$mean-10*binwidth:$mean+10*binwidth]
unset key
set style fill solid 0.5 # fill style
plot \\
\"<(awk <$file '{{print \$0}}')\" using (bin(\$$col,binwidth)):(1.0) smooth freq with boxes lc rgb \"#66BB66\"
"
fi

printf "$plot" | gnuplot > /dev/null 2>&1
