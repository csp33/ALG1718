#!/bin/bash
if [ $# -eq 3 ]
then
i="0"
tam=$2
#Primer argumento: programa a ejecutar
#Segundo argumento: tamaño inicial del umbral
#Tercer argumento : incremento
while [ $i -lt 20 ]
do
	./$1 $tam 1000000 >> ./$1.dat
	i=$[$i+1]
	tam=$[$tam+$3]
done
else
echo "Error de argumentos"
fi
