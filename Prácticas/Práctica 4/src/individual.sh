#!/bin/bash
if [ $# -eq 3 ]
then
i="0"
n=$2
#Primer argumento: programa a ejecutar
#Segundo argumento: Tamaño
#Tercer argumento : incremento
while [ $i -lt 25 ]
do
	./$1 $n >> ./$1.dat
	i=$[$i+1]
	n=$[$n+$3]
done
else
echo "Error de argumentos"
fi
