#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "Analyse-Data.h"

using namespace std;

int main() {
    vector<vector<float>> pairs = readFile("input2D_float.txt");
    outputXYPairs(pairs, 6);
    magnitudeOfAllPairs(pairs);
    return 0;
}

vector<vector<float>> readFile(string filename) {
    // open the file
    ifstream filestream;
    filestream.open(filename);
    if (filestream.fail()) {
        cout << "File IO Failed";
        exit(1);
    }

    vector<vector<float>> xy_pairs;

    // read the file line by line
    string line;
    getline(filestream, line); // skip the first line "x,y"
    while (getline(filestream, line, ',')) {
        // first part of the line was before the delimeter
        float x = stof(line);

        // rest of line from the delimeter to the end of the line
        getline(filestream, line);
        float y = stof(line);

        // store each pair
        vector<float> pair;
        pair.push_back(x);
        pair.push_back(y);
        xy_pairs.push_back(pair);
    }
    filestream.close();

    return xy_pairs;
}

void outputXYPairs(vector<vector<float>> pairs, int npairs) {
    if (npairs > pairs.size()) {
        cout << "WARNING: You have requested more lines to be output than exist." << endl;
        cout << "Outputting the first 5 lines..." << endl;
        npairs = 5;
    }

    for (int i = 0; i < npairs; i++) {
        vector<float> pair = pairs[i];
        cout << pair[0] << ", " << pair[1] << endl;
    }
}

float calculate_magnitude(vector<float> xyPair) {
    float x = xyPair[0];
    float y = xyPair[1];
    float mag = sqrt(x*x + y*y);
    return mag;
}

vector<float> magnitudeOfAllPairs(vector<vector<float>> pairs) {
    vector<float> magnitudes;
    cout << "Magnitudes:" << endl;
    for (vector<float> pair : pairs) {
        float mag = calculate_magnitude(pair);
        magnitudes.push_back(mag);
        cout << mag << endl;
    }
    return magnitudes;
}
