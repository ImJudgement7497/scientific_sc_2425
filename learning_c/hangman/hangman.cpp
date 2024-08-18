#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <algorithm> 
#include <cctype>    

using namespace std;

struct Play {

    string goal; // goal word for the game
    string unknown; // Gets replaced with letters as they are guessed (starts as "___")
    vector<char> letters; // Capital Alphabet

};

// ASCI art for hangman
void print_hangman(int stage) {
    switch (stage) {
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
void to_lower_case(string& str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return tolower(c);
    });
}

// Pass by constant reference
void print_vector(const vector<int>& v) {
    for (int num : v) {
        cout << num << ' ';
    }
    cout << endl;
}

// Pass by constant reference
void print_vector(const vector<char>& v) {
    for (char n : v) {
        cout << n << ' ';
    }
    cout << endl;
}

// Pass by reference so we modify the original structure, not a copy
void get_random_word(Play& current_play) {

    // Load file into a vector called lines 
    vector<string> lines;
    string line;
    ifstream inputFile("1000-most-common-words.txt");

    lines.reserve(1000);

    if (inputFile) {
        while (getline(inputFile, line)) {
            lines.push_back(line);
        }
    }
    else {
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
void get_string_from_user(Play& current_play) {

    cout << "Enter a phrase: ";
    getline(cin >> ws, current_play.goal);

}

// Initalise the guess with "____"
void initalise_guess(Play& current_play) {

    int goal_length = current_play.goal.size();
    for (int i = 0; i < goal_length; i++) {

        current_play.unknown = current_play.unknown + "_";

    }
}

// Initalise the alphabet
void initialise_alphabet(Play& current_play) {

    current_play.letters.clear();
    // Add uppercase alphabet letters to the vector
    for (char letter = 'a'; letter <= 'z'; ++letter) {
        current_play.letters.push_back(letter);
    }
}

void get_inital_info(Play& current_play) {

    bool stop = true;
    while (stop) {

        string start_string;

        cout << "Would you like a random word or to enter your own phrase (Y/N): " << endl;
        cin >> start_string;

        if (start_string == "Y") {
            get_string_from_user(current_play);
            to_lower_case(current_play.goal);
            stop = false;
        }
        else if (start_string == "N") {
            get_random_word(current_play);
            stop = false;
        }
        else {
            cout << "Error: Please answer Y or N" << endl;
        }
    }
}

void check_guess(Play& current_play, const string* guess_ptr) {

    string current_guess = *guess_ptr;
}

string* get_guesses(string* guess_ptr){

    guess_ptr = new string; // Allocate memory for a new string
    cout << "Please enter your guess: " << endl;
    cin >> *guess_ptr; // Assign the user inputted string to the reference of the pointer
    to_lower_case(*guess_ptr);
    cout << "Your guess was: " << *guess_ptr << endl; 

    return guess_ptr;
}

void start_game() {

    Play play;
    cout << "WELCOME TO HANGMAN" << endl;

    get_inital_info(play);

    initialise_alphabet(play);
    initalise_guess(play);
    print_vector(play.letters);

    bool stop = true;
    int life = 0;
    string* guess_ptr = nullptr; // Create a new pointer

    while (stop) {
        delete guess_ptr; // Clean up previous memory allocation
        print_hangman(life);
        cout << "Word/Phrase: " << play.unknown << endl;
        guess_ptr = get_guesses(guess_ptr); // Create new pointer pointing to guess string

    }

}

int main() {

    start_game();

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