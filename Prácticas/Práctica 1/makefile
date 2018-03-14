#Definimos etiquetas
DOC=doc
SRC=src
OUT=out
BIN=src

all : todos

todos : burbuja floyd hanoi heapsort insercion mergesort quicksort seleccion
	cd $(SRC) ; ./todos.sh

burbuja : 
	g++ -o ./$(BIN)/burbuja ./$(SRC)/burbuja.cpp

floyd : 
	g++ -o ./$(BIN)/floyd ./$(SRC)/floyd.cpp
hanoi : 
	g++ -o ./$(BIN)/hanoi ./$(SRC)/hanoi.cpp
heapsort : 
	g++ -o ./$(BIN)/heapsort ./$(SRC)/heapsort.cpp
insercion : 
	g++ -o ./$(BIN)/insercion ./$(SRC)/insercion.cpp
mergesort : 
	g++ -o ./$(BIN)/mergesort ./$(SRC)/mergesort.cpp
quicksort : 
	g++ -o ./$(BIN)/quicksort ./$(SRC)/quicksort.cpp
seleccion :
	g++ -o ./$(BIN)/seleccion ./$(SRC)/seleccion.cpp
