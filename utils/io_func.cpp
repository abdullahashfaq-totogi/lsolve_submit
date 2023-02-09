//
// Created by abdul on 09/02/2023.
//

#include "io_func.h"

/* Description:
 * prints matrix provided in COO format where data is stored in 2D array
 *
 * Inputs:
 * matrix: matrix to print
 * */
void print_matrix(COO_ptr<matrix_dtype>& matrix){
    cout << endl << " --- Printing Matrix --- " << endl;
    for (int i = 0; i < matrix.n_row; i++) {
        for (int j = 0; j < matrix.n_col; j++) {
            cout << matrix.data[i + j * matrix.n_col] << " ";
        }
        cout << endl;
    }
}


/* Description:
 * prints array
 *
 * Inputs:
 * x: array to print
 * n: array size
 * */
void print_array(matrix_dtype* x, int n) {
    for (int i = 0; i < n; i++){
        if (x[i] != x[i]) cout << 0; //For nan
        else cout << x[i];
        cout << ' ';
    }
    cout << endl;

}


/* Description:
 * prints array
 *
 * Inputs:
 * x: array to print
 * n: array size
 *
 * Note: I know this can be template but needed to move this to .h file to make it work.
 * */
void print_array_int(int* x, int n) {
    for (int i = 0; i < n; i++){
        if (x[i] != x[i]) cout << 0; //For nan
        else cout << x[i];
        cout << ' ';
    }
    cout << endl;

}


/* Description:
 * Reads .mtx file into COO format structure
 *
 * Inputs:
 * mtx_filename: path to file to be read
 *
 * Output:
 * matrix: The destination COO struct to which matrix is saved
 * */
void Mtx2vec(string mtx_filename, COO_vec<matrix_dtype>& matrix) {
    ifstream file(mtx_filename);

    // Ignore comments headers
    while (file.peek() == '%') file.ignore(2048, '\n');

    // Read number of rows and columns
    file >> matrix.n_row >> matrix.n_col >> matrix.n_nonzero;

    cout << "Rows: " << matrix.n_row << " Cols: " << matrix.n_col << " NonZeros: " << matrix.n_nonzero << endl;

    cout << "Reading from file: " << endl;
//     fill the matrix with data
    for (int l = 0; l < matrix.n_nonzero; l++)
    {
        double data;
        int row, col;
        file >> row >> col >> data;
        if (col > row) continue; //skip the upper triangle part of matrix

        matrix.data.push_back(make_tuple(row - 1, col - 1, data));

        if (l !=0 && l % 100000 == 0) cout << l << " ";
    }

    file.close();
    cout<<mtx_filename<<" file read ... "<<endl;
}


/* Description:
 * Reads .mtx file into structure where data array is 2D
 *
 * Inputs:
 * mtx_filename: path to file to be read
 *
 * Output:
 * matrix: The destination struct to which matrix is saved.
 *
 * Drawback: If matrix is large, this function consumes a lot of memory
 * */
void Mtx2Matrix(string mtx_filename, COO_ptr<matrix_dtype>& matrix) {
    ifstream file(mtx_filename);
    if(file.fail()){
        cout << "File "<< mtx_filename <<" not found"<<endl;
        exit(-1);
    }
    // Ignore comments headers
    while (file.peek() == '%') file.ignore(2048, '\n');

    // Read number of rows and columns
    file >> matrix.n_row >> matrix.n_col >> matrix.n_nonzero;

    cout << "Rows: " << matrix.n_row << " Cols: " << matrix.n_col << " NonZeros: " << matrix.n_nonzero << endl;

    // Create 2D array and fill with zeros
    matrix.data = new matrix_dtype[matrix.n_row * matrix.n_col](); //This also inits to zero
    std::fill(matrix.data, matrix.data + matrix.n_row * matrix.n_col, 0.); //But setting to 0 to be safe

    cout << "Reading from file: "<<endl;
    // fill the matrix with data
    for (int l = 0; l < matrix.n_nonzero; l++)
    {
        matrix_dtype data;
        int row, col;
        file >> row >> col >> data;
        if (col > row) continue; //skip the upper triangle part of matrix

        matrix.data[(row - 1) + (col - 1) * matrix.n_row] = data;

        if (l!= 0 && l % 100000 == 0) cout << l << " ";
    }

    file.close();
    cout<<mtx_filename<<" file read ... "<<endl;
}


/* Description:
 * Reads .mtx file into COO format structure
 *
 * Inputs:
 * mtx_filename: path to file to be read
 *
 * Output:
 * matrix: The destination COO struct to which matrix is saved
 * */
void Mtx2Matrix_vec(string mtx_filename, COO_vec<matrix_dtype>& matrix) {
    ifstream file(mtx_filename);

    // Ignore comments headers
    while (file.peek() == '%') file.ignore(2048, '\n');

    // Read number of rows and columns
    file >> matrix.n_row >> matrix.n_col >> matrix.n_nonzero;

    cout << "Rows: " << matrix.n_row << " Cols: " << matrix.n_col << " NonZeros: " << matrix.n_nonzero << endl;

    int diagonalVals = 0;
    //std::fill(matrix, matrix + n_row * n_col, 0.);
    cout << "Reading from file: " << endl;
    // fill the matrix with data
    for (int l = 0; l < matrix.n_nonzero; l++)
    {
        matrix_dtype data;
        int row, col;
        file >> row >> col >> data;
        if (col > row) continue; //skip the upper triangle part of matrix

        matrix.data.push_back(make_tuple(row - 1, col - 1, data));
        if (row == col && data != 0) diagonalVals++; //Counting diagonal values to find any missing

        if (l != 0 && l % 100000 == 0) cout << l << " ";
    }

    file.close();
    cout<<mtx_filename<<" file read ... "<<endl;

    cout << "Values in Diagonal = " << diagonalVals << endl;
    if (diagonalVals != matrix.n_row) {
        cout << "Matrix inverse not possible as diagonal has zeros " << endl;
        exit(-1);
    }

}


/* Description:
 *  Converts a matrix in COO format to CSC format
 *
 * Inputs:
 * matrix: COO formatted matrix where data is stored in a vector
 *
 * Outputs:
 * L_csc: destination matrix. It contains Lp,Li,Lx
 * */
void Matrix2CSC_vec(COO_vec<matrix_dtype>& matrix, CSC<matrix_dtype>& L_csc) {
    int Lp_counter = 0;
    int Lx_counter = 0;

    // Before converting to CSC, need to order matrix_coo by column pointer
    sort(matrix.data.begin(), matrix.data.end(), sortbysec);

    int prev_col = -1;
    int new_col = 0;
    for (int i = 0; i < matrix.data.size(); ++i) {
        new_col = get<1>(matrix.data[i]);
        if (new_col != prev_col) { //If new column, then add to column array
            L_csc.Lp[Lp_counter] = Lx_counter;
            prev_col = new_col;
            Lp_counter++;
        }

        L_csc.Lx[Lx_counter] = get<2>(matrix.data[i]);
        L_csc.Li[Lx_counter] = get<0>(matrix.data[i]);
        Lx_counter++;

    }

    L_csc.Lp_size = Lp_counter+1; //Add 1 to size of column array for traversal to work correctly

}


/* Description:
 * Reads file in .mtx format and converts to COO. Then converts COO to CSC.
 *
 * Inputs:
 * mtx_filename: .mtx file to read
 * displayM: print the matrix in COO format
 * displayCSC: print matrix in CSC format
 *
 * Outputs:
 * L_csc: matrix in CSC format
 * */
void Mtx2CSC_vec(string mtx_filename, CSC<matrix_dtype>& L_csc, bool displayM, bool displayCSC) {
    L_csc.n_row, L_csc.n_col, L_csc.n_nonzero = 0;

    /*
        Reading MTX File and converting to COO Matrix
    */
    COO_vec<matrix_dtype> matrix;
    cout << "Reading mtx and storing in 2D matrix" << endl;
    Mtx2Matrix_vec(mtx_filename, matrix);
    cout << "Reading mtx and storing in 2D matrix: Completed" << endl;
    if(displayM) matrix.print();
    /*
        Converting COO Matrix to CSC format
    */

    cout << "Converting COO matrix to CSC" << endl;

    L_csc.Lp = new int[matrix.n_col+1](); // This can be less than n_col if a column is completely empty
    L_csc.Li = new int[matrix.n_nonzero](); // new int[10]()   : This syntax initializes array to zero
    L_csc.Lx = new matrix_dtype[matrix.n_nonzero]();
    L_csc.Lp_size = 0;
    L_csc.n_col = matrix.n_col;
    L_csc.n_row = matrix.n_row;
    L_csc.n_nonzero = matrix.n_nonzero;


    cout << "Converting 2D matrix to CSC" << endl;
    Matrix2CSC_vec(matrix, L_csc);
    L_csc.Lp[L_csc.n_col] = L_csc.Lp[L_csc.n_col-1]+1; //Duplicate last entry to make lsolve work

    if (displayCSC) L_csc.print();

}

