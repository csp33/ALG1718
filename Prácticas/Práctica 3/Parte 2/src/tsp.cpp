#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cmath>

using namespace std;

struct ciudad {
	int n;
	double x;
	double y;
};

bool operator==(const ciudad &una, const ciudad &otra) {
	return una.x == otra.x && una.y == otra.y;
}
bool operator!=(const ciudad &una, const ciudad &otra) {
	return !(una == otra);
}


class TSP {

private:
	vector<ciudad> ciudades;
	double distancia_total;
	vector<ciudad> camino;
	vector<vector<bool>> matriz_distancias;
	vector<bool> visitados;

	void calcularDistancia() {
		distancia_total = 0;
		int i = 0;
		for (int j = 1; j < camino.size(); j++) {
			distancia_total += distanciaEuclidea(camino[i], camino[j]);
			i++;
		}
	}
	double distanciaEuclidea(const ciudad &una, const ciudad &otra) {
		double resultado;
		if (una == otra)
			resultado = 0;
		else
			resultado = sqrt(pow(una.x - otra.x, 2) + pow(una.y - otra.y, 2));
		return resultado;
	}

	int CiudadMasCercana(ciudad actual) {
		double distancia_minima = INFINITY;
		int ciudad = -1;
		for (int i = 0; i < ciudades.size(); i++) {
			if (matriz_distancias[actual.n][i] < distancia_minima && !visitados[i]) {
				distancia_minima = matriz_distancias[actual.n][i];
				ciudad = i;
			}
		}
		return ciudad;
	}

public:

	TSP() {

	}
	TSP(char *archivo) {
		CargarDatos(archivo);
	}
	~TSP() {

	}
	void VecinoMasCercano() {
		camino.clear();
		ciudad actual = ciudades[0];
		ciudad siguiente;
		bool fin = false;
		camino.push_back(actual);
		while (!fin) {
			visitados[actual.n] = true;						//Pongo como visitados
			int indice_siguiente = CiudadMasCercana(actual);	//Busco el indice de la siguiente ciudad
			if (indice_siguiente != -1)							// Si he recorrido todas las ciudades, añado la primera.
				siguiente = ciudades[indice_siguiente];
			else {
				fin = true;
				siguiente = camino[0];	//Volvemos al inicio
			}
			camino.push_back(siguiente);
			actual = siguiente;
		}
		calcularDistancia();
	}

	void CargarDatos(char *archivo) {
		ifstream datos;
		string s;
		int n;
		ciudad aux;
		datos.open(archivo);
		if (datos.is_open()) {
			datos >> s;	//Leo DIMENSIÓN
			datos >> n;	//Leo NÚMERO de ciudades
			visitados.reserve(n);
			matriz_distancias.reserve(n);

			for (int i = 0; i < n; i++)
				matriz_distancias[i].reserve(n);

			for (int i = 0; i < n; i++) {
				datos >> aux.n;	// Leo número de ciudad
				aux.n--;			//Decremento el número: los índices del archivo comienzan en 1. Los del vector en 0.
				datos >> aux.x >> aux.y; //Leo coordenadas
				ciudades.push_back(aux);
			}
			datos.close();
		}
		else
			cout << "Error al leer " << archivo << endl;


		for (int i = 0; i < ciudades.size(); i++) {
			for (int j = 0; j < ciudades.size(); j++) {
				matriz_distancias[i][j] = distanciaEuclidea(ciudades[i], ciudades[j]);
			}
		}
	}

	void matrizDistancias(){
		for(int i=0;i<matriz_distancias.size();i++){
			for(int j=0;j<matriz_distancias.size();j++)
				cout<<matriz_distancias[i][j]<<" ";
			cout<<endl;
		}
	}

	void imprimirResultado() {
		for (auto it = camino.begin(); it != camino.end(); ++it)
			cout << it->n << " ";
		cout << endl;
		cout << "Distancia: " << distancia_total << endl;
	}
};

void operator<<(ostream &flujo, const vector<ciudad> &v) {
	for (auto it = v.begin(); it != v.end(); ++it)
		cout << "(" << it->x << "," << it->y << ") -> ";
	cout << endl;
}
int main(int argc, char **argv) {
	if (argc != 2) {
		cerr << "Error de formato: " << argv[0] << " <fichero>." << endl;
		exit(-1);
	}
	TSP uno(argv[1]);
	uno.VecinoMasCercano();
	uno.imprimirResultado();
	cout<<"MAtriz:"<<endl;
	uno.matrizDistancias();
}