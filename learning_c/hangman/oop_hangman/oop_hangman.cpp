#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

class Hangman
{

private:
    string goal;          // goal word for the game
    string unknown;       // Gets replaced with letters as they are guessed (starts as "___")
    vector<char> letters; // Capital Alphabet
    int score = 0;        // Score

    void to_lower_case(string &str)
    {
        transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                  { return tolower(c); });
    }

    // Pass a char by reference and make it lowercase
    void const to_lower_case(char &c)
    {
        c = tolower(static_cast<unsigned char>(c));
    }

public:
    // Constructor

    Hangman(string goal_string)
    {

        goal = goal_string;
    }

    // ASCI art for hangman
    void const print_hangman(int stage)
    {
        switch (stage)
        {
        case 0:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        case 1:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "   O     |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        case 2:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "   O     |\n";
            cout << "   |     |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        case 3:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "   O     |\n";
            cout << "  /|     |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        case 4:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "   O     |\n";
            cout << "  /|\\    |\n";
            cout << "         |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        case 5:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "   O     |\n";
            cout << "  /|\\    |\n";
            cout << "  /      |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        case 6:
            cout << "   -------\n";
            cout << "   |     |\n";
            cout << "   O     |\n";
            cout << "  /|\\    |\n";
            cout << "  / \\    |\n";
            cout << "         |\n";
            cout << "----------\n";
            break;
        default:
            cout << "Invalid stage\n";
        }
    }
};

int main()
{

    return 0;
}