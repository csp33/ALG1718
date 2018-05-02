#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cmath>
#include <string>

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


public:
	TSP() {
		distancia_total = 0;
	}
	TSP(char *archivo) {
		CargarDatos(archivo);
		distancia_total = 0;
	}
	~TSP() {

	}

	int GetTamanio() {
		return ciudades.size();
	}


	void MismoOrden(char *archivo) {
		camino.clear();
		ifstream datos;
		string s;
		int n;
		int auxiliar;
		datos.open(archivo);
		if (datos.is_open()) {
			datos >> s;	//Leo cabecera
			datos >> n;	//Leo número ciudades
			for (int i = 0; i < ciudades.size(); i++) {
				datos >> auxiliar;		//Leo número de ciudad
				auxiliar--;
				bool fin = false;
				for (auto it = ciudades.begin(); it != ciudades.end() && !fin; ++it) {	//Busco la ciudad y la inserto en el camino
					if (it->n == auxiliar) {
						camino.push_back(*it);
						fin = true;
					}
				}
			}
			camino.push_back(camino[0]);
			distancia_total = calcularDistanciaCamino(camino);
		}
		else
			cout << "Error al leer" << archivo << endl;
	}

	int getDistancia(){
		return distancia_total;
	}

	void CargarDatos(char *archivo) {
		ifstream datos;
		string s;
		int n;
		ciudad aux;
		datos.open(archivo);
		if (datos.is_open()) {
			datos >> s;	//Leo DIMENSIÓN (cabecera)
			datos >> n;	//Leo NÚMERO de ciudades .

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

};

// Modo de uso: ./programa coordenadas orden
int main(int argc, char **argv) {

	if (argc != 3) {
		cerr << "Error de formato: " << argv[0] << " <coordenadas> <orden>." << endl;
		exit(-1);
	}

	string nombre = "";

	/*********** Vecino más cercano*******************/
	TSP instancia(argv[1]);




	instancia.MismoOrden(argv[2]);

	nombre = "mejor_";
	nombre += to_string(instancia.GetTamanio());
	nombre += ".tsp";
	instancia.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;

	cout<<"DISTANCIA "<<instancia.getDistancia()<<endl;


}