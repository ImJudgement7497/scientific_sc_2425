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

    void initalise_player_vector(Player &player_1, Player &player_2)
    {
        pl_vec = {player_1, player_2};
    }

    void initalise_all()
    {
        player_1.initalise_grid();
        cout << "Player 1" << endl;
        player_1.get_name();

        player_2.initalise_grid();
        cout << "Player 2" << endl;
        player_2.get_name();
        clear_terminal("Loading", 2);
        print_battleship_title();

        player_1.place_ships();
        clear_terminal("Loading", 2);
        player_2.place_ships();
        clear_terminal("Loading", 2);

        initalise_player_vector(player_1, player_2);
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
        }
    }

public:
    Player player_1;
    Player player_2;
    vector<Player> pl_vec;
    bool player_tracker = true; // PLayer 1 is true, player 2 is false

    void start_game()
    {   
        clear_terminal("", 1);
        initalise_all();
        while (true)
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
