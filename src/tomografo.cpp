#include <cassert>
#include <cmath>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>

#include "./csvParser.h"
#include "./estructuras.h"
#include "./tomografo.h"
#include "./utils.h"
#include "./generadores_rayos.h"
#include "./defines.h"



vector<double> calcularTiempos(Matrix &img, vector<Rayo> &rayos) {
    /* Dada una imagen y el conjunto de rayos de la tomografia
     * calcula el tiempo que tarda el recorrido de cada uno de los rayos
     *
     * IN:
     *  img -> imagen a la cual se le hace la tomografia
     *  rayos -> conjunto de rayos generados por la tomografia
     *
     * OUT:
     *  tiempos -> tiempo que tardo el recorrido de cada rayo
     */
    vector<double> tiempos;
    double velocidad;
    for (auto r : rayos) {
        velocidad = 0;
        set<Coord> puntos = coordenadasDeRayo(r);
        for (auto celda : puntos) {
            double elem = img.getElem(celda.x, celda.y);
            velocidad += elem;
        }
        tiempos.push_back(velocidad);
    }
    return tiempos;
}

void draw_rayo_csv(Matrix &img, Rayo &rayo, string filename) {
    /*
     * Escibe en filename en formato csv una matriz del tamanio de img,
     * donde la posicion (i,j) representa cuantos rayos pasaron por el pixel (i,j) de la imagen
     */
    Matrix matriz_img(img.n, img.m);
    set<Coord> puntos = coordenadasDeRayo(rayo);
    for (auto celda : puntos) {
        double temp = matriz_img.getElem(celda.x, celda.y) + 50;
        matriz_img.setElem(celda.x, celda.y, temp);
    }

    matrix_to_csv(matriz_img, filename);
}

void write_info_por_pixel(Matrix &img, vector<Rayo> &rayos, string filename) {
    /*
     * Escibe en filename en formato csv una matriz del tamanio de img,
     * donde la posicion (i,j) representa cuantos rayos pasaron por el pixel (i,j) de la imagen
     */
    Matrix cantidades(img.n, img.m);
    for (auto r : rayos) {
        set<Coord> puntos = coordenadasDeRayo(r);
        for (auto celda : puntos) {
            double temp = cantidades.getElem(celda.x, celda.y) + 1;
            cantidades.setElem(celda.x, celda.y, temp);
        }
    }
    std::fstream fs;
    fs.open(filename, fstream::out);
    for (int i = 0; i < cantidades.n; i++) {
        for (int j = 0; j < cantidades.m; j++) {
            fs << cantidades.getElem(i, j);
            if (j != cantidades.m - 1) {
                fs << ",";
            }
        }
        fs << "\n";
    }
    fs.close();;
}

Matrix generarDiscretizacion(const Matrix &img_original, const vector<Rayo> &rayos, int magnitud_discretizacion) {
    /* La matriz que devuelve tiene una fila por cada rayo tirado
     * y una columna por cada pixel de la imagen discretizada
     * la celda (i,j) valdra 1 si el rayo i pasa por el pixel j
     * (la coordenada (j/tamanio_discretizacion, j%tamanio_discretizacion)).
     * en caso contrario la celda valdra 0
     */
    int tamanio_discretizacion = ceil(img_original.n / (double) magnitud_discretizacion);
    Matrix matriz_sist(rayos.size(), pow(tamanio_discretizacion, 2));

    for (uint i = 0; i < rayos.size(); i++) {
        for (auto coord : coordenadasDeRayo(rayos[i])) {
            Coord coordDiscretizada = pixel_real_a_discretizado(coord, magnitud_discretizacion);
            double elem = matriz_sist.getElem(i, coordDiscretizada.x * tamanio_discretizacion + coordDiscretizada.y);
            matriz_sist.setElem(i, coordDiscretizada.x * tamanio_discretizacion + coordDiscretizada.y, elem + 1);
        }
    }
    return matriz_sist;
}

Coord pixel_real_a_discretizado(Coord real, int magnitud_discretizacion) {
    /* Dada una coordenada en la imagen real, y la magnitud de discretizacion
     * se calcula a que pixel corresponde en la imagen discretizada.
     *
     * IN:
     *  real -> pixel de la imagen real
     *  magnitud_discretizacion -> indica cuantos pixeles de la imagen original
     *      se corresponden con un pixel discretizado
     *      ej: si magnitud_discretizacion = 2 entonces (0,0) (0,1) (1,0) (1,1)
     *          de la imagen original se corresponden con el pixel (0,0) de la
     *          discretizada.
     */
    return Coord(real.x / magnitud_discretizacion,
                 real.y / magnitud_discretizacion);
}

void reescalarPixeles(vector<double>& img, int newMaxVal){
    cout << "Reescalando" << endl;
    double minElem = *min_element(img.begin(), img.end());
    double maxElem = *max_element(img.begin(), img.end());
    double oldRange = maxElem - minElem;

    if(oldRange == 0) return;

    for(size_t i = 0; i < img.size(); i++){
        img[i] = newMaxVal * (img[i] - minElem) / oldRange;
    }

}

Matrix obtenerResultado(Matrix &img_original, int magnitud_discretizacion, int width_rayos, int step_rayos, int step_other_side,
                        double varianza_ruido, int n_rayos, int pixel_size) {

    cout << "Generando rayos" << endl;
    vector<Rayo> rayos = tcRandom(img_original, n_rayos);//tcRandom(img_original, n_rayos);
    cout << "RAYOS:" << rayos.size() << endl;

    cout << "Calculando tiempos" << endl;
    vector<double> tiempos = calcularTiempos(img_original, rayos);
    cout << "TIEMPOS:" << tiempos.size() << endl;

    cout << "Agregando ruido" << endl;
    agregarRuido(tiempos, 1, 100, pixel_size*img_original.dimensions().first, varianza_ruido);

    cout << "Generando discretizacion" << endl;
    Matrix matriz_sistema = generarDiscretizacion(img_original, rayos, magnitud_discretizacion);
    cout << "MATRIZ: " << matriz_sistema.n << ", " << matriz_sistema.m << endl;

    cout << "Cuadrados Minimos" << endl;
    vector<double> solucion_cm = cuadradosMinimos(matriz_sistema, tiempos);
    cout << "Fin CM" << endl;

    reescalarPixeles(solucion_cm, pixel_size);

    int tamanio_discretizacion = ceil(img_original.n / (double) magnitud_discretizacion);

    return vec_to_matrix(solucion_cm, tamanio_discretizacion, tamanio_discretizacion);
}
