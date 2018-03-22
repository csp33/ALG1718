#!/bin/bash
	echo "Compilando..."
	g++ -o busqueda_binaria busqueda_binaria.cpp -O3 &&
	g++ -o busqueda_ternaria busqueda_ternaria.cpp -O3 &&
	rm -f ./busqueda_binaria.dat ;
	rm -f ./busqueda_ternaria.dat ;
	echo "Ejecutando búsqueda binaria..." ;
	./individual.sh busqueda_binaria 50000000 20000000;
	echo "Ejecutando búsqueda ternaria..." ;
	./individual.sh busqueda_ternaria 50000000 20000000;
	echo "Generando gráficas..." ;
	./gnuplot.sh ;
