#include "BattleshipGame.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void BattleshipGame::clear_terminal(string message, int delay)
{
    cout << message << endl;
    delay_function(delay);
    system("clear");
}

void BattleshipGame::print_battleship_title()
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

void BattleshipGame::delay_function(int delay)
{
    this_thread::sleep_for(std::chrono::seconds(delay));
}

void BattleshipGame::print_dash_lines()
{
    cout << "--------------------------------------------------------------------------------------------" << endl;
}

void BattleshipGame::print_turn_info(int &current, int &opponent)
{
    print_dash_lines();
    cout << "------------------------------OPPONENT GRID------------------------------" << endl;
    pl_vec[current].print_opponent_grid();
    print_dash_lines();
    cout << "------------------------------YOUR GRID------------------------------" << endl;
    pl_vec[current].print_player_grid();
    print_dash_lines();
}

void BattleshipGame::initalise_player_vector()
{
    Player player_1;
    Player player_2;

    pl_vec = {player_1, player_2};
}

void BattleshipGame::initalise_all()
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

bool BattleshipGame::is_ship_destroyed(int &opponent)
{
    for (const auto &ship : pl_vec[opponent].ships_vector)
    {
        if (ship.is_sunk())
        {
            cout << pl_vec[opponent].name << "'s " << ship.name << " has been destroyed!" << endl;
            return true;
        }
    }

    return false;
}

void BattleshipGame::play_turn()
{
    bool is_turn_over = true;
    int current;
    int opponent;

    // Determine whose turn it is
    if (player_tracker)
    {
        current = 0;  // player 1's turn
        opponent = 1; // player 2 is the opponent
    }
    else
    {
        current = 1;  // player 2's turn
        opponent = 0; // player 1 is the opponent
    }

    cout << "Loading " << pl_vec[current].name << "'s turn!" << endl;
    clear_terminal("", 5);

    print_dash_lines();
    print_battleship_title();
    print_dash_lines();

    while (is_turn_over)
    {
        print_turn_info(current, opponent);

        // Shoot, then receive shot, and decide what happens after
        pair<int, int> target = pl_vec[current].shoot();
        is_turn_over = pl_vec[opponent].recieve_shot(target);

        if (is_turn_over)
        {
            cout << pl_vec[current].name << ", you hit " << pl_vec[opponent].name << "'s ship at (" << target.first << "," << target.second << ")!" << endl;
            pl_vec[current].change_opponent_grid(target, 'H');
            delay_function(3);
            if (is_ship_destroyed(opponent))
            {
                delay_function(3);
                break;
            }
        }
        else
        {
            cout << pl_vec[current].name << ", you missed at (" << target.first << "," << target.second << ")!" << endl;
            pl_vec[current].change_opponent_grid(target, 'M');
            // delay_function(3);
        }

        if (is_game_over())
        {
            clear_terminal("", 1);
            print_battleship_title();
            for (int i = 0; i < 10; i++)
            {

                cout << "Congratulations " << pl_vec[current].name << ", you have won!" << endl;
            }

            game_over = true;
            break;
        }
    }
}

bool BattleshipGame::is_game_over()
{
    int player;
    int ships_destroyed = 0;

    if (player_tracker)
    {
        player = 1; // if it's player 1's turn, check player 2's ships
    }
    else
    {
        player = 0; // if it's player 2's turn, check player 1's ships
    }

    for (const auto &ship : pl_vec[player].ships_vector)
    {
        if (ship.is_sunk())
        {
            ships_destroyed++;
        }

        if (ships_destroyed == 5)
        {
            return true;
        }
    }

    return false;
}

void BattleshipGame::start_game()
{
    clear_terminal("", 1);
    initalise_all();

    while (!game_over)
    {
        play_turn();
        player_tracker = !player_tracker; // switch turns between players
    }
}