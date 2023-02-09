//
// Created by abdul on 08/02/2023.
//

#include "lsolver.h"


/* Description:
 * lower triangle solver using a simple implementation
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * x: This is b vector initially. This is modified in the function iteratively and it becomes the output
 *
 * Output:
 * x: Solution vector
 * */
int lsolve(CSC<matrix_dtype> L_csc, matrix_dtype *& x){
    int p, j;
    if (!L_csc.Lp || !L_csc.Li || !x) return (0); /* check inputs */
    for (j = 0; j < L_csc.n_col; j++)
    {
        x[j] /= L_csc.Lx[L_csc.Lp[j]];
        for (p = L_csc.Lp[j] + 1; p < L_csc.Lp[j + 1]; p++)
        {
            x[L_csc.Li[p]] -= L_csc.Lx[p] * x[j];
        }
    }
    return (1);
}


/* Description:
 * lower triangle solver using a simple implementation but columns where corresponding in b is zero are skipped.
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * x: This is b vector initially. This is modified in the function iteratively and it becomes the output
 *
 * Output:
 * x: Solution vector
 * */
int lsolve_skipZero(CSC<matrix_dtype> L_csc, matrix_dtype *& x) {
    int p, j;
    if (!L_csc.Lp || !L_csc.Li || !x) return (0); /* check inputs */
    for (j = 0; j < L_csc.n_col; j++)
    {
        if (x[j] == 0) continue;

        x[j] /= L_csc.Lx[L_csc.Lp[j]];
        for (p = L_csc.Lp[j] + 1; p < L_csc.Lp[j + 1]; p++)
        {
            x[L_csc.Li[p]] -= L_csc.Lx[p] * x[j];
        }
    }
    return (1);
}



/* Description:
 * Find columns which are reachable from nodes where b is nonzero
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * b_matrix: b matix in COO format
 *
 * Output:
 * visited_cols_arr: array of column indices that need to be visited
 * visited_col_size: number of nodes to be visited
 * */
void find_adj_nodes(CSC<matrix_dtype> L_csc, COO_vec<matrix_dtype> b_matrix, int*& visited_cols_arr, int& visited_col_size){
    set<int> visited_cols;
    for(int i=0; i< b_matrix.data.size(); i++){
        if(get<2>(b_matrix.data[i]) != 0) find_adj(L_csc.Lp, L_csc.Li, L_csc.Lx, get<1>(b_matrix.data[i]), visited_cols);
    }

    //Assigning values to outputs
    set2array(visited_cols, visited_cols_arr);
    visited_col_size = visited_cols.size();
}


/* Description:
 * lower triangle solver where essential columns are provided and it only uses those to compute solution.
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * visited_cols: array of columns to visit
 * num_nodes: number of columns to visit
 * x: This is b vector initially. This is modified in the function iteratively and it becomes the output
 *
 * Output:
 * x: Solution vector
 * */
int lsolve_graph(CSC<matrix_dtype> L_csc, matrix_dtype *& x, int* visited_cols, int num_nodes){

    cout<<"Columns to visit = "<<num_nodes<<endl;
    //Now only iterate over these columns and compute
    int p;
    for (int j = 0 ; j < num_nodes ; j++ )
    {

        if (x[visited_cols[j]] == 0) continue;

        x[visited_cols[j]] /= L_csc.Lx[L_csc.Lp[visited_cols[j]]];
        for (p = L_csc.Lp[visited_cols[j]] + 1; p < L_csc.Lp[visited_cols[j] + 1]; p++)
        {
            x[L_csc.Li[p]] -= L_csc.Lx[p] * x[visited_cols[j]];
        }
    }

    return (1);
}


/* Description:
 * Parallel implementation of lower triangle solver using a simple implementation
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * x: This is b vector initially. This is modified in the function iteratively and it becomes the output
 *
 * Output:
 * x: Solution vector
 * */
int lsolve_II(CSC<matrix_dtype> L_csc, matrix_dtype *& x){
    int p, j;
    if (!L_csc.Lp || !L_csc.Li || !x) return (0); /* check inputs */
    for (j = 0; j < L_csc.n_col; j++)
    {
        x[j] /= L_csc.Lx[L_csc.Lp[j]];

#pragma omp parallel //shared(x) num_threads(NUM_THREADS)
        {
#pragma omp for
            for (p = L_csc.Lp[j] + 1; p < L_csc.Lp[j + 1]; p++) {
                x[L_csc.Li[p]] -= L_csc.Lx[p] * x[j];
            }
        }
    }
    return (1);
}


/* Description:
 * Parallel implementation of lower triangle solver using a simple implementation but columns where corresponding in b is zero are skipped.
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * x: This is b vector initially. This is modified in the function iteratively and it becomes the output
 *
 * Output:
 * x: Solution vector
 * */
int lsolve_skipZero_II(CSC<matrix_dtype> L_csc, matrix_dtype *& x) {
    int p, j;
    if (!L_csc.Lp || !L_csc.Li || !x) return (0); /* check inputs */
    for (j = 0; j < L_csc.n_col; j++)
    {
        if (x[j] == 0) continue;

        x[j] /= L_csc.Lx[L_csc.Lp[j]];
#pragma omp parallel //shared(x) num_threads(NUM_THREADS)
        {
#pragma omp for
            for (p = L_csc.Lp[j] + 1; p < L_csc.Lp[j + 1]; p++)
            {
                x[L_csc.Li[p]] -= L_csc.Lx[p] * x[j];
            }
        }
    }
    return (1);
}


/* Description:
 * lower triangle solver where essential columns are provided and it only uses those to compute solution.
 *
 * Inputs:
 * L_csc: matrix in CSC format
 * visited_cols: array of columns to visit
 * num_nodes: number of columns to visit
 * x: This is b vector initially. This is modified in the function iteratively and it becomes the output
 *
 * Output:
 * x: Solution vector
 * */
int lsolve_graph_II(CSC<matrix_dtype> L_csc, matrix_dtype *& x, int* visited_cols, int num_nodes){

    cout<<"Columns to visit = "<<num_nodes<<endl;
    //Now only iterate over these columns and compute
    int p;
    for (int j = 0 ; j < num_nodes ; j++ )
    {

        if (x[visited_cols[j]] == 0) continue;

        x[visited_cols[j]] /= L_csc.Lx[L_csc.Lp[visited_cols[j]]];
#pragma omp parallel //shared(x) num_threads(NUM_THREADS)
        {
#pragma omp for
            for (p = L_csc.Lp[visited_cols[j]] + 1; p < L_csc.Lp[visited_cols[j] + 1]; p++) {
                x[L_csc.Li[p]] -= L_csc.Lx[p] * x[visited_cols[j]];
            }
        }
    }

    return (1);
}