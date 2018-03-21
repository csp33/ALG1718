
#define TEST 0	//Imprimir o no el resultado
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

int busquedaBinaria(const int *v, const int i, const int j, const int buscado) {
	//Caso base: nos cruzamos
	if (i > j) {
		return -1;
	}
	else {
		int mitad = (i + j) / 2;
		if (v[mitad] == buscado)		//Acertamos
			return mitad;
		else if (buscado < v[mitad])	//Buscamos hacia la izquierda
			return busquedaBinaria(v, i, mitad - 1, buscado);
		else						//Buscamos hacia la derecha
			return busquedaBinaria(v, mitad + 1, j, buscado);
	}
}
int main(int argc, char **argv) {
	if (argc != 2) {
		cerr << "Uso del programa: " << argv[0] << " <tamaño>" << endl;
		exit(-1);
	}
	int tam = atoi(argv[1]);
	int *v = new int[tam];
	high_resolution_clock::time_point tantes;
	high_resolution_clock::time_point tdespues;
	duration<double> tiempo;

	//Inicializamos vector
	for (int i = 0; i < tam; i++)
		v[i] = i;
	
	//Peor caso: el elemento no existe
	tantes = high_resolution_clock::now();
	int pos = busquedaBinaria(v, 0, tam, tam);
	tdespues = high_resolution_clock::now();
	tiempo = duration_cast<duration<double>>(tdespues - tantes);
#if TEST
	cout << "Posición: " << pos << endl;
#endif
	cout << tam << "\t\t" << tiempo.count() << endl;
}