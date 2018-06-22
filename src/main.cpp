#include <vector>
#include <string>
#include <cstring>
#include <cassert>

#include "./csvParser.hpp"
#include "./estructuras.h"
#include "./matrix.h"
#include "./tomografo.h"
#include "./utils.h"

using namespace std;

#define debugVec(v) for(auto i:v) cerr << i << ", "; cerr<<endl

void test() {
    Matrix m = Matrix(6, 6);
    Coord i, f;
    i.x = i.y = 0;
    f.x = 3; f.y = 5;
    m[1][1] = 3;
    m[2][2] = 3;
    m[3][5] = 1;
    vector<Rayo> rayos = tcPorConos(m);

    for (auto r : calcularTiempos(m, rayos)) cout << r << " ";
    cout << endl;
    // matrix_to_csv(m, "matrixout.csv");
}

string input_path = "";
string output_path = "";
double varianza_ruido;
int magnitud_discretizacion;

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            input_path = argv[i+1];
        } else if (strcmp(argv[i], "-o") == 0) {
            output_path = argv[i+1];
        } else if (strcmp(argv[i], "-v") == 0) {
            varianza_ruido = stod(argv[i+1]);
        } else if (strcmp(argv[i], "-d") == 0) {
            magnitud_discretizacion = stoi(argv[i+1]);
        }
    }

    assert(input_path != "" && output_path != "");
    Matrix img = csv_to_matrix(input_path);
    Matrix reconstruccion = obtenerResultado(img, magnitud_discretizacion, 4, 1, varianza_ruido);
    matrix_to_csv(reconstruccion, output_path);
    return 0;
}
