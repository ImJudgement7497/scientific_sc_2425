#include <iostream>
#include <vector>

vector<int> A(1024, 0);
vector<int> B(1024, 0);
vector<int> C(1024, 0);

int main()
{
    for (int i = 0; i < 1024; i+=32)
    {
        for (int j = i; j < i + 32; j++)
        {
            A[j] = A[j] + B[j];
        }
    }
}