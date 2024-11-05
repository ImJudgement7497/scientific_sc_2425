#include "helper.h"

// Function to print a vector
void print_vector(const vector<double> &vec)
{
    cout << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        cout << vec[i];
        if (i < vec.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

void write_vector(const vector<double> &vec, const string file_name)
{
    ofstream log_file(file_name, ios::app);
    log_file << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        log_file << vec[i];
        if (i < vec.size() - 1)
        {
            log_file << ", ";
        }
    }
    log_file << "]" << endl;
    log_file << "--------------------------------------------" << endl;
}

void print_vector(const vector<int> &vec)
{
    cout << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        cout << vec[i];
        if (i < vec.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

// Function to check if two vectors are close within a tolerance
bool allclose(const vector<double> &vec1, const vector<double> &vec2, double &TOLERANCE)
{
    if (vec1.size() != vec2.size())
    {
        return false;
    }

    // Compare each element within tolerance
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        double test = fabs(vec1[i] - vec2[i]);
        if (test > TOLERANCE)
        {
            return false;
        }
    }
    return true;
}
