#!/bin/bash
	echo "Compilando..."
	g++ -o busqueda_binaria busqueda_binaria.cpp &&
	g++ -o busqueda_ternaria busqueda_ternaria.cpp &&
	rm -f ./busqueda_binaria.dat ;
	rm -f ./busqueda_ternaria.dat ;
	echo "Ejecutando búsqueda binaria..." ;
	./individual.sh busqueda_binaria 10000000 10000000;
	echo "Ejecutando búsqueda ternaria..." ;
	./individual.sh busqueda_ternaria 10000000 10000000;
	echo "Generando gráficas..." ;
	./gnuplot.sh ;
