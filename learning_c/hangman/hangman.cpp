#include <iostream>
#include <random>
#include <vector>
#include <fstream>


using namespace std;
/*
Need a class with methods that print out the different lives of hangman
Need a function that picks the random word
*/

struct Play {

    string goal; // goal word for the game
    vector<char> guess; // User guess (starts as "___")
    vector<char> letters; // Capital Alphabet

};

// Pass by constant reference
void print_vector(const vector<int>& v) {
    for (int num : v) {
        cout << num << ' ';
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
    string goal;

    cout << "Enter a string: ";
    getline(cin, goal);

    current_play.goal = goal;
}

int main() {

    Play play;
    play.goal = "TEST";
    cout << play.goal << endl;

    get_random_word(play);

    cout << play.goal << endl;
    return 0;
}
