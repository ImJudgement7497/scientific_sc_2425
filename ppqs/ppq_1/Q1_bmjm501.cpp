#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double calculate_pi(const int &N)
{
    double pi = 0.0;

    for (int i = 1; i <= N; i++)
    {
        double denom = (i - 0.5) / (N);
        double sum = (1) / (1 + pow(denom, 2));
        pi = pi + sum;
    }

    return (pi * 4) / N;
}

double percent_error_calc(const double &approx)
{
    double error = abs((M_PI - approx) / (M_PI)) * 100.0;
    return error;
}

int main()
{
    vector<int> n_values = {10, 100, 1000, 10000, 100000, 999999999};
    cout.precision(20);

    for (int n : n_values)
    {
        double approx = calculate_pi(n);
        cout << n << " iterations: " << approx << ": Percent Error: " << percent_error_calc(approx) << endl;
    }
}