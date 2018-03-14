#!/usr/bin/gnuplot

#Burbuja

set xlabel "Tamanio del problema"
set ylabel "Tiempo (seg)"
set terminal png size 640,480
set output 'empirica_burbuja_2.png'
plot 'burbuja.out' with lines

#Floyd

set terminal png size 640,480
set output 'empirica_floyd_2.png'
plot 'floyd.out' with lines

#Hanoi

set terminal png size 640,480
set output 'empirica_hanoi_2.png'
plot 'hanoi.out' with lines

#Heapsort

set terminal png size 640,480
set output 'empirica_heapsort_2.png'
plot 'heapsort.out' with lines

#Insercion

set terminal png size 640,480
set output 'empirica_insercion_2.png'
plot 'insercion.out' with lines

#Mergesort

set terminal png size 640,480
set output 'empirica_mergesort_2.png'
plot 'mergesort.out' with lines

#Quicksort

set terminal png size 640,480
set output 'empirica_quicksort_2.png'
plot 'quicksort.out' with lines

#Selección

set terminal png size 640,480
set output 'empirica_seleccion_2.png'
plot 'seleccion.out' with lines


