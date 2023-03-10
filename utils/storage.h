//
// Created by abdul on 09/02/2023.
//

#ifndef LSOLVE_SUBMIT_STORAGE_H
#define LSOLVE_SUBMIT_STORAGE_H

#include <iostream>
#include <vector>
#include <tuple>
using namespace std;


/*
 * Storage for matrix in CSC format
 * */
template <typename Lx_type>
struct CSC{
    int* Lp; // column indices
    int* Li; // row indices
    Lx_type* Lx; // array containing data
    int n_row, n_col, n_nonzero, Lp_size; // number of row, columns, nonZeros and Lp_size=column array size

    CSC(){
        Lp,Li,Lx=NULL;
        n_row,n_col, n_nonzero, Lp_size=0;
    }

    //Assign arrays
    void set(int*& Lp_in, int*& Li_in, Lx_type*& Lx_in, int n_row_in, int n_col_in, int n_nonzero_in, int Lp_size_in){
        Lp = Lp_in;
        Li = Li_in;
        Lx = Lx_in;
        n_row = n_row_in;
        n_col = n_col_in;
        n_nonzero =n_nonzero_in;
        Lp_size = Lp_size_in;
    }

    //Delete arrays
    void eliminate(){
        delete[] Li;
        delete[] Lx;
        delete[] Lp;
        n_row, n_col, n_nonzero, Lp_size=0;
    }

    //Print arrays
    void print(){
        cout << endl << " --- Printing Matrix in CSC format --- " << endl;

        // Printing Lx
        cout << "Lx:" << endl;
        for (int i = 0; i < n_nonzero; i++) {
            cout << Lx[i] << " ";
        }
        cout << endl << "Li:" << endl;

        // Printing Li
        for (int i = 0; i < n_nonzero; i++) {
            cout << Li[i] << " ";
        }
        cout << endl << "Lp:" << endl;

        // Printing Lp
        for (int i = 0; i < Lp_size; i++) {
            cout << Lp[i] << " ";
        }
        cout << endl;
    }

};

/*
 * Storage for matrix in COO format and data is in a vector
 * */
template <typename dtype>
struct COO_vec{
    int n_row, n_col, n_nonzero;
    vector<tuple<int,int,dtype>> data;
    COO_vec(){
        n_row,n_col, n_nonzero=0;
    }

    void print(){
        cout<<"Print Matrix in COO format"<<endl;
        for(auto i=data.begin(); i!=data.end(); i++){
            cout<<get<0>(*i) <<" " <<get<1>(*i)<<" "<<get<2>(*i)<<endl;
        }
    }

};

/*
 * Storage for matrix in COO format and data is in an array
 * */
template <typename dtype>
struct COO_ptr{
    int n_row, n_col, n_nonzero;
    dtype* data;
    COO_ptr(){
        n_row,n_col, n_nonzero=0;
    }

};

#endif //LSOLVE_SUBMIT_STORAGE_H
