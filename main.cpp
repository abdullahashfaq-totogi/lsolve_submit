#include "main.h"

int main() {

    //Check current location so that L and b matrices can be reached using a relative path
    print_cwd();

    string L_mtx = L_matrix_path;
    string b_mtx = b_matrix_path;

    // Reading Matrix A
    CSC<matrix_dtype> L_csc;
    Mtx2CSC_vec(L_mtx, L_csc, false, false);

    /*
     * Solving Using the Sample lsolve function
     * */


    // Reading B vector
    COO_ptr<matrix_dtype> lsolve_b;
    Mtx2Matrix(b_mtx, lsolve_b);

    //Solving
    cout << " ############ Solution-Simple ############ \n" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    lsolve(L_csc, lsolve_b.data);
    auto end = std::chrono::high_resolution_clock::now();
    auto lsolve_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    cout << "lsolve-simple completed - Time: " << lsolve_time<< " ms "  << endl;

    /*
     * Solving Using the lsolve where zeros are skipped
     * */

    // Reading B vector
    COO_ptr<matrix_dtype> lsolve_skipz_b;
    Mtx2Matrix(b_mtx, lsolve_skipz_b);

    //Solving
    cout << " ############ Solution-SkipZero ############ \n" << endl;
    start = std::chrono::high_resolution_clock::now();
    lsolve_skipZero(L_csc, lsolve_skipz_b.data); //lsolve where zeros in b are skipped
    end = std::chrono::high_resolution_clock::now();
    auto lsolve_skipz_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    //verifying with standard
    bool areEqual_skipz = isEqual(lsolve_b.data, lsolve_skipz_b.data, lsolve_b.n_row);
    if(!areEqual_skipz) cout<<"lsolve_skipzero answer not correct"<<endl;
    else cout<<"lsolve-skipzero verified - Time: "<< lsolve_skipz_time <<" ms" <<endl;


    /*
      * Solving Using the lsolve where relevant columns are chosen using graph traversal
      * */
    // Reading B vector
    COO_ptr<matrix_dtype> lsolve_graph_b;
    Mtx2Matrix(b_mtx, lsolve_graph_b);

    COO_vec<matrix_dtype> lsolve_graph_bVec;
    Mtx2vec(b_mtx, lsolve_graph_bVec);

    int* visited_cols_arr;
    int visited_col_size;
    //Graph traversal to find reachable nodes
    start = std::chrono::high_resolution_clock::now();
    find_adj_nodes(L_csc, lsolve_graph_bVec, visited_cols_arr, visited_col_size);
    end = std::chrono::high_resolution_clock::now();
    auto graph_trav_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    cout << "Number of Nodes to visit: "<<visited_col_size<<endl;

    //Solving
    cout << " ############ Solution-Graph ############ \n" << endl;
    start = std::chrono::high_resolution_clock::now();
    lsolve_graph(L_csc, lsolve_graph_b.data, visited_cols_arr, visited_col_size); //lsolve where zeros in b are skipped
    end = std::chrono::high_resolution_clock::now();
    auto lsolve_graph_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    //verifying with standard
    bool areEqual_graph = isEqual(lsolve_b.data, lsolve_graph_b.data, lsolve_b.n_row);
    if(!areEqual_graph) cout<<"lsolve_graph answer not correct"<<endl;
    else cout<<"lsolve-graph verified - Time: "<< lsolve_graph_time <<" ms" <<endl;


    /*
     **************************
     * PARALLEL IMPLEMENTATIONS
     **************************
     * */

    /*
     * Parallel - lsolve
     * */

    // Reading B vector
    COO_ptr<matrix_dtype> lsolve_II_b;
    Mtx2Matrix(b_mtx, lsolve_II_b);

    //Solving
    cout << " ############ Solution-Parallel Simple ############ \n" << endl;
    start = std::chrono::high_resolution_clock::now();
    lsolve_II(L_csc, lsolve_II_b.data);
    end = std::chrono::high_resolution_clock::now();
    auto lsolve_II_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    bool areEqual_II = isEqual(lsolve_b.data, lsolve_II_b.data, lsolve_b.n_row);
    if(!areEqual_II) cout<<"lsolve-parallel simple answer not correct"<<endl;
    else cout<<"lsolve-parallel simple verified - Time: "<< lsolve_II_time <<" ms" <<endl;


    /*
     * Parallel - Solving Using the lsolve where zeros are skipped
     * */

    // Reading B vector
    COO_ptr<matrix_dtype> lsolve_skipz_II_b;
    Mtx2Matrix(b_mtx, lsolve_skipz_II_b);

    //Solving
    cout << " ############ Solution-Parallel SkipZero ############ \n" << endl;
    start = std::chrono::high_resolution_clock::now();
    lsolve_skipZero_II(L_csc, lsolve_skipz_II_b.data); //lsolve where zeros in b are skipped
    end = std::chrono::high_resolution_clock::now();
    auto lsolve_skipz_II_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

//    print_array(lsolve_skipz_b.data, lsolve_skipz_b.n_row);
    //verifying with standard
    bool areEqual_skipz_II = isEqual(lsolve_b.data, lsolve_skipz_II_b.data, lsolve_b.n_row);
    if(!areEqual_skipz_II) cout<<"lsolve_skipzero-parallel answer not correct"<<endl;
    else cout<<"lsolve-skipzero-parallel verified - Time: "<< lsolve_skipz_II_time <<" ms" <<endl;


    /*
      * Parallel - lsolve graph
      * */
    // Reading B vector
    COO_ptr<matrix_dtype> lsolve_graph_II_b;
    Mtx2Matrix(b_mtx, lsolve_graph_II_b);

    //Solving
    cout << " ############ Solution-Graph ############ \n" << endl;
    start = std::chrono::high_resolution_clock::now();
    lsolve_graph_II(L_csc, lsolve_graph_II_b.data, visited_cols_arr, visited_col_size); //lsolve where zeros in b are skipped
    end = std::chrono::high_resolution_clock::now();
    auto lsolve_graph_II_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    //verifying with standard
    bool areEqual_graph_II = isEqual(lsolve_b.data, lsolve_graph_b.data, lsolve_b.n_row);
    if(!areEqual_graph_II) cout<<"lsolve_graph-parallel answer not correct"<<endl;
    else cout<<"lsolve-graph-parallel verified - Time: "<< lsolve_graph_II_time <<" ms" <<endl;


    cout << "Program completed ..." << endl<<endl;

    cout<<" .... SUMMARY ...."<<endl;
    cout<<"Function \t Correct \t Time(ms)"<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"sample   \t   NA    \t "<<lsolve_time<<endl;
    cout<<"skipZero \t   "<<areEqual_skipz<<"    \t "<<lsolve_skipz_time<<endl;
    cout<<"Graph    \t   "<<areEqual_graph<<"    \t "<<graph_trav_time<<"+"<<lsolve_graph_time<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"       Parallel Implementations        "<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"sample   \t   "<<areEqual_II<<"    \t "<<lsolve_II_time<<endl;
    cout<<"skipZero \t   "<<areEqual_skipz_II<<"    \t "<<lsolve_skipz_II_time<<endl;
    cout<<"Graph    \t   "<<areEqual_graph_II<<"    \t "<<graph_trav_time<<"+"<<lsolve_graph_II_time<<endl;


    L_csc.eliminate();
    return 0;
}
