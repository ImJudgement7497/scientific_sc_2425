#include <iostream>
#include <vector>

using namespace std;

void printArray(double array[100][100])
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            std::cout << array[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void fill_heat_sources(double (&grid)[100][100])
{
    grid[50][50] = 10.0;
    grid[40][60] = 7.2;
    grid[70][25] = -1.2;
}

void iterate(double new_grid[100][100], double (&old_grid)[100][100])
{
    new_grid[0][0] = 100000.0;
}
int main()
{
    double grid[100][100] = {};
    fill_heat_sources(grid);

    return 0;
}