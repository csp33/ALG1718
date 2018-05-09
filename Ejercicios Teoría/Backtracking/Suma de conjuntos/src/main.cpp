#include <chrono>
#include <ctime>
#include <iostream>
#include <limits>
#include <list>
#include <ratio>
#include <vector>
using namespace std;
using namespace std::chrono;

#define DEBUG 0

ostream &operator<<(ostream &flujo, const vector<int> &v) {
  for (auto it = v.begin(); it != v.end(); ++it)
    flujo << *it << " ";
  return flujo;
}

class Solucion {

private:
  vector<int> candidatos;
  list<vector<int>> soluciones;
  int M;
  void FuerzaBrutaRecursiva(vector<int> &candidatos, int sum,
                            vector<int> actual);

public:
  Solucion(int tam, int m);
  int size() const;
  int getM() const;
  void PrintSoluciones() const;
  void FuerzaBruta();
};

Solucion::Solucion(int tam, int m) {
  this->M = m;
  candidatos.resize(tam);
  // Inicializamos los valores
  for (int i = 0; i < tam; i++)
    candidatos[i] = i + 1;
}

int Solucion::size() const { return candidatos.size(); }

int Solucion::getM() const { return M; }

void Solucion::PrintSoluciones() const {
  cout << soluciones.size() << endl;
  for (auto it = soluciones.begin(); it != soluciones.end(); ++it)
    cout << *it << endl;
}

void Solucion::FuerzaBrutaRecursiva(vector<int> &candidatos, int sum,
                                    vector<int> actual) {
  if (sum == 0) // Suma correcto
    soluciones.push_back(actual);
  else if (sum < 0) { // Me paso
    actual.clear();
  } else if (sum > 0) { // Me quedo corto
    for (int i = 0; i < candidatos.size(); i++) {
      actual.push_back(candidatos[i]);
      vector<int> nuevo(candidatos.begin() + i + 1, candidatos.end());
      FuerzaBrutaRecursiva(nuevo, sum - candidatos[i], actual);
      actual.erase(actual.end() - 1);
    }
  }
}

void Solucion::FuerzaBruta() {
  vector<int> actual;
  FuerzaBrutaRecursiva(candidatos, M, actual);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error de argumentos. Uso : " << argv[0] << " <tam> <m>" << endl;
    exit(-1);
  }
  int tam = atoi(argv[1]), m = atoi(argv[2]);
  Solucion s(tam, m);

  auto t1 = high_resolution_clock::now();
  s.FuerzaBruta();
  auto t2 = high_resolution_clock::now();

  duration<double> tiempo = duration_cast<duration<double>>(t2 - t1);

#if DEBUG
  cout << "----------Solución----------" << endl;
  s.PrintSoluciones();
  cout << "----------------------------" << endl;
#endif

  cout << tam << "\t" << m << "\t" << tiempo.count() << endl;
}
