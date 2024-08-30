#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

class Ship
{

public:
    int size;                         // Size of the ship
    vector<pair<int, int>> positions; // Vector of coordinates for position (needs to be same size as size)
    int hits;                         // Number of hits on the ship

    // Constructor         Construct size value and default hits to 0
    Ship(int size_value) : size(size_value), hits(0) {};

    // Check if size of ship is equal to the length of position vector
    bool check_size() const
    {
        return size == positions.size();
    }

    // Set the poistion of the ship
    void set_position(const vector<pair<int, int>> &pos)
    {
        positions = pos;
    }

    // Checks if a coordinate is equal to the position of ship
    bool occupies_position(int x, int y) const
    {
        for (const auto &pos : positions)
        {
            if (pos.first == x && pos.second == y)
            {
                return true;
            }
        }
        return false;
    }

    // Checks if a ship is sunk
    bool is_sunk() const
    {
        return hits >= size;
    }
};

// class Player {

// };

// class BattleshipGame {

// private:
//     static const int grid_size = 10; // static ensures all functions within the class use the same object

// };

int main()
{

    Ship ship(5);

    return 0;
}
