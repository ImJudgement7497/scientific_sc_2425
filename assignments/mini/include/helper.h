#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

// Function declarations
void print_vector(const vector<double> &vec);
void print_vector(const vector<int> &vec);
bool allclose(const vector<double> &vec1, const vector<double> &vec2, double &TOLERANCE);

#endif // HELPER_H
