#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

vector<pair<float, float>> readFile(string);
void outputXYPairs(vector<pair<float, float>>, int);
float calculate_magnitude(vector<float>);
vector<float> magnitudeOfAllPairs(vector<pair<float, float>>);
pair<float, float> fitStraightLine(vector<pair<float, float>>);
void fitLineSaveAndOutput(vector<pair<float, float>>);
float chi2(vector<pair<float, float>>, vector<pair<float, float>>, pair<float, float>);

void print(string);


// print functions
template<typename T>
void print(vector<pair<T, T>> data){
    for (pair<T, T> xy_pair : data) {
        cout << xy_pair.first << ", " << xy_pair.second << endl;
    }
}

template<typename T>
void print(vector<T> data){
    for (T point : data) {
        cout << point << endl;
    }
}
