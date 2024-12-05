#include <iostream>
#include <fstream>

#include "FiniteFunctions.h"

using namespace std;

vector<double> loadData(string filename) {
    ifstream filestream;
    filestream.open(filename);
    if (filestream.fail()) {
        cout << "File IO Failed" << endl;
        exit(1);
    }

    vector<double> data;
    string line;
    while (getline(filestream, line)) {
        double x = stod(line);
        data.push_back(x);
    }
    filestream.close();

    return data;
} 

int main() {
    CrystalBallDist func(0, 1, 1, 2, -5, +5, "Outputs/png/CBTest.png");
    func.plotFunction();
    
    vector<double> dataPtr = loadData("Outputs/data/MysteryData22211.txt");
    func.plotData(dataPtr, 100);
}