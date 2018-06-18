#include <vector>
#include <string>
#include <cstring>

#include "utils.h"
#include "matrix.h"
#include "estructuras.h"
#include "tomografo.h"
#include "csvParser.hpp"

using namespace std;

void test(){

	Matrix m = Matrix(6,6);
	Coord i, f;
	i.x = i.y = 0;
	f.x = 3; f.y = 5;
	m[1][1] = 3;
	m[2][2] = 3;
	m[3][5] = 1;
	cerr << "tcPorConos" << endl;
	vector<Rayo> rayos = tcPorConos(m);

	cout << "calcularTiempos" << endl;
	for(auto r: calcularTiempos(m, rayos)) cout << r << " ";
	cout << endl;
	//matrix_to_csv(m, "matrixout.csv");

}

string input_path;
string output_path;
int varianza_ruido;
int magnitud_discretizacion;

int main(int argc, char* argv[]){

	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-i") == 0){
			input_path = argv[i+1];
		} else if(strcmp(argv[i], "-o") == 0){
			output_path = argv[i+1];
		} else if(strcmp(argv[i], "-v") == 0){
			varianza_ruido = stoi(argv[i+1]);
		} else if(strcmp(argv[i], "-d") == 0){
			magnitud_discretizacion = stoi(argv[i+1]);
		}
	}

	test();
	return 0;
}
