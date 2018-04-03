
#define TEST 0

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

double uniforme() {
	int t = rand();
	double f = ((double)RAND_MAX + 1.0);
	return (double)t / f;
}

int elementoEnSuPosicion(const vector<int> v) {
	for (int i = 0; i < v.size() ; i++)
		if (v[i] == i)
			return i;
	return -1;
}
int main(int argc, char * argv[]) {

	if (argc != 2) {
		cerr << "Formato " << argv[0] << " <num_elem>" << endl;
		return -1;
	}

	int n = atoi(argv[1]);
	int m = 2 * n - 1;

	int * T = new int[n];
	assert(T);
	int * aux = new int[m];
	assert(aux);

	srand(time(0));
	high_resolution_clock::time_point tantes;
	high_resolution_clock::time_point tdespues;
	duration<double> tiempo;
	double acumulado = 0;
	for (int j = 0; j < m; j++)
		aux[j] = j - (n - 1);

	for (int j = m - 1; j > 0; j--) {
		double u = uniforme();
		int k = (int)(j * u);
		int tmp = aux[j];
		aux[j] = aux[k];
		aux[k] = tmp;
	}
	//me quedo con los n primeros del vector
	for (int j = 0; j < n; j++) T[j] = aux[j];

#if TEST
	for (int j = 0; j < n; j++) cout << T[j] << " ";
#endif
	//Y ahora ordeno el vector T
	vector<int> myvector (T, T + n);
	vector<int>::iterator it;

	sort(myvector.begin(), myvector.end());

#if TEST
	for (it = myvector.begin(); it != myvector.end(); ++it)
		cout << " " << *it;
	cout << endl;
#endif
	int pos;
	for (int i = 0; i < 1000; i++) {
		tantes = high_resolution_clock::now();
		pos = elementoEnSuPosicion(myvector);
		tdespues = high_resolution_clock::now();
		tiempo = duration_cast<duration<double>>
		         (tdespues - tantes);
		acumulado += tiempo.count();
	}
	acumulado /= 1000;

	cout << n << "\t" << acumulado << endl;
#if TEST
	cout << "Posición : " << pos << endl;
#endif

	delete [] aux;
	delete [] T;

}
