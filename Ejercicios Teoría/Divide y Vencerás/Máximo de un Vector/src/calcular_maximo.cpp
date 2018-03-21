#include <iostream>
#include <stdlib.h>  
#include <time.h>      
#include <climits>

using namespace std;

int hallarMaximo(const int *v, const int inicio, const int fin) {
	int max = v[inicio];
	for (int i = inicio + 1; i < fin; i++)
		if (v[i] > max)
			max = v[i];
	return max;
}

int hallarMaximoDyV(const int *v,const int i,const int j){
	if(i==j){
		return v[i];
	}
	else{
		int mitad=(i+j)/2;
		int max_izquierda=hallarMaximoDyV(v,i,mitad);
		int max_derecha=hallarMaximoDyV(v,mitad+1,j);
		if(max_izquierda>max_derecha)
			return max_izquierda;
		else
			return max_derecha;
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		cerr << "Falta el tamaño del vector.";
		exit(-1);
	}

	int tam = atoi(argv[1]);
	int *v = new int[tam];
	int max;
	clock_t tantes;    // Valor del reloj antes de la ejecución
	clock_t tdespues;  // Valor del reloj después de la ejecución

	//Inicializar vector con valores aleatorios
	srand (time(NULL));

	for (int i = 0; i < tam; i++)
		v[i] = rand() ;

	//Algoritmo sencillo
	cout<<"--------------------------------------------------------------------------------------------------------------------------\n";
	cout<<"\t\tTamaño "<<tam<<endl;
	tantes = clock();
	max = hallarMaximo(v, 0, tam);
	tdespues = clock();
	cout << "Algoritmo sencillo:\tMáximo: " << max << "\tTiempo: " << ((double)(tdespues - tantes)) / CLOCKS_PER_SEC << endl;


	//Algoritmo DyV

	tantes = clock();
	max = hallarMaximoDyV(v, 0,tam);
	tdespues = clock();
	cout << "Algoritmo DyV:\tMáximo: " << max << "\tTiempo: " << ((double)(tdespues - tantes)) / CLOCKS_PER_SEC << endl;

	cout<<"--------------------------------------------------------------------------------------------------------------------------\n";
	delete []v;
}