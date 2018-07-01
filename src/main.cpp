#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <chrono>

#include "./csvParser.h"
#include "./estructuras.h"
#include "./matrix.h"
#include "./tomografo.h"
#include "./utils.h"
#include "./defines.h"

using namespace std;

string input_path = "";
string output_path = "";
string output_time_path = "time_mesurements.txt";
extern bool mesure_time;
double varianza_ruido = 0;

int magnitud_discretizacion = 1;
int n_rayos = 10000;
int step = 1;
int step_other_side = 3;
int width = 50;
int pixel_size = PIXEL_SIZE_1_BYTE;
int tipo_rayo = TIPO_RAYO_RAND;

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
        } else if (strcmp(argv[i], "-step-other") == 0) {
            step_other_side = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "--time") == 0) {
            mesure_time = true;
        } else if (strcmp(argv[i], "-tipo-rayo") == 0) {
            tipo_rayo = stoi(argv[i+1]);
        } else if (strcmp(argv[i], "-out-time") == 0) {
            output_time_path = argv[i+1];
        }
    }

    cout << "Ejecutando con los parametros:" << endl;
    cout << "input_path: " << input_path << endl;
    cout << "output_path: " << output_path << endl;
    cout << "varianza_ruido: " << varianza_ruido << endl;
    cout << "magnitud_discretizacion: " << magnitud_discretizacion << endl;
    cout << "pixel_size: " << pixel_size << endl;
    cout << "tipo_rayo: " << (tipo_rayo == TIPO_RAYO_RAND ? "rand" : "conos") << endl;
    if (tipo_rayo == TIPO_RAYO_RAND) {
        cout << "       cantrayos: " << n_rayos << endl;
    } else {
        cout << "       width: " << width << endl;
        cout << "       step: " << step << endl;
        cout << "       step_other_side: " << step_other_side << endl;
    }
    cout << "Midiendo Tiempo : " << mesure_time << endl;
    cout << "output_time_path: " << output_time_path << endl;

    assert(input_path != "" && output_path != "");
    Matrix img = csv_to_matrix(input_path);
    auto start = std::chrono::high_resolution_clock::now();

    Matrix reconstruccion = obtenerResultado(img, magnitud_discretizacion, tipo_rayo, width,
                                             step, step_other_side, varianza_ruido, n_rayos,
                                             pixel_size);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<chrono::duration<double>>(end - start);
    matrix_to_csv(reconstruccion, output_path);
    if(mesure_time){
        cout << "Guardando Tiempo" << endl;
        std::fstream time_output;
        time_output.open(output_time_path, fstream::app);
        time_output << magnitud_discretizacion << ", "<< elapsed.count() << endl;
    }
    // test_eliminacion_gaussiana();
    return 0;
}
