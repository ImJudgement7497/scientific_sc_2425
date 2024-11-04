#ifndef PARALLEL_H
#define PARALLEL_H

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <mpi.h>

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

// Config Functions
void check_processor_initalisation(int rank, int size);
void get_sub_indices(int size);
void log_global_variables();
void generate_mappings();
void log_sources();
bool load_config(const string &filename);

// Indice Functions
int get_index(double x, double y);
vector<int> get_inner_indices();
pair<double, double> get_coordinates(int index);
void fill_local_sources(vector<double> &local_grid, vector<int> &local_source_indices, vector<double> &local_source_values);

// Simulation Functions
void fill_sources(vector<double> &grid);
vector<double> step(vector<double> &current_grid);
int execute_parallel();

#endif // PARALLEL_H
