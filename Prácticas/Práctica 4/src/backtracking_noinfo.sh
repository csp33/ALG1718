#!/bin/bash
if [ $# -eq 2 ]
then
i="0"
n=$1
#Primer argumento: Tamaño
#Tercer argumento : incremento
while [ $i -lt 100 ]
do
	./backtracking1 $n >> ./backtracking1.dat
	i=$[$i+1]
	n=$[$n+$2]
done
else
echo "Error de argumentos"
fi
