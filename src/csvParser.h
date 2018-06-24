#ifndef SRC_CSVPARSER_H_
#define SRC_CSVPARSER_H_

#include <fstream>
#include <sstream>
#include <vector>

#include "./matrix.h"

#define debug(v) cerr << #v << ": " << v << endl

using namespace std;

Matrix csv_to_matrix(string file_path);

void matrix_to_csv(Matrix& matrix, string file_path);

#endif  // SRC_CSVPARSER_H_
