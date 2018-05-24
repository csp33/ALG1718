
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
  ciudad &operator=(const ciudad &otra) {
    if (this != &otra) {
      x = otra.x;
      y = otra.y;
      n = otra.n;
    }
    return *this;
  }
};

bool operator==(const ciudad &una, const ciudad &otra) {
  return una.n == otra.n;
}
bool operator!=(const ciudad &una, const ciudad &otra) {
  return !(una == otra);
}

ostream &operator<<(ostream &flujo, const vector<ciudad> &v) {
  for (auto it = v.begin(); it != v.end(); ++it) {
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
  void RecBranchBound(double cota_actual, double peso_actual, int nivel,
                      vector<ciudad> solucion_parcial);
  double CalcularCotaInicial() const;

  double MenorEntrante(const ciudad &ciudad) const;

  double MenorSaliente(const ciudad &ciudad) const;
  double Distancia(const ciudad &a, const ciudad &b) const;

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

double TravelSalesman::Distancia(const ciudad &a, const ciudad &b) const {
  return matriz_distancias[a.n][b.n];
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
  double cota_inicial = CalcularCotaInicial();
  vector<ciudad> solucion_parcial;
  solucion_parcial.push_back(ciudades[0]); // Meto primera ciudad.
  visitados[0] = true;
  RecBranchBound(cota_inicial, 0, 1, solucion_parcial);
}

void TravelSalesman::RecBranchBound(double cota_actual, double peso_actual,
                                    int nivel,
                                    vector<ciudad> solucion_parcial) {
  if (nivel == ciudades.size()) { // Caso base
    double resultado_actual = peso_actual + Distancia(solucion_parcial.back(),
                                                      solucion_parcial.front());
    if (resultado_actual < distancia_total) {
      distancia_total = resultado_actual;
      camino = solucion_parcial;
      camino.push_back(camino.front());
    }
  } else { // Sigo expandiendo
    for (auto it = ciudades.begin(); it != ciudades.end(); ++it) {
      if (Distancia(*it, solucion_parcial.back()) != 0 && !visitados[it->n]) {
        double cota_local = cota_actual;
        double p_nuevo = peso_actual + Distancia(*it, solucion_parcial.back());

        if (nivel == 1)
          cota_local -= (MenorEntrante(solucion_parcial[nivel - 1]) +
                          MenorEntrante(*it)) /
                         2;
        else
          cota_local -= (MenorSaliente(solucion_parcial[nivel - 1]) +
                          MenorEntrante(*it)) /
                         2;
        double actual = cota_local + peso_actual;
        if (actual < distancia_total) { // La solución puede mejorar
          solucion_parcial.push_back(*it);
          visitados[it->n] = true;
          RecBranchBound(cota_local, p_nuevo, nivel + 1, solucion_parcial);
          solucion_parcial.erase(solucion_parcial.end()-1); //Deshago el cambio.
        }
        // Deshacemos cambios
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
