#!/bin/bash
if [ $# -eq 2 ]
then
i="0"
rm ./fuerzaBruta.dat;
n=$1
#Primer argumento: Tamaño
#Segundo argumento : incremento
while [ $i -lt 25 ]
do
	./fuerzaBruta $n >> ./fuerzaBruta.dat
	i=$[$i+1]
	n=$[$n+$2]
done
else
echo "Error de argumentos"
fi
