#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cmath>
#include <string>

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

	void InsercionMasEconomica() {

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
			salida << "DISTANCIA: "<<distancia_total<<endl;
			for (auto it = camino.begin(); it != camino.end(); ++it) {
				salida << it->n + 1 << " " << it->x << " " << it->y << endl;
			}
			salida.close();
		}
		else
			cout << "Error al exportar." << endl;
	}

};


int main(int argc, char **argv) {

	if (argc != 2) {
		cerr << "Error de formato: " << argv[0] << " <fichero>." << endl;
		exit(-1);
	}

	string nombre="";

	/*********** Vecino más cercano*******************/
	TSP vecino_mas_cercano(argv[1]);
	cout << "Heurística del Vecino más cercano." << endl;
	vecino_mas_cercano.VecinoMasCercano();
	vecino_mas_cercano.imprimirResultado();

	nombre = "vmc";
	nombre += to_string(vecino_mas_cercano.GetTamanio());
	nombre += ".tsp";
	vecino_mas_cercano.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;

	/*********** Inserción más económica*******************/

	TSP insercion_mas_economica(argv[1]);

	cout << "Heurística de la inserción más económica." << endl;
	insercion_mas_economica.InsercionMasEconomica();
	insercion_mas_economica.imprimirResultado();

	/*nombre = "ime_";
	nombre += argv[1];
	insercion_mas_economica.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;*/


	/*********** Derivado de Kruskal*******************/

	TSP derivado_kruskal(argv[1]);

	cout << "Heurística derivada de Kruskal." << endl;
	derivado_kruskal.DerivadoKruskal();
	derivado_kruskal.imprimirResultado();

	/*nombre = "kruskal_";
	nombre += argv[1];

	derivado_kruskal.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;*/

}