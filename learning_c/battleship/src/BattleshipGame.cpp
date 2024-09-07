#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <thread>
#include <chrono>
#include "Player.h"

using namespace std;

class BattleshipGame
{
private:
    // Clears the terminal completly
    void clear_terminal(string message, int delay)
    {
        cout << message << endl;
        delay_function(delay);
        system("clear");
    }

    // Prints the Battleship ACSI art
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

    void delay_function(int delay)
    {
        this_thread::sleep_for(std::chrono::seconds(delay));
    }

    void print_dash_lines()
    {
        cout << "--------------------------------------------------------------------------------------------" << endl;
    }

    void print_turn_info(int &current, int &opponent)
    {
        print_dash_lines();
        cout << "------------------------------OPPONENT GRID------------------------------" << endl;
        pl_vec[current].print_opponent_grid();
        print_dash_lines();
        cout << "------------------------------YOUR GRID------------------------------" << endl;
        pl_vec[current].print_player_grid();
        print_dash_lines();
    }

    void initalise_player_vector()
    {
        Player player_1;
        Player player_2;

        pl_vec = {player_1, player_2};
    }

    void initalise_all()
    {
        print_battleship_title();
        initalise_player_vector();
        cout << "Player 1" << endl;
        pl_vec[0].get_name();

        cout << "Player 2" << endl;
        pl_vec[1].get_name();
        clear_terminal("Loading", 2);
        print_battleship_title();

        pl_vec[0].place_ships();
        clear_terminal("Loading", 2);
        pl_vec[1].place_ships();
        clear_terminal("Loading", 2);
    }

    void play_turn()
    {

        bool is_turn_over = true;
        int current;
        int opponent;

        // whose turn is it?
        if (player_tracker)
        {
            // player 1's turn, so does the shooting and player 2 does the reciving
            current = 0;
            opponent = 1;
        }
        else
        {
            // player 2's turn, so does the shooting and player 1 does the reciving
            current = 1;
            opponent = 0;
        }

        cout << "Loading " << pl_vec[current].name << "'s turn!" << endl;
        clear_terminal("", 5);
        /*
        -Display current players grid and opponent grid
        -Shoot
        -If hit, mark H on reciving players grid and on current players opponent grid and continue turn
        -If miss, mark M on reciving players grid and on current players opponenet grid and end turn*/
        print_dash_lines();
        print_battleship_title();
        print_dash_lines();

        while (is_turn_over)
        {
            print_turn_info(current, opponent);

            // Shoot then recieve then decide what happens after

            pair<int, int> target = pl_vec[current].shoot();
            is_turn_over = pl_vec[opponent].recieve_shot(target);
            if (is_turn_over)
            {
                cout << pl_vec[current].name << ", you hit " << pl_vec[opponent].name << "'s ship at (" << target.first << "," << target.second << ")!" << endl;
                pl_vec[current].change_opponent_grid(target, 'H');
                delay_function(3);
            }
            else
            {
                cout << pl_vec[current].name << ", you missed at (" << target.first << "," << target.second << ")!" << endl;
                pl_vec[current].change_opponent_grid(target, 'M');
                delay_function(3);
            }

            if (is_game_over())
            {
                cout << "Congratulations " << pl_vec[current].name << ", you have won!" << endl;
                game_over = true;
                break;
            }
        }
    }

    bool is_game_over()
    {
        int player;
        int ships_destroyed = 0;

        if (player_tracker)
        {
            // if pllayer 1's turn, it needs to be player 2's ship checked if destroyed
            player = 1;
        }
        else
        {
            player = 0;
        }

        for (const auto &ship : pl_vec[player].ships_vector)
        {
            if (ship.is_sunk())
            {
                ships_destroyed = ships_destroyed + 1;
            }

            if (ships_destroyed == 5)
            {
                return true;
            }

            else
            {
                continue;
            }
        }

        return false;
    }

public:
    // Player player_1;
    // Player player_2;
    vector<Player> pl_vec;
    bool player_tracker = true; // PLayer 1 is true, player 2 is false
    bool game_over = false;

    void start_game()
    {
        clear_terminal("", 1);
        initalise_all();
        while (!game_over)
        {
            play_turn();
            player_tracker = !player_tracker;
        }
    }
};

int main()
{

    BattleshipGame game;
    game.start_game();

    return 0;
}