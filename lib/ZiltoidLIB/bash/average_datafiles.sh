#!/bin/bash

if [ $# -lt 4 ]
then
  echo " "
  echo "  ------------------------------------------------------------"
  echo "  Usage: $0   <file_prefix>  <file_suffix>  <output directory>"
  echo " "
  echo "  This script averages the y-columns (y1, y2, y3,...) of various files"
  echo "  and returns an averaged file with standard deviations:"
  echo "  (y1, y1std, y2, y2std, y3, y3std)."
  echo " "
  echo "  INPUT: Data file files \"file_prefix\"*\"file_suffix\", e.g.,"
  echo "    {DemoData/file_001.txt, data/file_002.txt, ..} --> file_prefix=\"DemoData/file_\", file_suffix=\".txt\", or "
  echo "    {DemoData_001/file.txt, data_002/file.txt, ..} --> file_prefix=\"DemoData_\", file_suffix=\"/file.txt\"."
  echo " "
  echo "  OUTPUT: written to the <output directory>."
  echo " "
  echo "  DEMO:   Run: $0 DemoData/data .txt OutputDirectory."
  echo "          This produces as output OutputDirectory/av.out. "
  echo "  Known bug: Header in the file is interpreted as zero values."
  echo "  ------------------------------------------------------------"
  echo " "
  exit 1
fi

file_prefix=$1  # e.g., "data/file" or "data"
file_suffix=$2  # e.g., ".txt" or "/file.txt" 
# file name: $file_prefix*$file_suffix

# Create output directory
dir=$3
dir=${dir#./} # remove "./" prefix
dir=${dir%/} # Input (remove "/" suffix)
mkdir -p $dir

Nheader=$4

# Create temporary output file (will be deleted at the end of the script)
timestamp() {
  date +"%T"
}
stamp=`timestamp`
averagetmp=$dir"/tmp_av_$stamp.out"
averagefinal=$dir"/av.out"
>$averagetmp


# Remove output files if they exist
if [  -f $file_prefix"av"$file_suffix ]
then
  rm $file_prefix"av"$file_suffix
fi
if [  -f $file_prefix"std"$file_suffix ]
then
  rm $file_prefix"std"$file_suffix
fi


file=`find "$file_prefix"*"$file_suffix" | head -1`


# GET AVERAGE FILES
{ head -n2 $file; paste $file_prefix*$file_suffix;} | awk -v Nheader=$Nheader 'NR==Nheader+1 {d=NF; next; } {
                if(NF>1)
                {
                  for (i=1;i<=d;i++) 
                  {
                    s=0;
                    for (j=i;j<=NF;j+=d)
                      s+=$j;
                    printf " %.2e%s",  s/(NF/d),  j==NF+d?"\n":" ";
                  }
                }}' > $averagetmp

# GET STANDARD DEVIATION
{ head -n2 $averagetmp; paste $averagetmp $file_prefix*$file_suffix;} | awk -v Nheader=$Nheader 'NR==Nheader+1 {d=NF;   next; } {
                if(NF>1)
                {
                  for (i=1;i<=d;i++) 
                  {
                    s=0;
                    av=$i;
                    for (j=i+d;j<=NF;j+=d)
                    { 
                      s+=($j-av)**2;
                    }
                    printf "%.2e %.2e%s",  av, sqrt(s/((NF/d)-1)),  i==d?"\n":" ";
                  }
                }}' > $averagefinal
rm -r $averagetmp
echo "Data written to $averagefinal"
