#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
#include "Ship.h" // Include the header file for the Ship class

using namespace std;

class Player
{
private:
    // Helper functions
    void print_vector_of_pairs(const vector<pair<int, int>> &vec);
    vector<pair<int, int>> fill_position(const pair<int, int> &first_coor, char &orientation, Ship &ship);
    int get_coordinate(const string &prompt);
    int get_target_coordinate(const string &prompt);
    char get_orientation();
    bool check_ship_positions(const Ship &ship);
    void place_single_ship(int size);

public:
    // Member variables
    vector<vector<char>> player_grid;
    vector<vector<char>> opponent_grid;
    vector<Ship> ships_vector;
    string name;
    unordered_map<int, string> name_info;

    Player();

    // Public member functions
    void print_grid(const vector<vector<char>> &grid);
    void print_player_grid();
    void print_opponent_grid();
    void initalise_grid();
    void change_grid(pair<int, int> &coor, char change, vector<vector<char>> &grid);
    void change_grid(vector<pair<int, int>> &positions, char change, vector<vector<char>> &grid);
    void change_player_grid(pair<int, int> &coor, char change);
    void change_opponent_grid(pair<int, int> &coor, char change);
    void get_name();
    void place_ships();
    pair<int, int> shoot();
    bool recieve_shot(pair<int, int> &target_coor);
};

#endif // PLAYER_H
