#include "Player.h"

// Helper function to print a vector of pairs (used internally)
void Player::print_vector_of_pairs(const vector<pair<int, int>> &vec) {
    for (const auto &p : vec) {
        cout << "(" << p.first << ", " << p.second << ")" << endl;
    }
}

// Helper function to generate ship positions based on orientation and starting coordinate
vector<pair<int, int>> Player::fill_position(const pair<int, int> &first_coor, char &orientation, Ship &ship) {
    vector<pair<int, int>> positions;
    pair<int, int> second_coor;

    for (int i = 1; i <= ship.size; i++) {
        if (orientation == 'V') {
            second_coor.first = first_coor.first + i - 1;
            second_coor.second = first_coor.second;
        } else {
            second_coor.first = first_coor.first;
            second_coor.second = first_coor.second + i - 1;
        }
        positions.push_back(second_coor);
    }

    return positions;
}

// Helper function to get and validate coordinate input
int Player::get_coordinate(const string &prompt) {
    int coord;
    while (true) {
        cout << prompt;
        cin >> coord;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer." << endl;
        } else {
            return coord;
        }
    }
}

// Helper function to get and validate orientation input
char Player::get_orientation() {
    char orientation;
    while (true) {
        cout << "Would you like to place it vertically (V) or horizontally (H)? ";
        cin >> orientation;

        if (orientation == 'V' || orientation == 'H') {
            return orientation;
        } else {
            cout << "Error: Please enter 'V' or 'H'." << endl;
        }
    }
}

// Function to check if the ship's positions are valid and do not overlap with other ships
bool Player::check_ship_positions(const Ship &ship) {
    for (const auto &pos : ship.positions) {
        if (pos.first < 0 || pos.first >= 11 || pos.second < 0 || pos.second >= 11) {
            return false;
        }
    }

    for (const auto &other_ship : ships_vector) {
        for (const auto &pos : ship.positions) {
            if (find(other_ship.positions.begin(), other_ship.positions.end(), pos) != other_ship.positions.end()) {
                return false;
            }
        }
    }

    return true;
}

// Function to place a single ship on the grid
void Player::place_single_ship(int size) {
    Ship ship(size);
    bool stop = true;

    while (stop) {
        pair<int, int> first_coor;
        cout << "Player Name: " << name << endl;

        first_coor.first = get_coordinate("Enter row (integer): ");
        first_coor.second = get_coordinate("Enter column (integer): ");

        char orientation = get_orientation();
        ship.set_position(fill_position(first_coor, orientation, ship));

        if (check_ship_positions(ship)) {
            stop = false;
            change_grid(ship.positions, 'S', player_grid);
            print_player_grid();
        } else {
            cout << "Error: Some positions are out of bounds. Please enter the coordinates again." << endl;
        }
    }

    ships_vector.push_back(ship);
}

// Function to print the grid (generic, can print player or opponent grid)
void Player::print_grid(const vector<vector<char>> &grid) {
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Function to print the player's grid
void Player::print_player_grid() {
    print_grid(player_grid);
}

// Function to print the opponent's grid
void Player::print_opponent_grid() {
    print_grid(opponent_grid);
}

// Function to initialize the player's grid with default values
void Player::initalise_grid() {
    vector<vector<char>> template_grid(11, vector<char>(11, '~'));
    template_grid[0][0] = ' ';

    for (int col = 1; col < 11; col++) {
        template_grid[0][col] = col + 47;
    }

    for (int row = 1; row < 11; row++) {
        template_grid[row][0] = row + 47;
    }

    player_grid = template_grid;
    opponent_grid = template_grid;
}

// Function to modify a specific coordinate on the grid
void Player::change_grid(pair<int, int> &coor, char change, vector<vector<char>> &grid) {
    grid[coor.first + 1][coor.second + 1] = change;
}

// Overloaded function to modify multiple coordinates on the grid
void Player::change_grid(vector<pair<int, int>> &positions, char change, vector<vector<char>> &grid) {
    for (auto &pos : positions) {
        grid[pos.first + 1][pos.second + 1] = change;
    }
}

// Function to change a single coordinate on the player's grid
void Player::change_player_grid(pair<int, int> &coor, char change) {
    change_grid(coor, change, player_grid);
}

// Function to change a single coordinate on the opponent's grid
void Player::change_opponent_grid(pair<int, int> &coor, char change) {
    change_grid(coor, change, opponent_grid);
}

// Function to prompt the player to enter their name
void Player::get_name() {
    cout << "What is your name?" << endl;
    cin >> name;
}

// Main function to handle the placement of all ships
void Player::place_ships() {
    vector<int> sizes = {5, 4, 3, 3, 2};
    for (int size : sizes) {
        cout << "-------------------------------------------------------------------------------------------" << endl;
        cout << "Placing Ship of size " << size << endl;
        place_single_ship(size);
    }
}

// Function to get target coordinates from the player for shooting
pair<int, int> Player::shoot() {
    pair<int, int> target_coor;
    target_coor.first = get_coordinate("Enter target row (integer): ");
    target_coor.second = get_coordinate("Enter target column (integer): ");
    return target_coor;
}

// Function to handle receiving a shot and updating the player's grid
bool Player::recieve_shot(pair<int, int> &target_coor) {
    for (auto &ship : ships_vector) {
        if (ship.occupies_position(target_coor)) {
            change_player_grid(target_coor, 'X');
            ship.hits++;
            return true;
        }
    }
    change_player_grid(target_coor, 'M');
    return false;
}
