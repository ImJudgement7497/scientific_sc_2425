#include <iostream>
#include <vector>

using namespace std;

const int GRID_SIZE = 100; // ROWS + COLUMNS
const double GRID_MIN = 0.0;
const double GRID_MAX = 10.0;
const double GRID_STEP = (GRID_MAX - GRID_MIN) / (GRID_SIZE);

/*
Map the (x, y) coordiante to a singular index
*/
int get_index(const double &x, const double &y)
{
    int row_index = (y * 10);
    int col_index = (x * 10);
    return row_index * GRID_SIZE + col_index;
}

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
    vector<double> new_grid(GRID_SIZE * GRID_SIZE, 1.0);
}

int main()
{
    vector<double> grid(GRID_SIZE * GRID_SIZE, 1.0);
    fill_heat_sources(grid);

    for (double i = 0.0; i <= 10.0; i += 0.1)
    {
        for (double j = 0.0; j <= 10.0; j += 0.1)
        {
            cout << i << " " << j << " :" << get_index(i, j) << endl;
        }
    }

    return 0;
}