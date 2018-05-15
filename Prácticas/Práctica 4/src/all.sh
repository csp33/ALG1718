#!/bin/bash
echo "Ejecutando fuerza bruta..."
./fuerzaBruta.sh 1 1
echo "Ejecutando backtracking sin información..."
./backtracking_noinfo.sh 1000 500
echo "Ejecutando backtracking con información..."
./backtracking_info.sh 1000 500
echo "Generando gráficas..."
./gnuplot.sh
