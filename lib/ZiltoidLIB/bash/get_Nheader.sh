#!/bin/bash

fname=$1

awk <$fname 'BEGIN {check=0; Nheader=0} NF>1 && check==0 { c=substr($1,0,1); check=(('0'<=c) && (c<='9')); Nheader=Nheader+1;    } END{print Nheader} '
