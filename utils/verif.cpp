#include "verif.h"


/* Description:
 * Check if the two arrays are equal
 *
 * Inputs:
 * A,B: Arrays that need to be compared
 * size: sizes of both arrays. Assumed to be same
 *
 * Outputs:
 * bool: True if all corresponding elements are equal
 * */
bool isEqual(matrix_dtype* A, matrix_dtype* B, int size){
    int not_equal = 0;
    cout << "Verif size="<<size<<endl;
    for(int i=0; i<size; i++){
//        cout << i <<": ";
//        cout << A[i] <<endl;
        if(A[i]!=A[i] && B[i]!=B[i]) continue; //If both nan, then consider equal

        if(A[i] != B[i]) {
            not_equal++;
            cout << A[i]<<" not equal "<<B[i]<< "   ";
            break;
        }
    }

    if(not_equal>0)return false;
    else return true;
}