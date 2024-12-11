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

void doStuff(FiniteFunction *dist, vector<double> data) {
    dist->integral(1000);
    dist->printInfo();
    dist->plotFunction();
    dist->plotData(data, 100);

    // using a low std-dev is better for sampling, otherwise the peaks are missed in the gaussian and cl dists
    vector<double> sampled_data = dist->createSampledDataPoints(100000, 42, 0.25);
    dist->plotData(sampled_data, 100, false);
}

int main() {
    vector<double> data = loadData("Outputs/data/MysteryData22211.txt");

    // start with the default function
    FiniteFunction invx2(-5, +5, "Outputs/png/InvX2.png");
    doStuff(&invx2, data);

    // gaussian
    NormalDist normal(0.0, 2, -5, +5, "Outputs/png/Gauss.png");
    doStuff(&normal, data);

    // cauchy lorentz
    CauchyLorentzDist cldist(0.0, 2.25, -5, +5, "Outputs/png/CauchyLorentz.png");
    doStuff(&cldist, data);

    // crystal ball
    // this seems to be the distribution that this is sampled from
    CrystalBallDist crystalball(0.0, -2, 1.15, 1.25, -10, +10, "Outputs/png/CrystalBall.png");
    doStuff(&crystalball, data);
}