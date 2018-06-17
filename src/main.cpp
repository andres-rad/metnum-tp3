#include <vector>
#include "utils.h"
#include "matrix.h"

using namespace std;

int main(){
	Matrix m = Matrix(4, 4);
	Coord i, f;
	i.x = i.y = 0;
	f.x = 3; f.y = 4;
	set<Coord> v = discretizarRayo(i, f);

	for(auto c : v) cout << c.x << ", " << c.y << endl;
    return 0;
}
