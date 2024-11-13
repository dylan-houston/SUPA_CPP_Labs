#pragma once
#include <vector>
#include <string>

std::vector<std::vector<float>> readFile(std::string filename);
void outputXYPairs(std::vector<std::vector<float>> pairs, int npairs);
float calculate_magnitude(std::vector<float> xyPair);
std::vector<float> magnitudeOfAllPairs(std::vector<std::vector<float>> pairs);
