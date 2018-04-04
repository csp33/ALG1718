#!/bin/bash
	echo "Compilando..."
	g++ -o clasico clasico.cpp -O3 &&
	g++ -o dyv dyv.cpp -O3 &&
	rm -f ./clasico.dat ;
	rm -f ./dyv.dat ;
	echo "Ejecutando algoritmo clásico..." ;
	./individual.sh clasico 100000 50000;
	echo "Ejecutando algoritmo DyV..." ;
	./individual.sh dyv 1000000 500000;
	echo "Generando gráficas..." ;
	./gnuplot.sh ;
