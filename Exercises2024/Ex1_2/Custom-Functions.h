#pragma once
#include <vector>
#include <string>

using namespace std;

vector<vector<float>> readFile(string filename);
void outputXYPairs(vector<vector<float>> pairs, int npairs);
float calculate_magnitude(vector<float> xyPair);
vector<float> magnitudeOfAllPairs(vector<vector<float>> pairs);
