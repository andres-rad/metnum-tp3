#include <cassert>
#include <cmath>
#include <fstream>
#include <utility>
#include <vector>

#include "./csvParser.h"
#include "./estructuras.h"
#include "./tomografo.h"
#include "./utils.h"

#define debug(v) cerr << #v << ": " << v << endl
#define debugVec(v) for(auto i:v) cerr << i << ", "; cerr<<endl

vector<Rayo> tcPorConos(Matrix &matrix, int width, int step) {
    /* Funcion que simula el conjunto de rayos que genera una tomografia.
     * El emisor se mueve por los lados de la imagen y tira rayos en
     * forma de cono a cada paso.
     *
     * IN:
     *  matrix -> imagen a la cual se le hace la tomografia
     *  width  -> ancho del cono de rayos generado en cada paso
     *     donde ancho = distancia del centro del arco del cono hasta una punta
     *  step   -> cuanto avanza el emisor de rayos en cada paso
     *
     * OUT:
     *  res -> rayos emitidos sobre la imagen dados los
     *         parametros de entrada
     */
    int n = matrix.dimensions().first - 1;
    int m = matrix.dimensions().second - 1;
    vector<Rayo> tc;

    assert(n == m);
    assert(step > 0 && step <= n);
    assert(width >= 0);

    for (int base = 0; base < n; base += step) {
        for (int endX = max(0, base - width); endX <= min(m, base + width); endX++) {
            tc.push_back(Rayo(Coord(base, 0), Coord(endX, m)));
            tc.push_back(Rayo(Coord(base, m), Coord(endX, 0)));
        }
    }

    for (int base = 0; base < m; base += step) {
        for (int endY = max(0, base - width); endY <= min(n, base + width); endY++) {
            tc.push_back(Rayo(Coord(0, base), Coord(n, endY)));
            tc.push_back(Rayo(Coord(n, base), Coord(0, endY)));
        }
    }

    return tc;
}

vector<Rayo> tCicrular(Matrix &matrix, bool completa) {
    int n = matrix.dimensions().first - 1;
    int m = matrix.dimensions().second - 1;
    int fin = n - 1;
    assert(n == m);
    vector<Rayo> tcir;
    tcir.push_back(Rayo(Coord(0, 0), Coord(fin, fin)));
    for (int i = 1; i < n; i++) {
        tcir.push_back(Rayo(Coord(0, i), Coord(fin, fin - i)));
        tcir.push_back(Rayo(Coord(i, 0), Coord(fin - i, fin)));
    }
    if (completa) {
        tcir.push_back(Rayo(Coord(fin, fin), Coord(0, 0)));
        for (int i = 1; i < n; i++) {
            tcir.push_back(Rayo(Coord(fin, fin - i), Coord(0, i)));
            tcir.push_back(Rayo(Coord(fin - i, fin), Coord(i, 0)));
        }
    }
    return tcir;
}

vector<Rayo> tRecta(Matrix &matrix, bool completa) {
    int n = matrix.dimensions().first - 1;
    int m = matrix.dimensions().second - 1;
    int fin = n - 1;
    assert(n == m);
    vector<Rayo> trec;
    for (int i = 0; i < n; i++) {
        trec.push_back(Rayo(Coord(0, i), Coord(fin, i)));
        trec.push_back(Rayo(Coord(i, 0), Coord(i, fin)));
    }
    if (completa) {
        for (int i = 0; i < n; i++) {
            trec.push_back(Rayo(Coord(fin, i), Coord(0, i)));
            trec.push_back(Rayo(Coord(i, fin), Coord(i, 0)));
        }
    }
    return trec;
}

set<Coord> coordenadasDeRayo(Rayo r) {
    // Sacado de http://eugen.dedu.free.fr/projects/bresenham/
    set<Coord> res;
    Coord inicio = r.inicio;
    Coord fin = r.fin;

    int deltaX = fin.x - inicio.x;
    int deltaY = fin.y - inicio.y;
    int curY = inicio.y;
    int curX = inicio.x;
    res.insert(inicio);

    int stepX = 1;
    int stepY = 1;
    int ultError, error;

    if (deltaX < 0) {
        stepX = -1;
        deltaX = -deltaX;
    }
    if (deltaY < 0) {
        stepY = -1;
        deltaY = -deltaY;
    }

    int dobleDX = 2 * deltaX;
    int dobleDY = 2 * deltaY;

    if (dobleDX >= dobleDY) {
        // Pendiente menor a identidad
        ultError = error = deltaX;

        for (int i = 0; i < deltaX; i++) {
            curX += stepX;
            error += dobleDY;

            if (error > dobleDX) {
                curY += stepY;
                error -= dobleDX;
            }

            if (error + ultError < dobleDX) {
                res.insert(Coord(curX, curY - stepY));
            } else if (error + ultError > dobleDX) {
                res.insert(Coord(curX - stepX, curY));
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    } else {  // Pendiente mayor a identidad
        ultError = error = deltaY;
        for (int i = 0; i < deltaY; i++) {
            curY += stepY;
            error += dobleDX;

            if (error > dobleDY) {
                curX += stepX;
                error -= dobleDY;
                if (error + ultError < dobleDY) {
                    res.insert(Coord(curX - stepX, curY));
                } else if (error + ultError > dobleDY) {
                    res.insert(Coord(curX, curY - stepY));
                }
            }
            res.insert(Coord(curX, curY));
            ultError = error;
        }
    }
    return res;
}

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
            velocidad += 1 / img.getElem(celda.x, celda.y);
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
            matriz_sist.setElem(i, coordDiscretizada.x * tamanio_discretizacion + coordDiscretizada.y, 1);
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


Matrix obtenerResultado(Matrix &img_original, int magnitud_discretizacion, int width_rayos, int step_rayos,
                        double varianza_ruido) {
    cout << "Generando rayos" << endl;
    vector<Rayo> rayos = tcPorConos(img_original, width_rayos, step_rayos);

    // Dibuja cada rayo en un archivo distinto
    // for (uint i =0; i < rayos.size(); i++) {
    //     draw_rayo_csv(img_original, rayos[i], "rayo-"+to_string(i));
    // }


    cout << "Calculando tiempos" << endl;
    vector<double> tiempos = calcularTiempos(img_original, rayos);




    // cout << "Escribiendo info pixeles" << endl;
    // write_info_por_pixel(img_original, rayos, "cantidades_rayos.csv");
    //agregarRuido(tiempos, 1, 1, 1000, varianza_ruido);

    cout << "Generando discretizacion" << endl;
    Matrix matriz_sistema = generarDiscretizacion(img_original, rayos, magnitud_discretizacion);


    cout << "Cuadrados Minimos" << endl;
    vector<double> solucion_cm = cuadradosMinimos(matriz_sistema, tiempos);
    cout << "Fin CM" << endl;


    int tamanio_discretizacion = ceil(img_original.n / (double) magnitud_discretizacion);
    for (uint i = 0; i < solucion_cm.size(); i++) {
        if (solucion_cm[i] != 0) solucion_cm[i] = 1 / solucion_cm[i];
    }
    return vec_to_matrix(solucion_cm, tamanio_discretizacion, tamanio_discretizacion);
}
