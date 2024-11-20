#pragma once
#include <vector>
#include <string>

using namespace std;

vector<pair<float, float>> readFile(string);
void outputXYPairs(vector<pair<float, float>>, int);
float calculate_magnitude(vector<float>);
vector<float> magnitudeOfAllPairs(vector<pair<float, float>>);
pair<float, float> fitStraightLine(vector<pair<float, float>>);
void fitLineSaveAndOutput(vector<pair<float, float>>);
