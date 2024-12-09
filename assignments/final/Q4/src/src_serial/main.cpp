#include <iostream>
#include <vector>
#include <fstream>
#include "vtimer_t.h"
#include "rng.h"

using namespace std;

#define L 500   // Length of box (to be changed to an input file at a later date)
#define r 1.234 // Radius of circle (to be changed to an input file at a later date)

/* ------------------------------I/O FUNCTIONS------------------------------ */

/* Writes a vector of pairs to a file*/
void write_coordinates(const vector<pair<double, double>> &coordinates, const string &filename)
{
    ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &coord : coordinates)
        {
            file << coord.first << " " << coord.second << "\n";
        }
        file.close();
    }
    else
    {
        cerr << "Error opening file: " << filename << endl;
    }
}

/* Generates and returns a pair of random doubles */
pair<double, double> gen_random_pair(rng &random_gen)
{
    pair<double, double> rand_coords;
    rand_coords.first = L * random_gen.grnd();
    rand_coords.second = L * random_gen.grnd();

    return rand_coords;
}

/* Checks if a coordiante is outside a boundary*/
bool check_boundaries(pair<double, double> &coords)
{
}

int main()
{

    rng random_gen;
    random_gen.seed(1829233);

    bool is_overlapping = false;

    vector<pair<double, double>> circle_coords;
    pair<double, double> test = gen_random_pair(random_gen);
    circle_coords.push_back(test);

    cout << circle_coords[0].first << " " << circle_coords[0].second << endl;

    for (int k = 0; k < 5; k++)
    {
        for (int i = 0; i < 1000; i++)
        {
            printf("\r k = %d, i = %d, size = %d", k, i, circle_coords.size());
            fflush(stdout); // Ensure it flushes to the terminal

            printf("\r%s", string(30, ' ').c_str()); // Clear the line (30 spaces)
            pair<double, double> new_circle = gen_random_pair(random_gen);

            // cout << new_circle.first << " " << new_circle.second << endl;

            for (int j = 0; j < circle_coords.size(); j++)
            {
                double distance_squared = (circle_coords[j].first - new_circle.first) * (circle_coords[j].first - new_circle.first) +
                                          (circle_coords[j].second - new_circle.second) * (circle_coords[j].second - new_circle.second);

                /* MAY NEED TO CHANGE THE CONDITION HERE */
                if (distance_squared < 4 * r * r)
                {
                    is_overlapping = true;
                    break;
                }
            }

            if (!is_overlapping)
            {
                circle_coords.push_back(new_circle);
            }
        }
    }

    cout << circle_coords.size() << endl;
    write_coordinates(circle_coords, "coords.txt");

    return 0;
}