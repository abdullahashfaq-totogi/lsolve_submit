//
// Created by abdul on 09/02/2023.
//

#ifndef LSOLVE_SUBMIT_IO_FUNC_H
#define LSOLVE_SUBMIT_IO_FUNC_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <tuple>
#include <algorithm> // sort
#include <stdlib.h>
using namespace std;

#include "../config.h"
#include "storage.h"
#include "helpers.h"

typedef tuple<int, int, matrix_dtype> coo; // row, col, value

//Display functions
void print_matrix(COO_ptr<matrix_dtype>& matrix);
void print_array(matrix_dtype* x, int n);
void print_array_int(int* x, int n);


//Read functions
void Mtx2vec(string mtx_filename, COO_vec<matrix_dtype>& matrix);

void Mtx2Matrix(string mtx_filename, COO_ptr<matrix_dtype>& matrix);

void Mtx2Matrix_vec(string mtx_filename, COO_vec<matrix_dtype>& matrix);

void Mtx2CSC_vec(string mtx_filename, CSC<matrix_dtype>& L_csc, bool displayM, bool displayCSC);

void Matrix2CSC_vec(COO_vec<matrix_dtype>& matrix, CSC<matrix_dtype>& L_csc);

#endif //LSOLVE_SUBMIT_IO_FUNC_H
