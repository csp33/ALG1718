// C++ program to solve Traveling Salesman Problem
// using Branch and Bound.

#include <bits/stdc++.h>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <vector>
using namespace std;
const int N = 16;

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
  for (auto it = v.begin(); it != v.end(); ++it) {
    if (it != v.begin())
      flujo << "->";
    flujo << it->n;
  }
  return flujo;
}

// final_path[] stores the final solution ie, the
// path of the salesman.
int final_path[N + 1];

// visited[] keeps track of the already visited nodes
// in a particular path
bool visited[N];

// Stores the final minimum weight of shortest tour.
int final_res = INT_MAX;

class TravelSalesman {

private:
  vector<ciudad> ciudades;
  double distancia_total;
  vector<ciudad> camino;
  vector<vector<double>> matriz_distancias;
  vector<bool> visitados;

  double calcularDistanciaCamino(const vector<ciudad> &path) {
    double distancia = 0;
    for (int i = 0, j = 1; j < path.size(); i++, j++)
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
  TravelSalesman() {
    distancia_total = 0;
    ResetVisitados();
  }
  TravelSalesman(char *archivo) {
    CargarDatos(archivo);
    distancia_total = 0;
    ResetVisitados();
  }
  ~TravelSalesman() {}

  vector<vector<double>> getMatrizDistancias() { return matriz_distancias; }

  int GetTamanio() { return ciudades.size(); }

  void CargarDatos(char *archivo) {
    ifstream datos;
    string s;
    int n;
    ciudad aux;
    datos.open(archivo);
    if (datos.is_open()) {
      datos >> s; // Leo DIMENSIÓN (cabecera)
      datos >>
          n; // Leo NÚMERO de ciudades y reservo espacio en matrices y vector.
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
    } else
      cout << "Error al exportar." << endl;
  }

  // Copia una solución final a la solución final.
  void CopiarAFinal(int curr_path[]) {
    for (int i = 0; i < N; i++)
      final_path[i] = curr_path[i];
    final_path[N] = curr_path[0];
  }

  // Calcula la arista de menor coste que lleva a la ciudad i.
  // Function to find the minimum edge cost
  // having an end at the vertex i
  int firstMin(int adj[N][N], int i) {
    int min = INT_MAX;
    for (int k = 0; k < N; k++)
      if (adj[i][k] < min && i != k)
        min = adj[i][k];
    return min;
  }

  // Calcula la segunda arista de menor coste que lleva a la ciudad i.
  // function to find the second minimum edge cost
  // having an end at the vertex i
  int secondMin(int adj[N][N], int i) {
    int primera = INT_MAX, second = INT_MAX;
    for (int j = 0; j < N; j++) {
      if (i != j) {
        if (adj[i][j] <= primera) {
          second = primera;
          primera = adj[i][j];
        } else if (adj[i][j] <= second && adj[i][j] != primera)
          second = adj[i][j];
      }
    }
    return second;
  }

  // function that takes as arguments:
  // curr_bound -> lower bound of the root node
  // curr_weight-> stores the weight of the path so far
  // level-> current level while moving in the search
  //         space tree
  // curr_path[] -> where the solution is being stored which
  //                would later be copied to final_path[]
  void TSPRec(int adj[N][N], int curr_bound, int curr_weight, int level,
              int curr_path[]) {
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level == N) {
      // check if there is an edge from last vertex in
      // path back to the first vertex
      if (adj[curr_path[level - 1]][curr_path[0]] != 0) {
        // curr_res has the total weight of the
        // solution we got
        int curr_res = curr_weight + adj[curr_path[level - 1]][curr_path[0]];

        // Update final result and final path if
        // current result is better.
        if (curr_res < final_res) {
          CopiarAFinal(curr_path);
          final_res = curr_res;
        }
      }
      return;
    }

    // for any other level iterate for all vertices to
    // build the search space tree recursively
    for (int i = 0; i < N; i++) {
      // Consider next vertex if it is not same (diagonal
      // entry in adjacency matrix and not visited
      // already)
      if (adj[curr_path[level - 1]][i] != 0 && visited[i] == false) {
        int temp = curr_bound;
        curr_weight += adj[curr_path[level - 1]][i];

        // different computation of curr_bound for
        // level 2 from the other levels
        if (level == 1)
          curr_bound -=
              ((firstMin(adj, curr_path[level - 1]) + firstMin(adj, i)) / 2);
        else
          curr_bound -=
              ((secondMin(adj, curr_path[level - 1]) + firstMin(adj, i)) / 2);

        // curr_bound + curr_weight is the actual lower bound
        // for the node that we have arrived on
        // If current lower bound < final_res, we need to explore
        // the node further
        if (curr_bound + curr_weight < final_res) {
          curr_path[level] = i;
          visited[i] = true;

          // call TSPRec for the next level
          TSPRec(adj, curr_bound, curr_weight, level + 1, curr_path);
        }

        // Else we have to prune the node by resetting
        // all changes to curr_weight and curr_bound
        curr_weight -= adj[curr_path[level - 1]][i];
        curr_bound = temp;

        // Also reset the visited array
        memset(visited, false, sizeof(visited));
        for (int j = 0; j <= level - 1; j++)
          visited[curr_path[j]] = true;
      }
    }
  }

  // This function sets up final_path[]
  //Calcula un camino mediante Branch&Bound
  void TSP(int adj[N][N]) {
    int curr_path[N + 1];

    // Calculate initial lower bound for the root node
    // using the formula 1/2 * (sum of first min +
    // second min) for all edges.
    // Also initialize the curr_path and visited array
    int curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(curr_path));

    // Compute initial bound
    for (int i = 0; i < N; i++)
      curr_bound += (firstMin(adj, i) + secondMin(adj, i));

    // Rounding off the lower bound to an integer
    curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 : curr_bound / 2;

    // We start at vertex 1 so the first vertex
    // in curr_path[] is 0
    visited[0] = true;
    curr_path[0] = 0;

    // Call to TSPRec for curr_weight equal to
    // 0 and level 1
    TSPRec(adj, curr_bound, 0, 1, curr_path);
  }
};
// Driver code
int main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Error de formato: " << argv[0] << " <fichero>." << endl;
    exit(-1);
  }

  TravelSalesman instancia(argv[1]);
  int adj[N][N];
  vector<vector<double>> matriz = instancia.getMatrizDistancias();
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      adj[i][j] = matriz[i][j];

  instancia.TSP(adj);

  printf("Minimum cost : %d\n", final_res);
  printf("Path Taken : ");
  for (int i = 0; i <= N; i++)
    printf("%d ", final_path[i]);
  printf("\n");
  return 0;
}
