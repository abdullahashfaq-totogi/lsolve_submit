//
// Created by abdul on 08/02/2023.
//

#ifndef LSOLVE_SUBMIT_LSOLVER_H
#define LSOLVE_SUBMIT_LSOLVER_H

#include <set>
using namespace std;

#include "storage.h"
#include "../config.h"
#include "helpers.h"

int lsolve(CSC<matrix_dtype> L_csc, matrix_dtype *& x);
int lsolve_skipZero(CSC<matrix_dtype> L_csc, matrix_dtype *& x);

void find_adj_nodes(CSC<matrix_dtype> L_csc, COO_vec<matrix_dtype> b_matrix, int*& visited_cols_arr, int& visited_col_size);
int lsolve_graph(CSC<matrix_dtype> L_csc, matrix_dtype *& x, int* visited_cols, int num_nodes);


//Parallel implementations
int lsolve_II(CSC<matrix_dtype> L_csc, matrix_dtype *& x);
int lsolve_skipZero_II(CSC<matrix_dtype> L_csc, matrix_dtype *& x);
int lsolve_graph_II(CSC<matrix_dtype> L_csc, matrix_dtype *& x, int* visited_cols, int num_nodes);

#endif //LSOLVE_SUBMIT_LSOLVER_H
