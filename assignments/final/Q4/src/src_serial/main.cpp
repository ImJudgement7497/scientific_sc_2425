#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "vtimer_t.h"
#include "rng.h"

using namespace std;

/* ------------------------------GLOBAL VARIABlE------------------------------ */
double L; // Length of box (read in from input)
double r; // Radius of circle (read in from input)

/* ------------------------------I/O FUNCTIONS------------------------------ */

/* Writes a vector of pairs to a file */
void write_coordinates(const vector<pair<double, double>> &coordinates, const string &file_name)
{
    ofstream file(file_name);
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
        cerr << "Error opening file: " << file_name << endl;
    }
}

/* Writes a string to a file */
void write_string_to_file(const string &data, const string &file_name)
{
    ofstream file(file_name, ios::app);
    if (file.is_open())
    {
        file << data << "\n";
        file.close();
    }
    else
    {
        cerr << "Error opening file: " << file_name << endl;
    }
}

/* Parses and loads the config file */
bool load_config(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines or lines that are comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            string key = line.substr(0, pos); // Key is the part before the space
            string value_str = line.substr(pos + 1); // Value is the part after the space

            float value = stof(value_str);

            // Assign the value to the corresponding parameter
            if (key == "L") {
                L = value;
            } else if (key == "r") {
                r = value;
            } else {
                cerr << "Unknown parameter: " << key << endl;
                return false;
            }
        }
    }

    file.close();
    return true;
}

/* ------------------------------SIMULATION FUNCTIONS------------------------------*/
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

    double x = coords.first;
    double y = coords.second;

    if (x - r < 0 || x + r > L || y - r < 0 || y + r > L)
    {
        return true;
    }

    return false;
}

int main()
{
    if (!load_config("./config/config.txt"))
    {
        return -1;
    }
    cout << "Running with L = " << L << " and r = " << r << endl;
    rng random_gen;
    random_gen.seed(1829233);
#ifdef DEBUG
    cout << "DEBUGGING ENABLED" << endl;
#endif
    bool is_overlapping;

    vector<pair<double, double>> circle_coords;
    pair<double, double> test = gen_random_pair(random_gen);
    circle_coords.push_back(test);

    cout << circle_coords[0].first << " " << circle_coords[0].second << endl;
    cout << "number of circles = " << circle_coords.size() << endl;

    for (int k = 0; k < 5; k++)
    {
        for (int i = 0; i < 10000; i++)
        {
            // PUT THIS IN A NEW FLAG CALLED VISULATION OR SOMETHING
            {
                printf("\r k = %d, i = %d, size = %zu", k, i, circle_coords.size());
                fflush(stdout); // Ensure it flushes to the terminal

                printf("\r%s", string(30, ' ').c_str()); // Clear the line (30 spaces)
            }

            pair<double, double> new_circle = gen_random_pair(random_gen);

            // First checks if the circle is overlapping the boudaries
            is_overlapping = check_boundaries(new_circle);
            if (!is_overlapping)
            {
                for (int j = 0; j < circle_coords.size(); j++)
                {
                    // More efficent to compare squared distances
                    double dx = circle_coords[j].first - new_circle.first;
                    double dy = circle_coords[j].second - new_circle.second;
                    double distance_squared = dx * dx + dy * dy;

                    if (distance_squared < (2 * r) * (2 * r))
                    {
                        is_overlapping = true;

#ifdef DEBUG
                        {
                            string temp = "Distance Squared = " + to_string(distance_squared) + ", compared to " + to_string((2 * r) * (2 * r));
                            write_string_to_file(temp, "distances.txt");
                        }
#endif

                        break; // Stop checking further if overlapping
                    }
                }

                // If still not overlapping, add the new circle
                if (!is_overlapping)
                {
                    circle_coords.push_back(new_circle);
                }
            }
        }
    }

    cout << "Number of circles: " << circle_coords.size() << endl;
    write_coordinates(circle_coords, "coords.txt");

    return 0;
}