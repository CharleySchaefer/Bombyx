#!/bin/bash

echo " COMPILE"
#gcc -o Demo_QR.o Demo_QRdecomposition.c ../Mathematics.c -lm

gcc -o Demo_QR.o Demo_QRdecomposition.c -L../build -lZiltoidMath -lm

echo " RUN"
./Demo_QR.o
