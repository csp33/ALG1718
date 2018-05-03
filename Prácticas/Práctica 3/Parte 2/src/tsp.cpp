#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cmath>
#include <string>
#include <limits>
using namespace std;

#define TEST 0

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

ostream& operator<<(ostream &flujo, const vector<ciudad> &v) {
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin())
			flujo << "->";
		flujo << it->n ;
	}
	return flujo;
}

class TSP {

private:
	vector<ciudad> ciudades;
	double distancia_total;
	vector<ciudad> camino;
	vector<vector<double>> matriz_distancias;
	vector<bool> visitados;

	double calcularDistanciaCamino(const vector<ciudad> &path) {
		double distancia = 0;
		for (int i = 0, j = 1; j < path.size(); i++ , j++)
			distancia += distanciaEuclidea(path[i], path[j]);
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

	pair<vector<ciudad>, double> VecinoMasCercanoParcial(int inicial) {
		vector<ciudad> resultado;
		ciudad actual = ciudades[inicial];
		ciudad siguiente;
		bool fin = false;
		resultado.push_back(actual);
		while (!fin) {
			visitados[actual.n] = true;						//Pongo como visitados
			int indice_siguiente = CiudadMasCercana(actual);	//Busco el indice de la siguiente ciudad
			if (indice_siguiente != -1)							// Si he recorrido todas las ciudades, añado la primera.
				siguiente = ciudades[indice_siguiente];
			else {
				fin = true;
				siguiente = resultado[0];	//Volvemos al inicio
			}
			resultado.push_back(siguiente);
			actual = siguiente;
		}
		double distancia = calcularDistanciaCamino(resultado);
		pair<vector<ciudad>, double> par;
		par.first = resultado;
		par.second = distancia;
		return par;
	}

	void InicializarMatrizDistancias() {
		for (int i = 0; i < ciudades.size(); i++)
			for (int j = 0; j < ciudades.size(); j++)
				matriz_distancias[i][j] = distanciaEuclidea(ciudades[i], ciudades[j]);
	}

	void Reservar(int n) {
		visitados.resize(n);
		matriz_distancias.resize(n);
		for (int i = 0; i < n; i++)
			matriz_distancias[i].resize(n);
	}

	void ResetVisitados() {
		for (auto it = visitados.begin(); it != visitados.end(); ++it)
			*it = false;
	}

	void Triangularizar(ciudad & norte, ciudad & este,
	                    ciudad & oeste) {

		double x_max, x_min, y_max;
		x_max = numeric_limits<double>::min();
		y_max = numeric_limits<double>::min();
		x_min = numeric_limits<double>::max();

		for (int i = 0; i < ciudades.size(); i++) {
			if (ciudades[i].x > x_max) {	//Más al este
				x_max = ciudades[i].x;
				este = ciudades[i];
			}
			if (ciudades[i].x < x_min) {	//Más al oeste
				x_min = ciudades[i].x;
				oeste = ciudades[i];
			}
			if (ciudades[i].y > y_max) {	//Más al norte
				y_max = ciudades[i].y;
				norte = ciudades[i];
			}
		}
	}

	void seleccionarNuevaCiudad() {
		ciudad actual;
		ciudad a_insertar;
		int indice;
		double dist_minima = numeric_limits<double>::max();
		double d_aux;
		for (int i = 0; i < ciudades.size(); i++) {		//Itero por todas las posibles ciudades
			actual = ciudades[i];
			if (!visitados[actual.n]) {				//Si no la he visitado
				for (int j = 1; j < camino.size() - 1; j++) {		//Veo en que posición podría insertarla
					vector<ciudad> aux = camino;
					aux.insert(aux.begin() + j, actual);
					d_aux = calcularDistanciaCamino(aux);
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

public:
	TSP() {
		distancia_total = 0;
		ResetVisitados();
	}
	TSP(char *archivo) {
		CargarDatos(archivo);
		distancia_total = 0;
		ResetVisitados();
	}
	~TSP() {

	}

	void VecinoMasCercano() {
		pair<vector<ciudad>, double> minimo, temp;
		minimo = VecinoMasCercanoParcial(0);		//Calculo el vecino más cercano comenzando por el primero
		for (int i = 0; i < ciudades.size(); i++) {
			ResetVisitados();
			temp = VecinoMasCercanoParcial(i);
#if TEST
			cout << temp.first << endl;
			cout << "Distancia " << temp.second << endl;
#endif
			if (temp.second < minimo.second)
				minimo = temp;
		}
		camino = minimo.first;
		distancia_total = minimo.second;
	}


	int GetTamanio() {
		return ciudades.size();
	}

	void DerivadoKruskal() {

	}

	void CargarDatos(char *archivo) {
		ifstream datos;
		string s;
		int n;
		ciudad aux;
		datos.open(archivo);
		if (datos.is_open()) {
			datos >> s;	//Leo DIMENSIÓN (cabecera)
			datos >> n;	//Leo NÚMERO de ciudades y reservo espacio en matrices y vector.
			Reservar(n);

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

		InicializarMatrizDistancias();
	}

	void imprimirResultado() {
		cout << endl << "Mejor solución:" << endl;
		cout << camino << endl;
		cout << "Distancia: " << distancia_total << endl;
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
		//camino.push_back(camino[0]);
		distancia_total = calcularDistanciaCamino(camino);

	}

};


int main(int argc, char **argv) {

	if (argc != 2) {
		cerr << "Error de formato: " << argv[0] << " <fichero>." << endl;
		exit(-1);
	}

	string nombre = "";

	/*********** Vecino más cercano*******************/
	cout << "**********************************************" << endl;

	TSP vecino_mas_cercano(argv[1]);
	cout << "Heurística del Vecino más cercano." << endl;
	vecino_mas_cercano.VecinoMasCercano();
	vecino_mas_cercano.imprimirResultado();

	nombre = "vmc";
	nombre += to_string(vecino_mas_cercano.GetTamanio());
	nombre += ".tsp";
	vecino_mas_cercano.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;

	cout << "**********************************************" << endl;


	/*********** Inserción más económica*******************/

	cout << "**********************************************" << endl;


	TSP insercion_mas_economica(argv[1]);

	cout << "Heurística de la inserción más económica." << endl;
	insercion_mas_economica.InsercionMasEconomica();
	insercion_mas_economica.imprimirResultado();

	nombre = "ime";
	nombre += to_string(insercion_mas_economica.GetTamanio());
	nombre += ".tsp";
	insercion_mas_economica.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;

	cout << "**********************************************" << endl;



	/*********** Derivado de Kruskal*******************/

	cout << "**********************************************" << endl;


	TSP derivado_kruskal(argv[1]);

	cout << "Heurística derivada de Kruskal." << endl;
	derivado_kruskal.DerivadoKruskal();
	derivado_kruskal.imprimirResultado();

	nombre = "kruskal";
	nombre += to_string(derivado_kruskal.GetTamanio());
	nombre += ".tsp";
	derivado_kruskal.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;

	cout << "**********************************************" << endl;


}