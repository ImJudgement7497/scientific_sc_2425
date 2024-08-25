#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

struct Play
{

    string goal;          // goal word for the game
    string unknown;       // Gets replaced with letters as they are guessed (starts as "___")
    vector<char> letters; // Capital Alphabet
    int score = 0;        // Score
};

// ASCI art for hangman
void print_hangman(int stage)
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

// Pass a string by constant reference and make it all lowercase
void to_lower_case(string &str)
{
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
              { return tolower(c); });
}

// Pass a char by reference and make it lowercase
void to_lower_case(char &c)
{
    c = tolower(static_cast<unsigned char>(c));
}

// Pass by constant reference
void print_vector(const vector<int> &v)
{
    for (int num : v)
    {
        cout << num << ' ';
    }
    cout << endl;
}

// Pass by constant reference
void print_vector(const vector<char> &v)
{
    for (char n : v)
    {
        cout << n << ' ';
    }
    cout << endl;
}

// Pass by reference so we modify the original structure, not a copy
void get_random_word(Play &current_play)
{

    // Load file into a vector called lines
    vector<string> lines;
    string line;
    ifstream inputFile("1000-most-common-words.txt");

    lines.reserve(1000);

    if (inputFile)
    {
        while (getline(inputFile, line))
        {
            lines.push_back(line);
        }
    }
    else
    {
        cout << "File could not be opened" << endl;
    }

    // for (const string& line : lines) {
    //     cout << line << endl;
    // }

    // Choose a random index for word list
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, lines.size() - 1);

    current_play.goal = lines[dis(gen)];
}

// Ask for a user input and modify original structure
void get_string_from_user(Play &current_play)
{

    cout << "Enter a phrase: ";
    getline(cin >> ws, current_play.goal);
}

// Initalise the guess with "____"
int initalise_guess(Play &current_play)
{

    int length_change = 0;

    for (int i = 0; i < current_play.goal.size(); i++)
    {
        if (current_play.goal[i] == ' ')
        {

            current_play.unknown = current_play.unknown + " ";
            length_change++;
        }

        else
        {

            current_play.unknown = current_play.unknown + "_";
        }
    }
    return length_change;
}

// Initalise the alphabet
void initialise_alphabet(Play &current_play)
{

    current_play.letters.clear();
    // Add uppercase alphabet letters to the vector
    for (char letter = 'a'; letter <= 'z'; ++letter)
    {
        current_play.letters.push_back(letter);
    }
}

// Inistalise the goal string through random or user input, and the mapping
void get_inital_info(Play &current_play)
{

    bool stop = true;
    while (stop)
    {

        string start_string;

        cout << "Would you like a random word (Y) or to enter your own phrase (N): " << endl;
        cin >> start_string;

        if (start_string == "N")
        {
            get_string_from_user(current_play);
            to_lower_case(current_play.goal);
            stop = false;
        }
        else if (start_string == "Y")
        {
            get_random_word(current_play);
            stop = false;
        }
        else
        {
            cout << "Error: Please answer Y or N" << endl;
        }
    }
}

// Get guess from user and assign it to the reference of a pointer
char *get_guesses(char *guess_ptr)
{

    guess_ptr = new char; // Allocate memory for a new string
    cout << "Please enter your guess: " << endl;
    cin >> *guess_ptr; // Assign the user inputted string to the reference of the pointer
    to_lower_case(*guess_ptr);

    return guess_ptr;
}

// Function to check guess
vector<int> *check_guess(Play &current_play, const char *guess_ptr)
{
    char current_guess = *guess_ptr;
    vector<int> *indice_ptr = new vector<int>;

    for (int i = 0; i < current_play.goal.size(); i++)
    {
        if (current_guess == current_play.goal[i])
        {
            indice_ptr->push_back(i);
        }
    }

    if (indice_ptr->empty())
    {
        delete indice_ptr;
        return nullptr;
    }

    return indice_ptr;
}

// Change the letters in unknown to the guessed letter through the indice pointer
int change_unknown(Play &current_play, char *guess_ptr, vector<int> *indice_ptr)
{

    int score = 0;

    for (int i : *indice_ptr)
    {
        current_play.unknown[i] = *guess_ptr;
        score++;
    }

    return score;
}

// Change letters in alphabet to empty character
void remove_letters(Play &current_play, char *guess_ptr)
{

    for (int i = 0; i < current_play.letters.size(); i++)
    {
        if (current_play.letters[i] == *guess_ptr)
        {
            current_play.letters[i] = ' ';
        }
    }
}
// Function to start the game
void start_game()
{
    Play play;
    cout << "WELCOME TO HANGMAN" << endl;

    get_inital_info(play);
    initialise_alphabet(play);
    int length_change = initalise_guess(play);

    bool stop = true;
    int life = 0;
    char *guess_ptr = nullptr; // Create a new pointer
    vector<int> *indice_ptr = nullptr;

    while (stop)
    {
        delete guess_ptr; // Clean up previous memory allocation (use delete[] for arrays)
        delete indice_ptr;

        if (life >= 7)
        {
            cout << "YOU HAVE LOST! The word was: " << play.goal << endl;
            break;
        }

        else if (play.score >= (play.goal.size() - length_change))
        {
            cout << "YOU HAVE WON! The word was: " << play.goal << endl;
            break;
        }
        else
        {
            print_vector(play.letters);
            print_hangman(life);
            cout << "Word/Phrase: " << play.unknown << endl;
            guess_ptr = get_guesses(guess_ptr); // Create new pointer pointing to guess string

            indice_ptr = check_guess(play, guess_ptr);

            if (indice_ptr)
            {
                int score = change_unknown(play, guess_ptr, indice_ptr); // No change in life
                play.score = play.score + score;
            }
            else
            { // Change in life
                delete indice_ptr;
                life++;
            }

            remove_letters(play, guess_ptr);
        }
    }

    // delete guess_ptr; // Clean up guess_ptr
    cout << "Thanks for playing!" << endl;
}

int main()
{

    start_game();
    // THIS IS  A TEST coMMIT

    return 0;
}

// int main() {
//     int stage = 0;

//     // Display each stage for demonstration
//     for (stage = 0; stage <= 6; ++stage) {
//         print_hangman(stage);
//         cout << endl;
//         // Simulate delay or user input here if desired
//     }

//     return 0;
// }
