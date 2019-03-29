#!/bin/bash

if [ $# -lt 4 ]
then
  echo " "
  echo "  -------------------------------------------------------------------"
  echo "  Usage: $0   <datafile>  <xcol>  <ycol> <Nheader>"
  echo " "
  echo "  Returns the first and second x value at which the y column in a"
  echo "  file crosses zero. This value is found using linear interpolation"
  echo "  between positive and negative y values."
  echo " "
  echo "  ALGORITHM"
  echo "- If y is zero in the first row, it is skipped and not considered to"
  echo "  be a root."
  echo "- Root is identified as a change of sign between two x values."
  echo "  Root is obtained by linear interpolation."
  echo " "
  echo "  OUTPUT"
  echo "  > datafile: Plain text data file with column data."
  echo "  > xcol:     Column in datafile with x data."
  echo "  > ycol:     Column in datafile with y data."
  echo "  > Nheader:  Number of header lines in the text file."
  echo " "
  echo "  OUTPUT"
  echo "  Columns x1 x2 x3 y3 x4 y4"
  echo "  > x1:       value of first root  (y1=0) (linear interpolation)"
  echo "  > x2:       value of second root (y2=0) (linear interpolation)"
  echo "  > (x3, y3): value ( x1 < x3 < x2 ) where y takes extremum"
  echo "              value (minimum or maximum)"
  echo "  > (x4, y4): value ( x3 < x4 ) where y takes extremum:"
  echo "              if x3 is a minimum, then x4 is a maximum-"
  echo "              if x3 is a maximum, then x4 is a minimum."
  echo " "
  echo "  DEMO: run $0 DemoData/rootsdata.txt 1 2 0"
  echo "  -------------------------------------------------------------------"
  echo " "
  exit 1
fi



# INPUT
file=$1 # plain text with column data
xcol=$2  # column with x data
ycol=$3  # column with y data
Nheader=$4





awk -v xcol=$xcol -v ycol=$ycol -v Nheader=$Nheader  <$file 'NR==(Nheader+1) {mode=-1; I=0.0; xroot1=0.0; xroot2=0.0;  xprev=0.0; yprev=1.0; check=0; extr2found=0} NR>Nheader {

      x=$(xcol); 
      y=$(ycol);

      if(mode==1)
      {
        # Find first zero point
        if (check==0 && (y < 0) )
        { 
          xroot1= xprev - (x-xprev)*(yprev)/(y-yprev); 
          yextr=y;
          xextr=x;
          check=1;
        }

        # Find minimum after zero point
        if ( check==1 )
        { 
          if ( y <= yextr )
          {
            yextr=y; yextr2=yextr;
            xextr=x; xextr2=xextr;
          }
          else
            {check=2;}
        } 

        # Find maximum after the minimum
        if( (check>1) && (extr2found==0) )
        {
          if ( y >= yextr2 )
          {
            yextr2=y;
            xextr2=x;
          }
          else
            {extr2found=1;}
        }

        # Find second zero point
        if (check==2 && (y > 0) )
        { 
          xroot2= xprev - (x-xprev)*(yprev)/(y-yprev); 
          check=3;
        }
      }
      else if(mode==2)
      {
        # Find first zero point
        if (check==0 && (y > 0) )
        { 
          xroot1= xprev - (x-xprev)*(yprev)/(y-yprev); 
          yextr=y;
          xextr=x;
          check=1;
        }

        # Find maximum after zero point
        if ( check==1 )
        { 
          if ( y >= yextr )
          {
            yextr=y; yextr2=yextr;
            xextr=x; xextr2=xextr;
          }
          else
            {check=2};
        } 

        # Find minimum after the maximum
        if( (check>1) && (extr2found==0) )
        {
          if ( y <= yextr2 )
          {
            yextr2=y;
            xextr2=x;
          }
          else
            {extr2found=1;}
        }

        # Find second zero point
        if (check==2 && (y < 0) )
        { 
          xroot2= xprev - (x-xprev)*(yprev)/(y-yprev); 
          check=3;
        }
      }
      else if( (mode==-1) && (y != 0) )
      {
        if ( y>0 )
          {yprev= 1.0; yextr=1.0;  mode=1}
        else if ( y<0 )
          {yprev=-1.0; yextr=-1.0; mode=2}
      }

      xprev=x;
      yprev=y;

    } END {printf xroot1" "xroot2" "xextr" "yextr" "xextr2" "yextr2" \n"}'
