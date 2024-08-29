#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

using namespace std;

struct Play
{
    string goal;          // Goal word for the game
    string unknown;       // Gets replaced with letters as they are guessed (starts as "___")
    vector<char> letters; // Alphabet available for guessing
    int score = 0;        // Score
};

// ASCII art for hangman
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

// Pass a string by reference and make it all lowercase
void to_lower_case(string &str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

// Pass a char by reference and make it lowercase
void to_lower_case(char &c)
{
    c = tolower(static_cast<unsigned char>(c));
}

// Print vector of integers
void print_vector(const vector<int> &v)
{
    for (int num : v)
    {
        cout << num << ' ';
    }
    cout << endl;
}

// Print vector of characters
void print_vector(const vector<char> &v)
{
    for (char n : v)
    {
        cout << n << ' ';
    }
    cout << endl;
}

// Get a random word from the file
void get_random_word(Play &current_play)
{
    vector<string> lines;
    string line;
    ifstream inputFile("1000-most-common-words.txt");

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
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, lines.size() - 1);

    current_play.goal = lines[dis(gen)];
}

// Get a phrase from the user
void get_string_from_user(Play &current_play)
{
    cout << "Enter a phrase: ";
    getline(cin >> ws, current_play.goal); // using ws to ignore leading whitespace
}

// Initialize the guess with "____"
int initialize_guess(Play &current_play)
{
    int length_change = 0;
    current_play.unknown = current_play.goal; // Initialize unknown directly

    for (char &ch : current_play.unknown)
    {
        if (ch == ' ')
        {
            length_change++;
        }
        else
        {
            ch = '_';
        }
    }
    return length_change;
}

// Initialize the alphabet
void initialize_alphabet(Play &current_play)
{
    current_play.letters.clear();
    for (char letter = 'a'; letter <= 'z'; ++letter)
    {
        current_play.letters.push_back(letter);
    }
}

// Get initial info from the user or generate it randomly
void get_initial_info(Play &current_play)
{
    while (true)
    {
        string start_string;
        cout << "Would you like a random word (Y) or to enter your own phrase (N): ";
        cin >> start_string;

        to_lower_case(start_string); // To handle case sensitivity

        if (start_string == "n")
        {
            get_string_from_user(current_play);
            to_lower_case(current_play.goal);
            break;
        }
        else if (start_string == "y")
        {
            get_random_word(current_play);
            break;
        }
        else
        {
            cout << "Error: Please answer Y or N" << endl;
        }
    }
}

// Get guess from user and return it by value
char get_guess()
{
    char guess;
    cout << "Please enter your guess: ";
    cin >> guess;
    to_lower_case(guess);
    return guess;
}

// Check if the guess is in the goal word and return the indices
vector<int> check_guess(const Play &current_play, char guess)
{
    vector<int> indices;
    for (int i = 0; i < current_play.goal.size(); ++i)
    {
        if (guess == current_play.goal[i])
        {
            indices.push_back(i);
        }
    }
    return indices;
}

// Update the unknown string with correct guesses
int update_unknown(Play &current_play, char guess, const vector<int> &indices)
{
    for (int index : indices)
    {
        current_play.unknown[index] = guess;
    }
    return indices.size();
}

// Remove a guessed letter from the available letters
void remove_letter(Play &current_play, char guess)
{
    auto it = find(current_play.letters.begin(), current_play.letters.end(), guess);
    if (it != current_play.letters.end())
    {
        *it = ' '; // Replace with a space to indicate it's been used
    }
}

// Start the hangman game
void start_game()
{
    Play play;
    cout << "WELCOME TO HANGMAN" << endl;

    get_initial_info(play);
    initialize_alphabet(play);
    int length_change = initialize_guess(play);

    int life = 0;

    while (true)
    {
        if (life >= 6) // Only 6 stages are defined, not 7
        {
            cout << "YOU HAVE LOST! The word was: " << play.goal << endl;
            break;
        }

        if (play.score >= (play.goal.size() - length_change))
        {
            cout << "YOU HAVE WON! The word was: " << play.goal << endl;
            break;
        }

        print_vector(play.letters);
        print_hangman(life);
        cout << "Word/Phrase: " << play.unknown << endl;

        char guess = get_guess();
        vector<int> indices = check_guess(play, guess);

        if (!indices.empty())
        {
            play.score += update_unknown(play, guess, indices); // Update score based on correct guesses
        }
        else
        {
            life++; // Incorrect guess, increase life (i.e., progress in the hangman)
        }

        remove_letter(play, guess); // Remove guessed letter from the alphabet
    }

    cout << "Thanks for playing!" << endl;
}

int main()
{
    start_game();
    return 0;
}
