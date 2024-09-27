#include <iostream>
#include <vector>
#include <fstream>
#include "vtimer_t.h"

using namespace std;

void print_int_vector(vector<int> &vec)
{
    cout << "vector size" << vec.size() << endl;
    for (int elm : vec)
    {
        cout << elm << endl;
    }
}

void write_vector_to_file(const string file_name, vector<int> &vec)
{
    ofstream ofile;

    ofile.open(file_name);

    for (int elm : vec)
    {
        ofile << elm << endl;
    }

    ofile.close();
}

vector<int> read_vector_file(const string file_name)
{

    ifstream read_file(file_name);
    vector<int> read_vec;
    string read_elm_string;
    int read_elm_int;

    while (getline(read_file, read_elm_string))
    {
        read_elm_int = stoi(read_elm_string);
        read_vec.push_back(read_elm_int);
    }

    read_file.close();

    return read_vec;
}

void write_vector_binary(const string file_name, vector<int> &vec)
{
    ofstream ofile(file_name, ios::binary);
    ofile.write(reinterpret_cast<const char *>(&vec[0]), sizeof(int) * vec.size());
}

void read_vector_binary(const string file_name)
{
    ifstream ifile(file_name, ios::binary);
    
}

void run_test(const int size)
{
    vtimer_t timer;

    vector<int> my_vec(size, 5);

    timer.start();
    write_vector_to_file("./src/vector.txt", my_vec);
    timer.stop();
    cout << "Size: " << size << ", Write: " << timer.elapsed_time() << endl;

    timer.start();
    vector<int> test_vec = read_vector_file("./src/vector.txt");
    timer.stop();
    cout << "Size: " << size << ", Read: " << timer.elapsed_time() << endl;
}

int main()
{
    run_test(100000);
    vector<int> my_vec(100, 100);
    write_vector_binary("./src/vector_binary", my_vec);

    return 0;
}