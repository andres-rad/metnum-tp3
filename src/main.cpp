#include <vector>
#include "utils.h"
#include "matrix.h"
#include "estructuras.h"
#include "tomografo.h"

using namespace std;

int main(){
	Matrix m = Matrix(6, 6);
	Coord i, f;
	i.x = i.y = 0;
	f.x = 3; f.y = 8;
	set<Coord> v = discretizarRayo(i, f);

	for(auto c : v) cout << c << endl;

	for(auto r: tcPorConos(m)) cout << r << endl;
    return 0;
}
