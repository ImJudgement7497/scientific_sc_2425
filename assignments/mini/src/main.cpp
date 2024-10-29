#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

int GRID_SIZE; // For an N point grid, you need N+1 grid size
double GRID_MIN;
double GRID_MAX;
double GRID_STEP;
/*----------------------------------HELPER FUNCTIONS------------------------------------------------------------*/
double safe_access(const vector<double> &grid, int index)
{
    try
    {
        return grid.at(index);
    }
    catch (const out_of_range &)
    {
        return 0.0;
    }
}
void print_vector(const vector<double> &vec)
{
    cout << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        cout << vec[i];
        if (i < vec.size() - 1)
        { // Add a comma except for the last element
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

bool load_config(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    file >> GRID_SIZE >> GRID_MIN >> GRID_MAX;

    GRID_STEP = (GRID_MAX - GRID_MIN) / (GRID_SIZE - 1);
    file.close();

    return true;
}
/*
Map the (x, y) coordiante to a singular index
*/
int get_index(double x, double y)
{
    int col_index = round(x / GRID_STEP);
    int row_index = round(y / GRID_STEP);
    int index = row_index * GRID_SIZE + col_index;
    return index;
}

pair<double, double> get_coordinates(int index)
{
    int row_index = index / GRID_SIZE; // Calculate row
    int col_index = index % GRID_SIZE; // Calculate column

    double x = col_index * GRID_STEP; // Convert column to x coordinate
    double y = row_index * GRID_STEP; // Convert row to y coordinate

    return {x, y}; // Return as a pair of doubles
}
/* ----------------------------------MAIN FUNCTIONS--------------------------------------------------*/
/*
Initalise the heat sources
*/
void fill_heat_sources(vector<double> &grid)
{
    grid[get_index(5.0, 5.0)] = 10.0;
    grid[get_index(4.0, 6.0)] = 7.2;
    grid[get_index(7.0, 2.5)] = -1.2;
}

void iterate(vector<double> &grid)
{
    /*
    MOVING RIGHT IN X: index + 1
    MOVING LEFT IN X: index - 1
    MOVING UP IN Y: index + GRID_SIZE
    MOVING DOWN IN Y: index - GRID_SIZE
    */

    vector<double> new_grid(GRID_SIZE * GRID_SIZE, 0.0);
    new_grid = grid;
    for (int index = 0; index < GRID_SIZE * GRID_SIZE; index++)
    {
        double current = safe_access(new_grid, index);
        double left = safe_access(new_grid, index - 1);
        double right = safe_access(new_grid, index + 1);
        double up = safe_access(new_grid, index + GRID_SIZE);
        double down = safe_access(new_grid, index - GRID_SIZE);

        new_grid[index] = (current + left + right + up + down) / 5.0;

        // cout << "Index: " << index << endl;
        // cout << "Index + 1: " << index + 1 << endl;
        // cout << "Index - 1: " << index - 1 << endl;
        // cout << "Index + GRID_SIZE: " << index + GRID_SIZE << endl;
        // cout << "Index - GRID_SIZE: " << index - GRID_SIZE << endl;
        // cout << "----------------------------------------" << endl;
    }
    grid = new_grid;
}

int main()
{
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }
    vector<double> grid(GRID_SIZE * GRID_SIZE, 0.0);
    fill_heat_sources(grid);

    // for (double i = GRID_MIN; i <= GRID_MAX; i += GRID_STEP)
    // {
    //     cout << "X Y  Index" << endl;
    //     cout << "---------------------------------------------------" << endl;
    //     for (double j = GRID_MIN; j <= GRID_MAX; j += GRID_STEP)
    //     {
    //         cout << j << " " << i << " :" << get_index(j, i) << endl;
    //     }
    // }
    print_vector(grid);
    iterate(grid);
    print_vector(grid);
    return 0;
}