#!/bin/bash
g++ -o tsp tsp.cpp -Ofast;
./tsp ../datosTSP/ulysses6.tsp;
./tsp ../datosTSP/ulysses7.tsp;
./tsp ../datosTSP/ulysses8.tsp;
./tsp ../datosTSP/ulysses9.tsp;
./tsp ../datosTSP/ulysses10.tsp;
./tsp ../datosTSP/ulysses11.tsp;
./tsp ../datosTSP/ulysses12.tsp;
./tsp ../datosTSP/ulysses13.tsp;
./tsp ../datosTSP/ulysses14.tsp;
./tsp ../datosTSP/ulysses15.tsp;
./tsp ../datosTSP/ulysses16.tsp;
./gnuplot.sh
