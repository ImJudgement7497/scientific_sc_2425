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

class Player
{
public:
    vector<vector<int>> grid;
    vector<Ship> ships;
    string name;

    void create_ships()
    {
        vector<int> sizes = {5, 4, 3, 3, 2};

        for (int size : sizes)
        {
            Ship ship(size);       // Create a Ship object directly
            ships.push_back(ship); // Push the object into the vector
        }
    }

    // Find the coordinates from the first corner and orientation
    pair<int, int> find_coordinates(const pair<int, int> &first_coor, char &orientation, Ship &ship)
    {
        pair<int, int> second_coor;

        if (orientation == 'V')
        {
            second_coor.first = first_coor.first + ship.size - 1;
            second_coor.second = first_coor.second;
        }

        else
        {
            second_coor.first = first_coor.first;
            second_coor.second = second_coor.second + ship.size - 1;
        }

        return second_coor;
    }

    void place_ships()
    {
        int row;
        int column;
        pair<int, int> first_coor;
        char orientation;
        int i = 0;
        bool stop = true;

        while (i <= ships.size())
        {
            cout << "Enter row (integer): ";
            cin >> first_coor.first;

            // Check if input failed (e.g., user didn't enter an integer)
            if (cin.fail())
            {
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input. Please enter an integer for the row." << endl;
                continue; // Restart the loop
            }

            cout << "Enter column (integer): ";
            cin >> first_coor.second;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter an integer for the column." << endl;
                continue;
            }

            cout << "Would you like to place it vertically (V) or horizontally (H)? ";
            cin >> orientation;

            // Check the orientation input
            if (orientation != 'V' && orientation != 'H')
            {
                cout << "Error: Please enter 'V' or 'H'." << endl;
                continue;
            }

            cout << "Ship size" << ships[i].size << ": " << i << endl;
            cout << "1st " << first_coor.first << first_coor.second << endl;

            pair<int, int> second_coor = find_coordinates(first_coor, orientation, ships[i]);

            cout << "2nd " << second_coor.first << second_coor.second << endl;

            i++;
        }
    }

    void recieve_shot()
    {
    }
};

// class BattleshipGame {

// private:
//     static const int grid_size = 10; // static ensures all functions within the class use the same object

// };

int main()
{

    Player player;
    player.create_ships();
    player.place_ships();
    return 0;
}
