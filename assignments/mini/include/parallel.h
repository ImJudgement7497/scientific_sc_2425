#ifndef PARALLEL_H
#define PARALLEL_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <fstream>
#include <utility>
#include <cmath>
#include <mpi.h>

using namespace std;

/*-------------------------------GLOBAL VARIABLES*-------------------------------*/
extern int GRID_SIZE;
extern double GRID_MIN;
extern double GRID_MAX;
extern double GRID_STEP;
extern int NUM_OF_PROCS;
extern int INNER_GRID_MIN_INDEX;
extern int INNER_GRID_MAX_INDEX;
extern double TOLERANCE;

/*-------------------------------CONFIG FUNCTIONS-------------------------------*/

void check_processor_initalisation(int rank, int size);
void log_global_variables();
void generate_mappings();
void log_sources();
void log_local_sources(unordered_set<int> &source_indices, const string &file_name);
bool load_config(const string &filename);

/*-------------------------------INDICE FUNCTIONS-------------------------------*/

int get_index(double x, double y);
vector<int> get_iteration_indices(vector<int> &inner_indices);
pair<double, double> get_coordinates(int index);
void get_local_indices(int size, vector<int> &starting_indices, vector<int> &ending_indices);
void initalise_indices(int rank, int size, vector<int> &iterating_indices, int counts[], int displacement[]);

/* ----------------------------------SIMULATION FUNCTIONS--------------------------------------------------*/

void fill_sources(vector<double> &grid);
unordered_set<int> get_local_sources(vector<double> &local_grid);
void fill_local_sources(vector<double> &local_grid, vector<int> &local_source_indices, vector<double> &local_source_values);
vector<double> scatter_grid(int rank, int size, vector<double> &full_grid, int counts[], int displacement[]);
vector<double> gather_grid(int rank, int size, vector<double> &local_grid, int counts[], int displacement[]);
vector<double> step(int rank, int size, vector<double> &local_grid, unordered_set<int> &source_indices,
                    vector<int> &iterating_indices, int counts[], int displacement[]);
int execute_parallel();

#endif // PARALLEL_H
