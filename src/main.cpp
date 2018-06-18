#include <vector>
#include "utils.h"
#include "matrix.h"
#include "estructuras.h"
#include "tomografo.h"
#include "csvParser.hpp"

using namespace std;

int main(){
	Matrix m = csv_to_matrix("matrix.csv");
	Coord i, f;
	i.x = i.y = 0;
	f.x = 3; f.y = 8;
	set<Coord> v = discretizarRayo(i, f);

	for(auto c : v) cout << c << endl;

	for(auto r: tcPorConos(m)) cout << r << endl;

	matrix_to_csv(m, "matrixout.csv");
	return 0;

}
