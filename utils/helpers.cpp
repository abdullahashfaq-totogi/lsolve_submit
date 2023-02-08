#include "helpers.h"


/*
 * Function: To copy a set into an array. Because looping over an array is faster
 * */
void set2array(set<int> set_of_smth, int* &arr){

    arr = new int[set_of_smth.size()]();
    int arr_i = 0;
    for (auto j = set_of_smth.begin() ; j != set_of_smth.end() ; j++ ){
        arr[arr_i] = *j;
        arr_i++;
    }
}

void print_cwd(){
    char tmp[256];
    getcwd(tmp, 256);
    cout << "Current working directory: " << tmp << endl;
}




// Comparison function to sort the vector elements
// by second element of tuples
bool sortbysec(const tuple<int, int, double>& a, const tuple<int, int, double>& b)
{
    return (get<1>(a) < get<1>(b));
}


void find_adj(int* Lp, int* Li, double* Lx, int col, set<int> &visited_cols){
    //If column already visited, return
    bool exists = visited_cols.find(col) != visited_cols.end();
    if(exists) return;

    visited_cols.insert(col);
    for (int p = Lp[col]; p < Lp[col + 1]; p++){
        if(col == Li[p]) continue; // skip for values on diagonal
        if(Lx[p] != 0) find_adj(Lp, Li, Lx, Li[p], visited_cols);
    }

}


void copyArray(double* src, double*& target, int size){
    target = new double[size]();
    for(int i=0; i<size; i++){
        target[i] = src[i];
    }
}
