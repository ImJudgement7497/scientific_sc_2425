#include <iostream>
#include <random>
#include <vector>

using namespace std;

void printVector(const std::vector<int>& v) {
    for (int num : v) {
        std::cout << num << ' ';
    }
    std::cout << std::endl;
}

struct Result {
    vector<int> index_list;
    int score;
};

char genRandomChar(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 52);

    char randLetter;

    int randIndex = dis(gen);

    if (randIndex < 26){
        randLetter = 'A' + randIndex;
    }

    else if (randIndex < 52) {
        randLetter = 'a' + (randIndex - 26);
    }
    else {
        randLetter = ' ';
    }

    return randLetter;
}

string genRandomString(int length){

    string randString;

    for (int i = 0; i < length; i++){

        randString = randString + genRandomChar();
    }

    return randString;

}

Result compareStrings(string test, string goal){
    int score = 0;
    vector<int> v;
    Result result;

    for (int i = 0; i < test.size(); i++){

        if (test[i] == goal[i]){
            score = score + 1;
        
        }
        else {
            v.push_back(i);
        }
    }
    result.index_list = v;
    result.score = score;

    return result;
}

string changeCharInString(string test, Result result){

    for (int index : result.index_list){
        test[index] = genRandomChar();
    }
    return test;
}

int main() {
    bool stop = true;
    int i = 0;
    string goal;

    cout << "Enter a string: ";
    getline(cin, goal);
    int goal_length = goal.size();
    string test = genRandomString(goal_length);

    while (stop) {
        Result result = compareStrings(test, goal);
        if (result.score == goal_length) {
            stop = false;
        }
        else if (i == 3000) {
            stop = false;
        }
        else {
            test = changeCharInString(test, result);
        }
        
        cout << "Try " << i << ' ' << "Score " << result.score << endl;
        cout << test << endl;
        i = i + 1;
    }

    return 0;
}