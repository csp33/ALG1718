#!/usr/bin/gnuplot

set xlabel "Tamanio del problema"
set ylabel "Tiempo (seg)"
set terminal png size 640,480
set output 'clasico.png'
plot 'clasico.dat' with lines
set output 'dyv.png'
plot 'dyv.dat' with lines
