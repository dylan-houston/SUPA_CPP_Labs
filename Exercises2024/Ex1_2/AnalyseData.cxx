#include <iostream>
#include "CustomFunctions.h"

using namespace std;

bool userRequestFurtherInput();
void run();

int main() {
    run();
    return 0;
}

void run() {
    bool anotherInput = true;

    vector<pair<float, float>> data = readFile("input2D_float.txt");
    vector<pair<float, float>> errors = readFile("error2D_float.txt");
    
    while (anotherInput) {
        string choice;
        int option;
        print("--- Menu ---");
        print("1. Print n lines the data.");
        print("2. Calculate the magnitude of each vector.");
        print("3. Fit a straight line to the data.");
        print("4. Calculate x^y for each data point.");
        cout << "Enter a choice [1-4]: ";
        cin >> choice;
        option = stoi(choice);

        switch (option) {
        case 1: {
            cout << "How many lines to print? ";
            int lines;
            cin >> choice;
            lines = stoi(choice);
            outputXYPairs(data, lines);
            break;
        }
        case 2: {
            vector<float> results = magnitudeOfAllPairs(data);
            printAndSaveToFile(results, "magnitudes.txt");
            break;
        }
        case 3: {
            fitLineSaveAndOutput(data, errors);
            break;
        }
        case 4: {
            vector<float> results = xPowOfyAllPairs(data);
            printAndSaveToFile(results, "xToPowY.txt");
            break;
        }
        default:
            print("Please select a valid option.");
            continue;
        }

        if (!userRequestFurtherInput()) {
            anotherInput = false;
        }
    }
}

bool userRequestFurtherInput() {
    string inp;
    while (true) {
        cout << "Would you like to perform another action [y/n]? ";
        cin >> inp;
        if (inp == "y") {
            return true;
        } else if (inp == "n") {
            return false;
        } else {
            print("Unrecognised Input, try again.");
        }
    }
}
