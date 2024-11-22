#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "CustomFunctions.h"

using namespace std;


void print(string s) {
    cout << s << endl;
}

vector<pair<float, float>> readFile(string filename) {
    // open the file
    ifstream filestream;
    filestream.open(filename);
    if (filestream.fail()) {
        print("File IO Failed");
        exit(1);
    }

    vector<pair<float, float>> xy_pairs;

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
        pair<float, float> xy_pair;
        xy_pair.first = x;
        xy_pair.second = y;

        xy_pairs.push_back(xy_pair);
    }
    filestream.close();

    return xy_pairs;
}

void outputXYPairs(vector<pair<float, float>> data, int npairs) {
    if (npairs > data.size()) {
        print("WARNING: You have requested more lines to be output than exist.");
        print("Outputting the first 5 lines...");
        npairs = 5;
    }

    vector<pair<float, float>> sub_data(data.begin(), data.begin() + npairs);
    print(sub_data);
}

float calculate_magnitude(pair<float, float> xyPair) {
    float x = xyPair.first;
    float y = xyPair.second;
    float mag = sqrt(x*x + y*y);
    return mag;
}

vector<float> magnitudeOfAllPairs(vector<pair<float, float>> pairs) {
    vector<float> magnitudes;
    for (pair<float, float> xy_pair : pairs) {
        float mag = calculate_magnitude(xy_pair);
        magnitudes.push_back(mag);
    }
    return magnitudes;
}

pair<float, float> fitStraightLine(vector<pair<float, float>> data) {
    float sum_x = 0;
    float sum_x2 = 0;
    float sum_y = 0;
    float sum_xy = 0;
    for (pair<float, float> xy_pair : data) {
        sum_x += xy_pair.first;
        sum_y += xy_pair.second;
        sum_x2 += xy_pair.first * xy_pair.first;
        sum_xy += xy_pair.first * xy_pair.second;
    }

    int N = data.size();

    float denom = N * sum_x2 - sum_x * sum_x;

    pair<float, float> pq_pair;
    pq_pair.first = (N * sum_xy - sum_x * sum_y) / denom;
    pq_pair.second = (sum_x2 * sum_y - sum_xy * sum_x) / denom;

    return pq_pair;
}

float chi2NDF(vector<pair<float, float>> data, vector<pair<float, float>> errors, pair<float, float> fit_pq) {
    float chi2 = 0;
    for (int i=0; i < data.size(); i++) {
        float prediction = data[i].first * fit_pq.first + fit_pq.second;
        float chi2_i = pow(data[i].second - prediction, 2);
        // Jonathan told me this is the error
        float error = errors[i].second + prediction * errors[i].first;
        chi2_i /= pow(error, 2);
        chi2 += chi2_i;
    }

    // NDF is the number of rows in the data
    int NDF = data.size();

    return chi2/NDF;
}

pair<float, float> fitLineSaveAndOutput(vector<pair<float, float>> data, vector<pair<float, float>> errors) {
    pair<float, float> pq = fitStraightLine(data);
    float chi2NDF_res = chi2NDF(data, errors, pq);
    printAndSaveToFile(pq, chi2NDF_res, "fit-results.txt");
    return pq;
}

vector<float> xPowOfyAllPairs(vector<pair<float, float>> pairs) {
    vector<float> results;
    for (pair<float, float> xy_pair : pairs) {
        int int_y = round(xy_pair.second);
        float res = xPowOfy(xy_pair.first, int_y);
        results.push_back(res);
    }
    return results;
}

float xPowOfy(float x, int y) {
    if (y == 0) {
        return 1.0;
    } else if (y >= 1) {
        return x * xPowOfy(x, y - 1);
    } else {
        return (1/x) * xPowOfy(x, y + 1);
    }
}

void printAndSaveToFile(vector<float> data, string filename) {
    print(data);

    ofstream outStream;
    outStream.open(filename);
    if (!outStream.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    print("Writing to file...");
    for (float point : data) {
        outStream << point << endl;
    }
    outStream.close();
}

template<typename T>
void printAndSaveToFile(pair<T, T> fit_data, float chi2NDF, string filename) {
    printf("y = %f * x + %f\n", fit_data.first, fit_data.second);
    printf("chi^2/NDF = %f\n", chi2NDF);

    ofstream outStream;
    outStream.open(filename);
    if (!outStream.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    print("Writing to file...");
    outStream << "y = " << fit_data.first << " * x + " << fit_data.second << endl;
    outStream << "chi^2/NDF = " << chi2NDF << endl;
    outStream.close();
}
