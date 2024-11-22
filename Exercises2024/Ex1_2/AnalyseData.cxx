#include <iostream>
#include "CustomFunctions.h"

using namespace std;

int main() {
    vector<pair<float, float>> data = readFile("input2D_float.txt");
    vector<pair<float, float>> errors = readFile("error2D_float.txt");
    outputXYPairs(data, 6);
    magnitudeOfAllPairs(data);
    pair<float, float> fit = fitLineSaveAndOutput(data, errors);
    return 0;
}
