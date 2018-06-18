#include <fstream>
#include <vector>

#include "matrix.h"

using namespace std;

vector<string> split
Matrix csv_to_matrix(string file_path){
    int size;
    int idx = 0;
    fstream fs;

    fs.open(file_path, fstream::in);
    fs >> size;

    Matrix m = Matrix(size, size);

    while(getline(fs, item, ',')){
        m[i/size][i%size] = atoi(item);
    }

    return m;
}
