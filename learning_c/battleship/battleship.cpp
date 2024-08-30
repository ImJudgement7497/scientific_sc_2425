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
    bool occupies_position(pair<int, int> target) const
    {
        for (const auto &pos : positions)
        {
            if (pos.first == target.first && pos.second == target.second)
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

private:
    void print_vector_of_pairs(const vector<pair<int, int>> &vec)
    {
        for (const auto &p : vec)
        {
            cout << "(" << p.first << ", " << p.second << ")" << endl;
        }
    }

    void print_grid(const vector<vector<char>> &grid)
    {
        for (const auto &row : grid)
        {
            for (const auto &cell : row)
            {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    // Add all the coordinates to a vector of coordinates
    vector<pair<int, int>> fill_position(const pair<int, int> &first_coor, char &orientation, Ship &ship)
    {

        vector<pair<int, int>> positions;
        pair<int, int> second_coor;

        for (int i = 1; i <= ship.size; i++)
        {
            if (orientation == 'V')
            {
                second_coor.first = first_coor.first + i - 1;
                second_coor.second = first_coor.second;
            }

            else
            {
                second_coor.first = first_coor.first;
                second_coor.second = first_coor.second + i - 1;
            }

            positions.push_back(second_coor);
        }

        return positions;
    }

    // Helper function to get and validate coordinate input
    int get_coordinate(const string &prompt)
    {
        int coord;
        while (true)
        {
            cout << prompt;
            cin >> coord;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter an integer." << endl;
            }
            else
            {
                return coord;
            }
        }
    }

    // Helper function to get and validate orientation input
    char get_orientation()
    {
        char orientation;
        while (true)
        {
            cout << "Would you like to place it vertically (V) or horizontally (H)? ";
            cin >> orientation;

            if (orientation == 'V' || orientation == 'H')
            {
                return orientation;
            }
            else
            {
                cout << "Error: Please enter 'V' or 'H'." << endl;
            }
        }
    }

    // Function to check if the ship's positions are valid and do not overlap with other ships
    bool check_ship_positions(const Ship &ship)
    {
        // Check if all positions are within bounds
        for (const auto &pos : ship.positions)
        {
            if (pos.first < 0 || pos.first >= 11 || pos.second < 0 || pos.second >= 11)
            {
                return false;
            }
        }

        // Check for overlaps with other ships
        for (const auto &other_ship : ships_vector)
        {
            for (const auto &pos : ship.positions)
            {
                // If pos is found in other_ship.positions, return false
                if (find(other_ship.positions.begin(), other_ship.positions.end(), pos) != other_ship.positions.end())
                {
                    return false;
                }
            }
        }

        return true;
    }

    // Function to place a single ship
    void place_single_ship(int size)
    {
        Ship ship(size);

        bool stop = true;

        while (stop)
        {
            pair<int, int> first_coor;
            cout << "Player Name: " << name << endl;

            // Get and validate coordinates
            first_coor.first = get_coordinate("Enter row (integer): ");
            first_coor.second = get_coordinate("Enter column (integer): ");

            char orientation = get_orientation();

            // Set ship position
            ship.set_position(fill_position(first_coor, orientation, ship));

            if (check_ship_positions(ship))
            {
                stop = false; // Exit the loop if all positions are valid
                change_grid(ship.positions, 'S', player_grid);
            }
            else
            {
                cout << "Error: Some positions are out of bounds. Please enter the coordinates again." << endl;
            }
        }

        ships_vector.push_back(ship);
    }

public:
    vector<vector<char>> player_grid;
    vector<vector<char>> opponent_grid;
    vector<Ship> ships_vector;
    string name;

    void print_player_grid()
    {
        for (const auto &row : player_grid)
        {
            for (const auto &cell : row)
            {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    void print_opponent_grid()
    {
        for (const auto &row : opponent_grid)
        {
            for (const auto &cell : row)
            {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    // Function to initialize the grid and print it
    void initalise_grid()
    {
        // Initialize a 11x11 grid with the character '~'
        vector<vector<char>> template_grid(11, vector<char>(11, '~'));
        template_grid[0][0] = ' ';

        for (int col = 1; col < 11; col++)
        {

            template_grid[0][col] = col + 47; // Need ASCI for int '0-9'
        }

        for (int row = 1; row < 11; row++)
        {
            template_grid[row][0] = row + 47;
        }

        // print_grid(inital_grid);
        player_grid = template_grid;
        opponent_grid = template_grid;
    }

    // Change grid at a given coordinate with a specfied change
    // Handles out of bounds exceptions
    void change_grid(pair<int, int> &coor, char change, vector<vector<char>> &grid)
    {

        grid[coor.first + 1][coor.second + 1] = change;
        print_grid(grid);
    }

    void change_grid(vector<pair<int, int>> &positions, char change, vector<vector<char>> &grid)
    {
        for (auto &pos : positions)
        {
            grid[pos.first + 1][pos.second + 1] = change;
        }
        print_grid(grid);
    }

    // Get name input
    void get_name()
    {
        cout << "What is your name?" << endl;
        cin >> name;
    }

    // Main function to place all ships
    void place_ships()
    {
        vector<int> sizes = {5, 4, 3, 3, 2};
        for (int size : sizes)
        {
            cout << "-------------------------------------------------------------------------------------------" << endl;
            cout << "Placing Ship of size " << size << endl;
            place_single_ship(size);
        }
    }

    pair<int, int> shoot()
    {

        pair<int, int> target_coor;
        target_coor.first = get_coordinate("Enter target row (integer): ");
        target_coor.second = get_coordinate("Enter target column (integer): ");

        return target_coor;
    }

    // Returns true if hit
    bool recieve_shot(pair<int, int> &target_coor)
    {
        for (auto &ship : ships_vector)
        {
            if (ship.occupies_position(target_coor))
            {
                change_grid(target_coor, 'X', player_grid);
                ship.hits++;
                return true;
            }
            else
            {
                change_grid(target_coor, 'M', player_grid);
                return false;
            }
        }
    }
};

class BattleshipGame
{
private:
    void clear_terminal()
    {
        cout << "Clearing: " << endl;
        cin.get();
        system("clear");
    }

    void print_battleship_title()
    {
        std::cout << R"(
------------------------------------------------
  ____        _   _   _          _     _       
 |  _ \      | | | | | |        | |   (_)      
 | |_) | __ _| |_| |_| | ___ ___| |__  _ _ __  
 |  _ < / _` | __| __| |/ _ / __| '_ \| | '_ \ 
 | |_) | (_| | |_| |_| |  __\__ | | | | | |_) |
 |____/ \__,_|\__|\__|_|\___|___|_| |_|_| .__/ 
                                        | |    
                                        |_|    
------------------------------------------------    
    )" << std::endl;
    }

public:
    Player player_1;
    Player player_2;
    int player_tracker = 1; 

    void start_game()
    {

        player_1.initalise_grid();
        cout << "Player 1" << endl;
        player_1.get_name();

        player_2.initalise_grid();
        cout << "Player 2" << endl;
        player_2.get_name();
        clear_terminal();
        print_battleship_title();

        player_1.place_ships();
        clear_terminal();
        player_2.place_ships();
        clear_terminal();

        
    }
};

int main()
{

    BattleshipGame game;
    game.start_game();

    return 0;
}
