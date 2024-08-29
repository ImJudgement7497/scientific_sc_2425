#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

class Ship {

    public:
        int size;
        vector<pair<int, int>> position;
        int hits;

        // Constructor
        Ship(int size_value) : size(size_value) {};

        // Print ship size
        void print_size() {
            cout << size << endl;
        }

};

// class Player {


// };

// class BattleshipGame {

// private:
//     static const int grid_size = 10; // static ensures all functions within the class use the same object


// };

int main() {

    Ship ship(5);
    ship.print_size();
    return 0;
    
}
