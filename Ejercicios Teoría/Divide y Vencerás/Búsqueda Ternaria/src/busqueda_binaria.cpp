
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
	double acumulado = 0;

	//Inicializar vector con valores aleatorios
	srand (time(NULL));

	for (int i = 0; i < tam; i++)
		v[i] = rand() ;
	// Peor caso: no está
	//Ejecutamos 1000 veces y obtenemos el tiempo medio
	for (int i = 0; i < 1000; i++) {
		tantes = high_resolution_clock::now();
		int pos = busquedaBinaria(v, 0, tam, rand() + 0.5);
		tdespues = high_resolution_clock::now();
		tiempo = duration_cast<duration<double>>(tdespues - tantes);
		acumulado += tiempo.count();
	}
	acumulado /= 1000;


#if TEST
	cout << "Posición: " << pos << endl;
#endif
	cout << tam << "\t\t" << acumulado << endl;

	delete []v;
}