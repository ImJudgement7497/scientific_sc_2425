#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

// Function declarations
void print_vector(const vector<double> &vec);
void print_vector(const vector<int> &vec);
void write_vector(const vector<double> &vec, const string file_name);
void write_vector(const vector<int> &vec, const string file_name);
bool allclose(const vector<double> &vec1, const vector<double> &vec2, double &TOLERANCE);
bool allclose(const vector<double> &vec1, const vector<double> &vec2, double &TOLERANCE, vector<int> &iterating_indices);

#endif // HELPER_H
