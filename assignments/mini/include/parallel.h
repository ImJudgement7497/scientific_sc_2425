#ifndef PARALLEL_H
#define PARALLEL_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

// Global variables
extern int GRID_SIZE; // For an N point grid, you need N+1 grid size
extern double GRID_MIN;
extern double GRID_MAX;
extern double GRID_STEP;
extern int INNER_GRID_SIZE;
extern int INNER_GRID_MIN_INDEX;
extern int INNER_GRID_MAX_INDEX;
extern double TOLERANCE;

// Function declarations
void log_global_variables();
bool load_config(const string &filename);
int get_index(double x, double y);
pair<double, double> get_coordinates(int index);
void fill_heat_sources(vector<double> &grid);
vector<double> step(vector<double> &current_grid);
int execute_parallel(int argc, char **argv);

#endif // SERIAL_H
