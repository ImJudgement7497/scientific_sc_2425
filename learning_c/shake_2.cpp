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

double getMeanVector(const vector<int>& v) {
    int sum = 0;
    int length = v.size();

    for (int num : v) {

        sum = sum + num;
    }

    double mean = static_cast<double>(sum) / length;

    return mean;

}

double findMaxElement(const vector<double>& v) {

    double maxElement = numeric_limits<double>::lowest();

    for (double num : v) {
        if (num > maxElement) {
            maxElement = num;
        }
    }

    return maxElement;
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

string getString() {
    string goal;

    cout << "Enter a string: ";
    getline(cin, goal);

    return goal;

}
int runTest(string goal) {
    bool stop = true;
    int i = 0;

    // string goal = getString();
    int goal_length = goal.size();
    
    string test = genRandomString(goal_length);

    Result* resultPtr = nullptr;

    while (stop) {
        delete resultPtr; // Clean up previous allocation
        resultPtr = new Result(compareStrings(test, goal));
        
        if (i == 0) {
            cout << "Try " << i << ' ' << "Score " << resultPtr->score << endl;
            cout << test << std::endl;
        }
        else if (resultPtr->score == goal_length) {
            stop = false;
        }
        else if (i == 3000) {
            stop = false;
        }
        else {
            test = changeCharInString(test, *resultPtr);
        }
        
        i = i + 1;
    }

    cout << "Try " << i << ' ' << "Score " << resultPtr->score << endl;
    cout << test << endl;

    delete resultPtr; // Clean up final allocation
    return i;
}

double run() {

    vector<int> tries;

    for (int i = 0; i < 10; i++) {
        int num = runTest("Hello I am Ben");
        cout << "---------------------------" << endl;
        tries.push_back(num);
    }

    double mean = getMeanVector(tries);
    printVector(tries);
    cout << "Mean Number of Tries: " << mean << endl;

    return mean;
}

int main() {

    vector<double> means;

    for (int i = 0; i < 500; i++) {

        means.push_back(run());
    }

cout << "Maximum mean number of tries: " << findMaxElement(means) << endl;
    

    return 0;
}