#include <iostream>
#include "CustomFunctions.h"

using namespace std;

int main() {
    vector<pair<float, float>> data = readFile("input2D_float.txt");
    outputXYPairs(data, 6);
    magnitudeOfAllPairs(data);
    fitLineSaveAndOutput(data);
    return 0;
}
