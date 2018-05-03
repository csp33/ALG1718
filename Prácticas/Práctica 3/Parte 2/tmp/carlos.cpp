#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <limits>
#include <cstddef>
#include <cmath>

using namespace std;

struct ciudad {
	int n;
	double x;
	double y;
	int grado;
	ciudad * anterior;
	ciudad * siguiente;
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
	vector<vector <double> > matriz_distancias;
	vector<bool> visitados;

	double calcularDistancia(const vector<ciudad> &path) {
		double distancia = 0;
		int i = 0;
		for (int j = 1; j < path.size(); j++) {
			distancia += distanciaEuclidea(path[i], path[j]);
			i++;
		}
		return distancia;
	}
	double distanciaEuclidea(const ciudad &una, const ciudad &otra) {
		double resultado;
		if (una == otra)
			resultado = 0;
		else
			resultado = sqrt(pow(una.x - otra.x, 2) + pow(una.y - otra.y, 2));
		return resultado;
	}

public:

	TSP() {

	}
	TSP(char *archivo) {
		CargarDatos(archivo);

		for (auto it = visitados.begin(); it != visitados.end(); ++it)
			*it = false;

	}
	~TSP() {

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
			visitados.resize(n);
			matriz_distancias.resize(n);

			for (int i = 0; i < n; i++)
				matriz_distancias[i].resize(n);

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

	void matrizDistancias() {
		for (int i = 0; i < matriz_distancias.size(); i++) {
			for (int j = 0; j < matriz_distancias.size(); j++)
				cout << matriz_distancias[i][j] << " ";
			cout << endl;
		}
	}

	void imprimirResultado() {
		for (auto it = camino.begin(); it != camino.end(); ++it)
			cout << it->n + 1 << " ";
		cout << endl;
		cout << "Distancia: " << distancia_total << endl;

	}

	/*************************************************************************/


	void Triangularizar(ciudad & norte, ciudad & este,
	                    ciudad & oeste) {
		/*	double x_max, x_min, y_max;
			x_max=-INFINITY;
			y_max=-INFINITY;
			x_min=INFINITY;
			for (int i = 0; i < ciudades.size(); i++) {
				if (ciudades[i].x > x_max) {
					x_max = ciudades[i].x;
					este = ciudades[i];
				}
				else if (ciudades[i].x < x_min) {
					x_min = ciudades[i].x;
					oeste = ciudades[i];
				}
				else if (ciudades[i].y > y_max) {
					y_max = ciudades[i].y;
					norte = ciudades[i];
				}
			}
			*/
		int indice_este = 0, indice_oeste = 0, indice_norte = 0;
		int minimo_x = 10000, maximo_x = 0, maximo_y = 0;

		for (int i = 0; i < ciudades.size(); i++) {
			//Calcular ciudad más al oeste
			if (ciudades[i].x < minimo_x) {
				minimo_x = ciudades[i].x;
				indice_oeste = i;
			}
			//Calcular ciudad más al norte
			if (ciudades[i].y > maximo_y) {
				maximo_y = ciudades[i].y;
				indice_norte = i;
			}
			//Calcular ciudad más al este
			if (ciudades[i].x > maximo_x) {
				maximo_x = ciudades[i].x;
				indice_este = i;
			}
		}
		norte=ciudades[indice_norte];
		este=ciudades[indice_este];
		oeste=ciudades[indice_oeste];
	}

	void seleccionarNuevaCiudad() {
		ciudad actual;
		ciudad a_insertar;
		int indice;
		double dist_minima = INFINITY;
		double d_aux;
		for (int i = 0; i < ciudades.size(); i++) {		//Itero por todas las posibles ciudades
			actual = ciudades[i];
			if (!visitados[actual.n]) {				//Si no la he visitado
				vector<ciudad> aux = camino;
				for (int j = 0; j < camino.size(); j++) {		//Veo en que posición podría insertarla
					aux.insert(aux.begin() + j, actual);
					d_aux = calcularDistancia(aux);
					if (d_aux < dist_minima) {		//Me quedo con la que menos incrementa la distancia
						dist_minima = d_aux;
						a_insertar = actual;
						indice = j;
					}
				}
			}
		}
		camino.insert(camino.begin() + indice, a_insertar);
		visitados[a_insertar.n] = true;
	}

	void InsercionMasEconomica() {
		camino.clear();
		ciudad norte, este, oeste;
		//Hallo triángulo inicial
		Triangularizar(norte, este, oeste);
		//Añado al camino
		camino.push_back(oeste);
		camino.push_back(norte);
		camino.push_back(este);
		camino.push_back(oeste);

		visitados[norte.n] = true;
		visitados[este.n] = true;
		visitados[oeste.n] = true;

		cout << "Circuito Parcial:" << endl;
		imprimirResultado();


		//Voy eligiendo la siguiente
		while (camino.size() != ciudades.size())		//Mientras no recorramos todas las ciudades
			seleccionarNuevaCiudad();
		//Añado el inicio
		camino.push_back(camino[0]);
		distancia_total = calcularDistancia(camino);

	}
	void Exportar(const char *name) {
		ofstream salida;
		salida.open(name);
		if (salida.is_open()) {
			salida << "DIMENSION: ";
			salida << ciudades.size() << endl;
			salida << "DISTANCIA: " << distancia_total << endl;
			for (auto it = camino.begin(); it != camino.end(); ++it) {
				salida << it->n + 1 << " " << it->x << " " << it->y << endl;
			}
			salida.close();
		}
		else
			cout << "Error al exportar." << endl;
	}

	int GetTamanio() {
		return ciudades.size();
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
	TSP insercion_mas_economica(argv[1]);

	cout << "Heurística de la inserción más económica." << endl;
	insercion_mas_economica.InsercionMasEconomica();
	insercion_mas_economica.imprimirResultado();

	string nombre = "ime";
	nombre += to_string(insercion_mas_economica.GetTamanio());
	nombre += ".tsp";
	insercion_mas_economica.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;
}
