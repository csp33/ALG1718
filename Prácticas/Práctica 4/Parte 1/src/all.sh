#!/bin/bash
echo "Compilando..."
g++ -o fuerzaBruta fuerzaBruta.cpp -O3
g++ -o backtracking1 backtracking1.cpp -O3
g++ -o backtracking2 backtracking2.cpp -O3
echo "Ejecutando fuerza bruta..."
./individual.sh fuerzaBruta 1 1
echo "Ejecutando backtracking sin información..."
./individual.sh backtracking1 1 1
echo "Ejecutando backtracking con información..."
./individual.sh backtracking2 1 1
echo "Generando gráficas..."
./gnuplot.sh
