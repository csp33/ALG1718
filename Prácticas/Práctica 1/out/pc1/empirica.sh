#!/usr/bin/gnuplot

#Burbuja

set xlabel "Tamanio del problema"
set ylabel "Tiempo (seg)"
set terminal png size 640,480
set output 'empirica_burbuja.png'
plot 'burbuja.out' with lines

#Floyd

set terminal png size 640,480
set output 'empirica_floyd.png'
plot 'floyd.out' with lines

#Hanoi

set terminal png size 640,480
set output 'empirica_hanoi.png'
plot 'hanoi.out' with lines

#Heapsort

set terminal png size 640,480
set output 'empirica_heapsort.png'
plot 'heapsort.out' with lines

#Insercion

set terminal png size 640,480
set output 'empirica_insercion.png'
plot 'insercion.out' with lines

#Mergesort

set terminal png size 640,480
set output 'empirica_mergesort.png'
plot 'mergesort.out' with lines

#Quicksort

set terminal png size 640,480
set output 'empirica_quicksort.png'
plot 'quicksort.out' with lines

#Selección

set terminal png size 640,480
set output 'empirica_seleccion.png'
plot 'seleccion.out' with lines


