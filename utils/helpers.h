#ifndef HELPERS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HELPERS_H

#include <iostream>
#include <vector>
#include <tuple>
#include <set>
using namespace std;

#include <filesystem>
#include <unistd.h>

void print_cwd();
void set2array(set<int> set_of_smth, int* &arr);

bool sortbysec(const tuple<int, int, double>& a, const tuple<int, int, double>& b);
void find_adj(int* Lp, int* Li, double* Lx, int col, set<int> &visited_cols);
void copyArray(double* src, double*& target, int size);

#endif