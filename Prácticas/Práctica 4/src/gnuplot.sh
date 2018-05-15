#!/usr/bin/gnuplot

set xlabel "Tamanio del problema"
set ylabel "Tiempo (seg)"
set terminal png size 640,480
set output 'fuerzaBruta.png'
plot 'fuerzaBruta.dat' with linespoints
set output 'backtracking1.png'
plot 'backtracking1.dat' with linespoints
set output 'backtracking2.png'
plot 'backtracking2.dat' with linespoints
