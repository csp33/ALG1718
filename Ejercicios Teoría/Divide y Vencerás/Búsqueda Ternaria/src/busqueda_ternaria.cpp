
#define TEST 0	//Imprimir o no el resultado
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;

int busquedaTernaria(const int *v, const int i, const int j, const int buscado) {
	//Caso base: nos cruzamos
	if (i > j)
		return -1;
	else {
		int tam = j - i;
		int tercio = i + (tam / 3);
		int dostercio = i + (tam * 2) / 3;
		if (v[tercio] == buscado) //Acertamos
			return tercio;
		else if (buscado < v[tercio]) //Primer tercio
			busquedaTernaria(v, i, tercio - 1, buscado);
		else {
			if (v[dostercio] == buscado)	//Acertamos
				return dostercio;
			else if (buscado > v[dostercio]) //Tercer tercio
				busquedaTernaria(v, dostercio + 1, j, buscado);
			else //Segundo tercio
				busquedaTernaria(v, tercio + 1, dostercio - 1, buscado);
		}
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
		int pos = busquedaTernaria(v, 0, tam, rand() + 0.5);
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