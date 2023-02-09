#include "helpers.h"


/* Description:
 * To copy a set into an array. Because looping over an array is faster
 *
 * Inputs:
 * set_of_smth: set of use as source
 *
 * Outputs:
 * arr: destination array to which data will be copied
 * */
void set2array(set<int> set_of_smth, int* &arr){

    arr = new int[set_of_smth.size()]();
    int arr_i = 0;
    for (auto j = set_of_smth.begin() ; j != set_of_smth.end() ; j++ ){
        arr[arr_i] = *j;
        arr_i++;
    }
}


/* Description:
 * To print current working directory. Will help to reach the .mtx files
 * */
void print_cwd(){
    char tmp[256];
    getcwd(tmp, 256);
    cout << "Current working directory: " << tmp << endl;
}



/* Description:
 * Comparison function to sort the vector elements by second element of tuples
 *
 * Inputs:
 * a,b: tuples to compare
 *
 * Outputs:
 * bool: True if 2nd element of b is greater than 2nd element of a
 * */

bool sortbysec(const tuple<int, int, double>& a, const tuple<int, int, double>& b)
{
    return (get<1>(a) < get<1>(b));
}


/* Description:
 * Find nodes reachable from current node in a directed graph. Graph can be in form of CSC representation of matrix.
 *
 * Inputs:
 * Lp: column pointer of CSC
 * Li: row indices array of CSC
 * Lx: data array of CSC
 * col: column the reach of which we need to find
 *
 * Outputs:
 * visited_cols: The columns reachable from col
 * */
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

