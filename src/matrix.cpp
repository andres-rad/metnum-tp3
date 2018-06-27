#include <cassert>
#include <unordered_map>
#include <vector>
#include <utility>
#include <cmath>

#include "./matrix.h"
#include "./defines.h"
double mu =1e-8;

//Que acceden a estructura interna

Matrix::Matrix(int n, int m) : n(n), m(m), matrix(unordered_map<int, unordered_map<int, double> >()) {}

Matrix::Matrix(int n) : n(n), m(n), matrix(unordered_map<int, unordered_map<int, double> >()) {}

Matrix::Matrix(vector<vector<double> > &mat) {
    n = mat.size();
    m = mat[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            setElem(i, j, mat[i][j]);
        }
    }
}

double Matrix::getElem(int i, int j) const {
    if (matrix.find(i) != matrix.end()) {
        if (matrix.find(i)->second.find(j) != matrix.find(i)->second.end()) {
            return matrix.find(i)->second.find(j)->second;
        }
    }
    return 0;
}

void Matrix::setElem(int i, int j, double val) {
    if (abs(val) > mu) {
        this->matrix[i][j] = val;
    } else {
        if (matrix.find(i) != matrix.end() and matrix.find(i)->second.find(j) != matrix.find(i)->second.end()) {
            matrix[i].erase(j);
        }
    }
}

pair<int, int> Matrix::dimensions() const {
    return make_pair(this->n, this->m);
}

void Matrix::swap_rows(int r1, int r2) {
    unordered_map<int, double> row1 = matrix[r1];
    unordered_map<int, double> row2 = matrix[r2];
    swap(row1, row2);
}

Matrix Matrix::transpose() {
    Matrix transposed(this->m, this->n);
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < this->m; j++) {
            transposed.setElem(j, i, this->getElem(i, j));
        }
    }
    return transposed;
}

int Matrix::findPivot(int row_init, int col) {
    //busca fila con el mayor valor,
    // como es el paso numero col supongo que el primer valor de la fila esta en un j >= col
    int pivot = row_init;
    double pivot_value = getElem(pivot, col);
    for (int i = row_init; i < this->n; i++) {
        if (getElem(i, col) > pivot_value) {
            pivot_value = getElem(i, col);
            pivot = i;
        }
    }
    return pivot;
}

void Matrix::row_operation(int indice_pivot, int fila, vector<double> &b) {//realiza la resta de filas
    double row_elem = getElem(fila, indice_pivot);
    if (row_elem != 0) {
        double e = row_elem / getElem(indice_pivot, indice_pivot);
        auto pivot_row = matrix.find(indice_pivot);
        for (unordered_map<int, double>::iterator it = pivot_row->second.begin(); it != pivot_row->second.end(); it++) {
            double res = getElem(fila, it->first) - it->second * e;
            setElem(fila, it->first, res);
        }
        b[fila] = b[fila] - b[indice_pivot] * e; //opero sobre b tambien
    }
}

//NO
void Matrix::g_elimination(vector<double> &b) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            row_operation(i, j, b);
        }
    }
}

//SI
Matrix operator*(const Matrix &m1, const Matrix &m2) {
    int n_prod = m1.dimensions().first;
    int m_prod = m2.dimensions().second;
    int mid_dim = m1.dimensions().second;
    assert (mid_dim == m2.dimensions().first);

    Matrix prod(n_prod, m_prod);
    //Itero filas del map
    for (unordered_map<int, unordered_map<int, double> >::const_iterator it = m1.matrix.begin(); it != m1.matrix.end(); it++) {
        for (int j = 0; j < m_prod; j++) {
            double sum = 0;
            //Itero las columnas del map
            for (unordered_map<int, double>::const_iterator it_row = it->second.begin(); it_row != it->second.end(); it_row++) {
                //algo
                sum += it_row->second * m2.getElem(it_row->first, j);
            }
            prod.setElem(it->first, j, sum);
        }
    }
    return prod;
}


Matrix Matrix::prodTranspuesto(Matrix& at){
    //Precondicion: at es la transpuesta de this
    int m = this->dimensions().second;
    Matrix ata(m, m);
    for (auto it = this->matrix.begin(); it != this->matrix.end(); it++){
        for(int j = 0; j <= it->first; j++){
            double sum = 0;
            for(auto it_row = it->second.begin(); it_row != it->second.end(); it_row++){
                sum += it_row->second * at.getElem(it_row->first,j);
            }
            ata.setElem(it->first, j, sum);
            ata.setElem(j, it->first, sum);
        }
    }
    return ata;
}
//SI
Matrix operator+(const Matrix &a, const Matrix &b) {
    int n1 = a.dimensions().first;
    int m1 = a.dimensions().second;
    int n2 = b.dimensions().first;
    int m2 = b.dimensions().second;
    assert(n1 == n2);
    assert(m1 == m2);

    Matrix sum(n1, m1);

    for (int i = 0; i < m1; ++i) {
        for (int j = 0; j < n1; ++j) {
            sum.setElem(i, j, a.getElem(i, j) + b.getElem(i, j));
        }
    }
    return sum;
}

//NO
Matrix operator-(const Matrix &a, const Matrix &b) {
    int n1 = a.dimensions().first;
    int m1 = a.dimensions().second;

    int n2 = b.dimensions().first;
    int m2 = b.dimensions().second;

    assert(n1 == n2);
    assert(m1 == m2);

    Matrix sub(n1, m1);

    for (int i = 0; i < m1; ++i) {
        for (int j = 0; j < n1; ++j) {
            sub.setElem(i, j, a.getElem(i, j) - b.getElem(i, j));
        }
    }
    return sub;
}

//SI
Matrix operator*(double e, const Matrix &m) {
    int n = m.dimensions().first;
    int k = m.dimensions().second;
    Matrix prod(n, k);

    for (unordered_map<int, unordered_map<int, double> >::const_iterator it = m.matrix.begin();
         it != m.matrix.end();
         it++) {
        for (unordered_map<int, double>::const_iterator it_row = it->second.begin();
             it_row != it->second.end(); it_row++) {
            prod.setElem(it->first, it_row->first, it_row->second * e);
        }
    }
    return prod;
}

vector<double> operator*(Matrix &matrix, vector<double> &v) {
    vector<double> prod(matrix.n, 0);
    for (int i = 0; i < matrix.n; ++i) {
        double aux = 0;
        for (int j = 0; j < matrix.m; ++j) {
            aux += matrix.getElem(i, j) * v[j];
        }
        prod[i] = aux;
    }
    return prod;
}
//No
ostream &operator<<(ostream &os, const Matrix &matrix) {
    int n = matrix.dimensions().first;
    int m = matrix.dimensions().second;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            os << round(matrix.getElem(i, j));
            if (j != m-1) os << ",";
        }
        os << endl;
    }
    return os;
}

//NO
Matrix identity(int n) {
    Matrix id(n);
    for (int i = 0; i < n; ++i) {
        id.setElem(i, i, 1);
    }
    return id;
}


Matrix vec_to_matrix(vector<double> elems, int n, int m) {
    Matrix matriz(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matriz.setElem(i, j, elems[m * i + j]);
        }
    }
    return matriz;
}
