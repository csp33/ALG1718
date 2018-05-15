#!/bin/bash
if [ $# -eq 2 ]
then
rm ./backtracking2.dat;
i="0"
n=$1
#Primer argumento: Tamaño
#Tercer argumento : incremento
while [ $i -lt 100 ]
do
	./backtracking2 $n >> ./backtracking2.dat
	i=$[$i+1]
	n=$[$n+$2]
done
else
echo "Error de argumentos"
fi
