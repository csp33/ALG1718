#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cmath>
#include <string>
#include <limits>
#include <list>
using namespace std;

#define TEST 0

struct ciudad {
	int n;
	double x;
	double y;
	//int grado = 0;
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
	/*
		pair< pair<ciudad, ciudad>, double> calularMinimo(pair< pair<ciudad, ciudad>, double> &min,
		        vector<vector <bool> > &matrizVisitados, vector<list<ciudad>> &conex, vector<bool> &aniadida) {
			cout << "entra calcularminimo" << endl;

			int x = 0;
			int y = 0;
			pair< pair<ciudad, ciudad>, double> resultado;
			resultado.second = INFINITY;

			for (int i = 0; i < matriz_distancias.size(); i++) {

				for (int j = i + 1; j < matriz_distancias.size(); j++) {

					if (ciudades[i].grado < 2 and ciudades[j].grado < 2) {

						if (matriz_distancias[i][j] >= min.second and
						        matriz_distancias[i][j] < resultado.second and
						        matriz_distancias[i][j] != 0 and
						        not matrizVisitados[i][j] and
						        !hay_ciclo (conex, ciudades[i], ciudades[j], matrizVisitados)) {

							resultado.second = matriz_distancias[i][j];
							(resultado.first).first = ciudades[i];
							(resultado.first).second = ciudades[j];
							x = i;
							y = j;
						}
					}
				}
			}


			if (!conex[x].empty() and !aniadida[y] and not (conex[x].size() == ciudades.size() || conex[y].size() == ciudades.size())) {

				if (conex[y].size() > 1) {

					for (auto it = conex[y].begin(); it != conex[y].end(); ++it) {
						cout << "pb1" << endl;

						conex[x].push_back(*it);
					}

				}
				else {
					cout << "pb2" << endl;

					conex[x].push_back(ciudades[y]);
				}

				aniadida[y] = true;
				conex[y].clear();
			}
			else if (!conex[x].empty() and aniadida[y] and not (conex[x].size() == ciudades.size() || conex[y].size() == ciudades.size())) {

				for (int k = 0; k < conex.size(); k++) {
					for (auto it = conex[k].begin(); it != conex[k].end(); ++it) {

						if (*it == ciudades[y]) {
							if (conex[x].size() > 1) {

								for (auto it2 = conex[x].begin(); it2 != conex[x].end(); ++it2) {
									cout << "pb3" << endl;

									conex[k].push_back(*it2);
								}
							}
							else {
								cout << "pb4" << endl;

								conex[k].push_back(ciudades[x]);
							}
							aniadida[x] = true;
							conex[x].clear();
						}
					}
				}
			}
			else if (conex[x].empty() and !aniadida[y] and not (conex[x].size() == ciudades.size() || conex[y].size() == ciudades.size())) {

				for (int k = 0; k < conex.size(); k++) {
					for (auto it = conex[k].begin(); it != conex[k].end(); ++it) {

						if (*it == ciudades[x]) {
							if (conex[y].size() > 1) {

								for (auto it2 = conex[y].begin(); it2 != conex[y].end(); ++it2) {
									cout << "pb5" << endl;
									conex[k].push_back(*it2);

								}
							}
							else {
								cout << "pb6" << endl;

								conex[k].push_back(ciudades[y]);

							}
							aniadida[y] = true;
							conex[y].clear();
						}
					}
				}
			}

			matrizVisitados[x][y] = true;
			ciudades[x].grado += 1;
			ciudades[y].grado += 1;

			cout << "Fin calculaMin" << endl;

			return resultado;
		}

		bool hay_ciclo(vector<list<ciudad>> &conex, const ciudad & primera, const ciudad & segunda, vector<vector <bool> > &matrizVisitados) {

			cout << "entra hay ciclo" << endl;
			bool primera_enc = false;
			bool hay = false;
			ciudad restante;
			bool primera_vez = true;

			for (int i = 0; i < conex.size(); i++) {
				for (auto it = conex[i].begin(); it != conex[i].end() && !hay; ++it) {

					if (*it == primera || *it == segunda && primera_vez) {

						primera_enc = true;
						primera_vez = false;

						if (*it == primera) {

							restante = segunda;
						}
						else {

							restante = primera;
						}
					}

					if (primera_enc && *it == restante) {

						hay = true;
						matrizVisitados[primera.n][segunda.n] = true;
					}
				}

				primera_vez = true;
			}
			cout << "Fin hayCiclo" << endl;
			return (hay);
		}
	*/

	pair<int, int> ArcoMenor() {
		pair<int, int> resultado;
		double d_minima = numeric_limits<double>::max();
		for (int i = 0; i < ciudades.size(); i++) {
			for (int j = 0; j < ciudades.size(); j++) {
				if (i != j && !visitados[i] && !visitados[j]) {
					if (matriz_distancias[i][j] < d_minima) {
						d_minima = matriz_distancias[i][j];
						resultado.first = i;
						resultado.second = j;
					}
				}
			}
		}

		visitados[resultado.first] = true;
		visitados[resultado.second] = true;
		return resultado;
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

	void DerivadoKruskal() {
		camino.clear();
		while (camino.size() != ciudades.size()) {
			pair<int, int> arcoMenor = ArcoMenor();
			camino.push_back(ciudades[arcoMenor.first]);
			camino.push_back(ciudades[arcoMenor.second]);
		}
		for (int i = 0; i < ciudades.size(); i++)
			if (!visitados[i]){
				camino.push_back(ciudades[i]);		//Si queda alguna por visitar. Pasa cuando NCIUDADES es impar
				visitados[i]=true;
			}
		camino.push_back(camino[0]);
		distancia_total = calcularDistanciaCamino(camino);

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
	/*
		void algoritmo_de_laura_y_jose() {
			vector< pair<ciudad, ciudad> > aristas;
			pair< pair<ciudad, ciudad>, double> arista_minima;
			ciudad ciudad_actual;
			vector<vector <bool> > matrizVisitados (matriz_distancias.size(), vector<bool>(matriz_distancias.size(), false));
			vector<list<ciudad>> ciudades_conex;
			vector<bool> aniadida (ciudades.size(), false);

			ciudades_conex.resize (ciudades.size());

			//Cada ciudad está conexa consigo mismo
			for (int in = 0; in < ciudades_conex.size(); in++)
				ciudades_conex[in].push_back (ciudades[in]);

			arista_minima.second = 0;

			for (int i = 0; i < ciudades.size(); i++) {

				arista_minima = calularMinimo(arista_minima, matrizVisitados, ciudades_conex, aniadida);
				cout << "i= " << i << endl;
				aristas.push_back(arista_minima.first);
				distancia_total += arista_minima.second;
			}

			vector<bool> arista_leida (aristas.size(), false);

			camino.push_back(aristas[0].first);
			camino.push_back(aristas[0].second);
			ciudad_actual = aristas[0].second;
			arista_leida[0] = true;
			for (auto it = matrizVisitados.begin(); it != matrizVisitados.end(); ++it)
				it->clear();
			matrizVisitados.clear();

			for (auto it = aristas.begin(); it != aristas.end(); ++it)
				cout << "(" << it->first.n << "," << it->second.n << ") ";
			cout << endl;

			while (camino.size() < ciudades.size()) {

				for (int i = 1; i < aristas.size(); i++) {

					if (ciudad_actual == aristas[i].first && !arista_leida[i]) {

						ciudad_actual = aristas[i].second;
						camino.push_back(ciudad_actual);
						arista_leida[i] = true;
					}
					else if (ciudad_actual == aristas[i].second && !arista_leida[i]) {

						ciudad_actual = aristas[i].first;
						camino.push_back(ciudad_actual);
						arista_leida[i] = true;
					}
				}
			}
		}
	*/
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
	//derivado_kruskal.algoritmo_de_laura_y_jose();
	derivado_kruskal.imprimirResultado();

	nombre = "kruskal";
	nombre += to_string(derivado_kruskal.GetTamanio());
	nombre += ".tsp";
	derivado_kruskal.Exportar(nombre.c_str());
	cout << "Exportado archivo " << nombre << endl;

	cout << "**********************************************" << endl;


}