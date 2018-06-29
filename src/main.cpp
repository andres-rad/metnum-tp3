#include <vector>
#include <string>
#include <cstring>
#include <cassert>

#include "./csvParser.h"
#include "./estructuras.h"
#include "./matrix.h"
#include "./tomografo.h"
#include "./utils.h"
#include "./defines.h"

using namespace std;

string input_path = "";
string output_path = "";
double varianza_ruido = 0;

int magnitud_discretizacion = 1;
int n_rayos = 10000;
int step = 1;
int width = 50;
int pixel_size;

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
        } else if (strcmp(argv[i], "-rayos") == 0){
            n_rayos = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-width") == 0){
            width = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-step") == 0){
            step = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-pixel") == 0){
             if(stoi(argv[i+1]) == 1){
                 pixel_size = PIXEL_SIZE_1_BYTE;
             } else {
                 pixel_size = PIXEL_SIZE_2_BYTES;
             }
        }
    }

    assert(input_path != "" && output_path != "");
    Matrix img = csv_to_matrix(input_path);
    Matrix reconstruccion = obtenerResultado(img, magnitud_discretizacion, width,
                                             step, varianza_ruido, n_rayos, 
                                             pixel_size);
    matrix_to_csv(reconstruccion, output_path);

    // test_eliminacion_gaussiana();
    return 0;
}
