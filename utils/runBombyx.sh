#################################################################
#    Bombyx: Modelling of linear viscoelasticity
#    Copyright (C) 2019 Charley Schaefer <The University of York, UK>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#################################################################
#!/bin/bash

#################################################################
# USER INPUT
inputfile="../Demos/demodata/demodata.bbx"
outfile="bbx_errordata.out"

rseed=-1       # random number seed. -1: time stamp; >0: user-defined integer
MCseeds=50     # Number of Monte Carlo seeds
MCiter=1000    # Number of Monte Carlo iterations per seed
MCdelta=0.2    # small value -> small step size
MCsigma=0.2    # small value -> larger acceptance probability
ZeL=4          # Lower value for number of entanglements
ZeU=30         # Upper value for number of entanglements
ZsL=1          # Lower value for number of stickers
ZsU=10         # Upper value for number of stickers
tauSL=3.5e-3   # Lower value for sticker lifetime
tauSU=0.25     # Upper value for sticker lifetime
fL=0.2         # Lower value for prefactor f: Ge = f * Ge_esimate
fU=2.0         # Upper value for prefactor f: Ge = f * Ge_esimate
alpha=10       # material parameter
# END USER INPUT
#################################################################


#################################################################
# PARSE SCRIPT ARGUMENTS
src=../src
lib=../lib

executable=Bombyx.o
outfile="bbx_errordata.out"
do_debug=0
do_compile=0
do_runBombyx=0
do_postprocessing=0

version=`awk <../src/main.h '$1=="#define" && $2=="BBX_VERSION" {print $3}'`
if [ $# -lt 1 ] || [ $1 != "--version" ]; then
echo " "
echo "Bombyx $version Copyright (C) 2019 Charley Schaefer <The University of York, UK>"
echo " "
echo "This program comes with ABSOLUTELY NO WARRANTY; "
echo "for details type \"$0 --copying\"."
echo "This is free software, and you are welcome to redistribute it"
echo "under certain conditions; type \"$0 --copying\" for details."
echo " "
echo "For help: enter \"$0 --help\""
echo " "
if [ $# -lt 1 ] ; then
  exit 1;
fi
fi
for i in `seq 1 $#`; do
  if [ "${@:$i:1}" == "--help" ] || [ "${@:$i:1}" == "-h" ]; then
    echo "  Usage: $0 [script arguments]"
    echo " "
    echo "  --help        (or -h): Show help."
    echo "  --version     (or -v): Show version."
    echo "  --compile     (or -c): Tested for for gcc version 4.9.2 (Debian 4.9.2-10)."
    echo "  --run-bombyx  (or -r): Run Bombyx."
    echo "  --postprocess (or -p): Do postprocessing."
    echo "  --debug       (for development): compile \"gcc -g\" and run program using valgrind."
    echo "                Tested using valgrind-3.10.0."
    echo " "
    echo "  This script compiles and runs Bombyx and analyses its results."
    echo "  Input data consists of G' and G'' data as a function of the"
    echo "  angular frequency [rad/s] \"omega\", or the freqency [Hz] (1 [Hz] = 2 pi [rad/s])"
    echo " "
    echo "  The input data / parameters are specified in the \"USER INPUT\" section"
    echo "  in the $0 (plaintext) file. Running $0 with the default parameters"
    echo "  runs the demo that is discussed in doc/user_manual.pdf".
    echo " "
    echo "  Example: \"$0 -c -r -p\" subsequently compiles, runs, and postprocesses."
    echo "  Compilation can be skipped if \"$executable\" is already present in the build directory. "
    echo " "
    exit 1
  elif [ "${@:$i:1}" == "--copying" ]; then
    cat ../LICENSE.txt
    exit 1
  elif [ "${@:$i:1}" == "--compile" ] || [ "${@:$i:1}" == "-c" ] ; then
    do_compile=1
  elif [ "${@:$i:1}" == "--version" ] || [ "${@:$i:1}" == "-v" ] ; then
    echo "Bombyx $version"
    exit 1
  elif [ "${@:$i:1}" == "--debug" ] ; then
    do_compile=1
    do_debug=1
    do_postprocessing=1
    arg_prog=-g
    MCiter=10
  elif [ "${@:$i:1}" == "--run-bombyx" ] || [ "${@:$i:1}" == "-r" ] ; then
    do_runBombyx=1
  elif [ "${@:$i:1}" == "--postprocess" ] || [ "${@:$i:1}" == "-p" ] ; then
    do_postprocessing=1
  fi
done
# END PARSE SCRIPT ARGUMENTS
#################################################################



#################################################################
# COMPILE
if [ $do_compile -eq 1 ]; then

  echo "Compiling lib/ZiltoidLIB"
  pushd ../lib/ZiltoidLIB > /dev/null
    ./makeZiltoidLIB.sh
  popd > /dev/null
  cp ../lib/ZiltoidLIB/Applications/bin/getColumnStats.o ../utils/getColumnStats.o
  cp ../lib/ZiltoidLIB/Applications/ColumnStats/plotColumnStats.plt ../utils/plotColumnStats.plt
  echo "  created utils/getColumnStats.o and plotColumnStats.plt"

  echo "Compiling lib/ModuliLIB"
  pushd ../lib/ModuliLIB > /dev/null
    ./makeModuliLIB.sh
  popd > /dev/null

  echo "Compiling $executable."
  if ! gcc -o $executable $src/main.c $src/MonteCarlo/BBX_MC_fitting.c $src/MonteCarlo/BBX_MC_FilterSamples.c $src/Import/ReadBBXdata.c $src/Import/ParseProgramArguments.c $src/export.c -L$lib/ModuliLIB -lModuliLIB -L$lib/ZiltoidLIB -lZiltoidLIB -lm $arg_prog
  then 
    echo "Compilation failed - terminating $0"
    exit
  fi
  echo " "
  echo "Compilation finished"
  echo " "
  echo "Bombyx $version Copyright (C) 2019 Charley Schaefer"
  echo "This program comes with ABSOLUTELY NO WARRANTY; "
  echo "for details type \"$0 --copying\"."
  echo "This is free software, and you are welcome to redistribute it"
  echo "under certain conditions; type \"$0 --copying\" for details."
  echo " "
fi
# COMPILATION DONE
#################################################################


#################################################################
# RUN PROGRAM
if [ $do_runBombyx -eq 1 ]; then

  if [ $do_debug -eq 1 ]; then
    run="valgrind --leak-check=full --track-origins=yes ./$executable $inputfile --MCseeds 2 --rseed $rseed --alpha $alpha --ZeL $ZeL --tauSU $tauSU --MCiter $MCiter --MCdelta $MCdelta --MCsigma $MCsigma"
  else 
    run="./$executable $inputfile --MCseeds $MCseeds --rseed $rseed --alpha $alpha --ZsL $ZsL --ZsU $ZsU --ZeL $ZeL --ZeU $ZeU  --tauSL $tauSL --tauSU $tauSU --MCiter $MCiter --MCdelta $MCdelta --MCsigma $MCsigma --GeL $fL --GeU $fU "
  fi

  echo "Running $run  > $outfile"
  printf "`$run`" > $outfile
  echo " "
fi
# FINISHED RUNNING PROGRAM
#################################################################


#################################################################
# POSTPROCESS
if [ $do_postprocessing -eq 1 ] && [ ! -z `command -v gnuplot` ]; then
  echo "Postprocessing - plotting using `gnuplot --version`"
  if [ "`gnuplot --version`" != "gnuplot 4.6 patchlevel 6" ]; then
  echo "                 WARNING: postprocessing only tested for gnuplot 4.6 patchlevel 6."
  fi
elif [ $do_postprocessing -eq 1 ]; then
  echo "Postprocessing - gnuplot not installed - skipping processing. Info: http://www.gnuplot.info/" 
  do_postprocessing=0
fi

if [ $do_postprocessing -eq 1 ]; then

  cp ../utils/FitReport.html .

  img="img"
  img=${img#./} # remove "./" prefix
  img=${img%/} # Input (remove "/" suffix)
  mkdir -p $img



  ../utils/gnuplot_png/plotStickyReptationFit.sh
  gnuplot ../utils/gnuplot_png/plotChi2Landscape.plt

  gnuplot ../utils/gnuplot_png/plotChi2convergence.plt > /dev/null
  gnuplot ../utils/gnuplot_png/plotTauSconvergence.plt > /dev/null
  gnuplot ../utils/gnuplot_png/plotZSconvergence.plt > /dev/null
  gnuplot ../utils/gnuplot_png/plotZEconvergence.plt > /dev/null
  gnuplot ../utils/gnuplot_png/plotAconvergence.plt > /dev/null
  gnuplot ../utils/gnuplot_png/plotGEconvergence.plt > /dev/null


  a="`../utils/gnuplot_png/plotHist.sh bbx_convergence_filtered.out 1`
"
  a="$a`../utils/gnuplot_png/plotHist.sh bbx_convergence_filtered.out 2`
"
  a="$a`../utils/gnuplot_png/plotHist.sh bbx_convergence_filtered.out 3`
"
  a="$a`../utils/gnuplot_png/plotHist.sh bbx_convergence_filtered.out 4`
"
  a="$a`../utils/gnuplot_png/plotHist.sh bbx_convergence_filtered.out 5`
"
  a="$a`../utils/gnuplot_png/plotHist.sh bbx_convergence_filtered.out 6`
"

  echo "$a"
  #sed -i.bak '/<!--FIT-RESULT-->/ a '"$a" FitReport.html





  echo "Calculate Pearson Correlation Matrix from \"bbx_convergence_filtered.out\". "
  echo " " >> $outfile # white line
  echo " " >> $outfile # white line
  echo "Pearson correlation matrix:" >> $outfile 
  ../utils/get_PearsonCorrelationMatrix.sh bbx_convergence_filtered.out >> $outfile
  echo " " >> $outfile # white line
  echo " " >> $outfile # white line
  echo "Statistics:" >> $outfile

  for n in `seq 2 6`; do
  ../utils/getColumnStats.o --file bbx_convergence_filtered.out --column $n > ColumnStats.out
  cat ColumnStats.out | awk -vn=$n 'n==2 || NR==2 {print($0)}' >> $outfile
    gnuplot ../utils/plotColumnStats.plt
    if [ $n -eq 2 ]; then
  cp ColumnStats.png img/stats_TauS.png
    elif [ $n -eq 3 ]; then
  cp ColumnStats.png img/stats_ZE.png
    elif [ $n -eq 4 ]; then
  cp ColumnStats.png img/stats_ZS.png
    elif [ $n -eq 5 ]; then
  cp ColumnStats.png img/stats_Ge.png
    elif [ $n -eq 6 ]; then
  cp ColumnStats.png img/stats_alpha.png
    fi
  done


  echo "Results summarised in \"FitReport.html\" and in \"$outfile\"."
  echo " "
fi
# DONE POSTPROCESSING
#################################################################

