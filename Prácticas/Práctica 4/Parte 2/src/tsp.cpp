
#include <chrono>
#include <climits>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;

#define DEBUG 0

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

ostream &operator<<(ostream &flujo, const vector<ciudad> &v) {
  for (auto it = v.begin(); it != v.end() ; ++it) {
    if (it != v.begin())
      flujo << "->";
    flujo << it->n + 1;
  }
  return flujo;
}
class TravelSalesman {

private:
  const double MAX = numeric_limits<double>::max();
  vector<ciudad> ciudades; // Almaceno problema
  double distancia_total;  // Distancia del circuito
  vector<ciudad> camino;   // Solución final.
  vector<vector<double>> matriz_distancias;
  vector<bool> visitados;
  double calcularDistanciaCamino(const vector<ciudad> &path) const;
  double distanciaEuclidea(const ciudad &una, const ciudad &otra) const;
  void InicializarMatrizDistancias();
  void Reservar(int n);
  void ResetVisitados();
  void RecBranchBound(int cota_actual, double peso_actual, int nivel,
                      vector<ciudad> solucion_parcial);
  double CalcularCotaInicial() const;

  double MenorEntrante(const ciudad &ciudad) const;

  double MenorSaliente(const ciudad &ciudad) const;

public:
  TravelSalesman();
  TravelSalesman(char *archivo);
  int GetTamanio() const;
  void CargarDatos(char *archivo);
  void imprimirResultado() const;
  void Exportar(const char *name) const;

  void BranchBound();
};

TravelSalesman::TravelSalesman() {
  distancia_total = MAX;
  ResetVisitados();
}

TravelSalesman::TravelSalesman(char *archivo) {
  CargarDatos(archivo);
  distancia_total = MAX;
  ResetVisitados();
}

void TravelSalesman::imprimirResultado() const {
  cout << endl << "Mejor solución:" << endl;
  cout << camino << endl;
  cout << "Distancia: " << distancia_total << endl;
}

void TravelSalesman::Exportar(const char *name) const {
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
  } else
    cout << "Error al exportar." << endl;
}

void TravelSalesman::CargarDatos(char *archivo) {
  ifstream datos;
  string s;
  int n;
  ciudad aux;
  datos.open(archivo);
  if (datos.is_open()) {
    datos >> s; // Leo DIMENSIÓN (cabecera)
    datos >> n; // Leo NÚMERO de ciudades .
    Reservar(n);

    for (int i = 0; i < n; i++) {
      datos >> aux.n; // Leo número de ciudad
      aux.n--; // Decremento el número: los índices del archivo comienzan
               // en 1. Los del vector en 0.
      datos >> aux.x >> aux.y; // Leo coordenadas
      ciudades.push_back(aux);
    }
    datos.close();
  } else
    cout << "Error al leer " << archivo << endl;
  InicializarMatrizDistancias();
}

int TravelSalesman::GetTamanio() const { return ciudades.size(); }

void TravelSalesman::ResetVisitados() {
  for (auto it = visitados.begin(); it != visitados.end(); ++it)
    *it = false;
}

double
TravelSalesman::calcularDistanciaCamino(const vector<ciudad> &path) const {
  double distancia = 0;
  for (int i = 0, j = 1; j < path.size(); i++, j++)
    distancia += distanciaEuclidea(path[i], path[j]);
  return distancia;
}

double TravelSalesman::distanciaEuclidea(const ciudad &una,
                                         const ciudad &otra) const {
  double resultado;
  if (una == otra)
    resultado = 0;
  else
    resultado = sqrt(pow(una.x - otra.x, 2) + pow(una.y - otra.y, 2));
  return resultado;
}

void TravelSalesman::InicializarMatrizDistancias() {
  for (int i = 0; i < ciudades.size(); i++)
    for (int j = 0; j < ciudades.size(); j++)
      matriz_distancias[i][j] = distanciaEuclidea(ciudades[i], ciudades[j]);
}

void TravelSalesman::Reservar(int n) {
  visitados.resize(n);
  matriz_distancias.resize(n);
  for (int i = 0; i < n; i++)
    matriz_distancias[i].resize(n);
}

double TravelSalesman::MenorEntrante(const ciudad &ciudad) const {
  double menor = MAX;
  for (int i = 0; i < ciudades.size(); i++)
    if (i != ciudad.n && matriz_distancias[i][ciudad.n] < menor)
      menor = matriz_distancias[i][ciudad.n];
  return menor;
}

double TravelSalesman::MenorSaliente(const ciudad &ciudad) const {
  double menor_entrante = MAX, menor_saliente = MAX;
  for (int i = 0; i < ciudades.size(); i++) {
    if (ciudad.n != i) {
      if (matriz_distancias[i][ciudad.n] <= menor_entrante) {
        menor_saliente = menor_entrante;
        menor_entrante = matriz_distancias[i][ciudad.n];
      } else if (matriz_distancias[i][ciudad.n] <= menor_saliente &&
                 matriz_distancias[i][ciudad.n] != menor_entrante)
        menor_saliente = matriz_distancias[i][ciudad.n];
    }
  }
  return menor_saliente;
}

double TravelSalesman::CalcularCotaInicial() const {
  double cota = 0;
  for (auto it = ciudades.begin(); it != ciudades.end(); ++it)
    cota += MenorEntrante(*it) + MenorSaliente(*it);
  cota /= 2;
  return cota;
}

void TravelSalesman::BranchBound() {
  double cota_inferior = CalcularCotaInicial();
  vector<ciudad> solucion_parcial;
  camino.resize(ciudades.size() + 1);
  solucion_parcial.resize(ciudades.size() + 1);
  ciudad primera = ciudades[0];
  solucion_parcial.push_back(primera); // Meto primera ciudad.
  visitados[primera.n] = true;
  RecBranchBound(cota_inferior, 0, 1, solucion_parcial);
  camino.erase(camino.end()-1);
}

void TravelSalesman::RecBranchBound(int cota_actual, double peso_actual,
                                    int nivel,
                                    vector<ciudad> solucion_parcial) {
  int n_primera = solucion_parcial[0].n;
  int n_ultima = solucion_parcial[nivel - 1].n;
  if (nivel == ciudades.size()) { // Caso base
    double resultado_actual =
        peso_actual + matriz_distancias[n_primera][n_ultima];
    if (resultado_actual < distancia_total) {
      distancia_total = resultado_actual;
      camino = solucion_parcial;
      //  camino.push_back(camino[0]);
    }
  } else { // Sigo expandiendo
    for (auto it = ciudades.begin(); it != ciudades.end(); ++it) {
      if (matriz_distancias[n_ultima][it->n] != 0 && !visitados[it->n]) {
        double temp = cota_actual; // Guardo cota actual
        peso_actual += matriz_distancias[n_ultima][it->n];
        if (nivel == 1)
          cota_actual -= (MenorEntrante(solucion_parcial[nivel - 1]) +
                          MenorEntrante(*it)) /
                         2;
        else
          cota_actual -= (MenorSaliente(solucion_parcial[nivel - 1]) +
                          MenorEntrante(*it)) /
                         2;
        double actual = cota_actual + peso_actual;
        if (actual < distancia_total) { // La solución puede mejorar
          solucion_parcial[nivel] = *it;
          visitados[it->n] = true;
          RecBranchBound(cota_actual, peso_actual, nivel + 1, solucion_parcial);
        }
        // Podamos
        peso_actual -= matriz_distancias[it->n][n_ultima];
        cota_actual = temp; // Restauro la cota.
        ResetVisitados();
        for (auto it = solucion_parcial.begin(); it != solucion_parcial.end();
             ++it)
          visitados[it->n] = true;
      }
    }
  }
}
int main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Error de formato: " << argv[0] << " <fichero>." << endl;
    exit(-1);
  }
  TravelSalesman instancia(argv[1]);

  auto tantes = high_resolution_clock::now();

  instancia.BranchBound();

  auto tdespues = high_resolution_clock::now();

  double tiempo = duration_cast<duration<double>>(tdespues - tantes).count();

  cout << "Tamaño=" << instancia.GetTamanio() << " Tiempo (s)=" << tiempo
       << endl;

  string nombre;
  nombre = "bb";
  nombre += to_string(instancia.GetTamanio());
  nombre += ".tsp";
  instancia.Exportar(nombre.c_str());

#if DEBUG
  instancia.imprimirResultado();
#endif

  return 0;
}
