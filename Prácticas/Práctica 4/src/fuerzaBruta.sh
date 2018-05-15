#!/bin/bash
if [ $# -eq 2 ]
then
i="0"
rm ./FuerzaBruta.dat
n=$1
#Primer argumento: Tamaño
#Tercer argumento : incremento
while [ $i -lt 25 ]
do
	./FuerzaBruta $n >> ./FuerzaBruta.dat
	i=$[$i+1]
	n=$[$n+$2]
done
else
echo "Error de argumentos"
fi
