#include <iostream>
#include <cmath>

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

double error_calc(const double& approx) {
    cout << M_PI;
    return 0.0;
    
} 

int main()
{
    error_calc(1.90);
    cout.precision(20);
    cout << calculate_pi(10) << endl;
    cout << calculate_pi(100) << endl;
    cout << calculate_pi(1000) << endl;
    cout << calculate_pi(10000) << endl;
    cout << calculate_pi(100000) << endl;
    cout << calculate_pi(500000) << endl;
}