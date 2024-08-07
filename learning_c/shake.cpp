#include <iostream>
#include <random>

using namespace std;

string genRandomString(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(26, 52);

    char randLetter;
    string testString;

    for (int i = 0; i < 28; i++) {

        int randIndex = dis(gen);

        if (randIndex < 52) {
            randLetter = 'a' + (randIndex - 26);
        }
        else {
            randLetter = ' ';
        }
        testString = testString + randLetter;
        
    }

    return testString;

}

int compareStrings(string test){
    string goal = "methinks it is like a weasel";
    int score = 0;

    for (int i = 0; i < test.size(); i++){

        if (test[i] == goal[i]){
            score = score + 1;
        }
    }
    return score;
}
int main(){
    
    bool stop = true;
    int i = 0;

    while (stop){
        string test = genRandomString();
        int score = compareStrings(test);

        if (score == 28){
            stop = false;
        }
        else if (i == 1000000){
            stop = false;
        }
        else if (i % 1000 == 0 || i == 0){
            cout << "Try " << i << ' ' << "Score " << score << endl;

        }

        i = i + 1;


    }

    return 0;
}