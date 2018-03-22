#!/usr/bin/gnuplot

set xlabel "Tamanio del problema"
set ylabel "Tiempo (seg)"
set logscale
set terminal png size 640,480
set output 'busqueda_binaria.png'
plot 'busqueda_binaria.dat' with lines
set output 'busqueda_ternaria.png'
plot 'busqueda_ternaria.dat' with lines
