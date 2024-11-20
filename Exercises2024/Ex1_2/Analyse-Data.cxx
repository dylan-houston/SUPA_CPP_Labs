#include <iostream>
#include "Analyse-Data.h"
#include "Custom-Functions.h"

using namespace std;

int main() {
    vector<vector<float>> pairs = readFile("input2D_float.txt");
    outputXYPairs(pairs, 6);
    magnitudeOfAllPairs(pairs);
    return 0;
}
