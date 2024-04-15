//
// Created by Lorenzo on 14/04/24.
//

#ifndef ORDERING_UTILS_H
#define ORDERING_UTILS_H

#include <iostream>
#include <algorithm> // for shuffle
#include <numeric>   // for iota

using namespace std;

vector<double> generate_random_vector(int num_values) {
    vector<double> vec(num_values);

    iota(vec.begin(), vec.end(), 0);

    for (double& value : vec) {
        value /= num_values;
    }

    cout << vec;

    random_device rd;
    mt19937 g(rd());
    shuffle(vec.begin(), vec.end(), g);

    return vec;
}


int poly_evaluation_cost(int degree) {
    //Cost for running the Paterson-Stockmeyer algorithm
    //https://github.com/openfheorg/openfhe-development/blob/main/src/pke/examples/FUNCTION_EVALUATION.md

    if (degree < 5) {
        return 3;
    } else if (degree < 13) {
        return 4;
    } else if (degree < 27) {
        return 5;
    } else if (degree < 27) {
        return 6;
    } else if (degree < 59) {
        return 7;
    } else if (degree < 119) {
        return 8;
    } else if (degree < 247) {
        return 9;
    } else if (degree < 495) {
        return 10;
    } else if (degree < 1007) {
        return 11;
    } else if (degree < 2031) {
        return 12;
    } else {
        cerr << "Use a valid degree!" << endl;
        return 0;
    }

}

static inline void print_duration(chrono::time_point<steady_clock, nanoseconds> start, const string &title) {
    auto ms = duration_cast<milliseconds>(steady_clock::now() - start);

    auto secs = duration_cast<seconds>(ms);
    ms -= duration_cast<milliseconds>(secs);
    auto mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);

    if (mins.count() < 1) {
        cout << "⌛(" << title << "): " << secs.count() << ":" << ms.count() << "s" << endl;
    } else {
        cout << "⌛(" << title << "): " << mins.count() << "." << secs.count() << ":" << ms.count() << endl;
    }
}



#endif //ORDERING_UTILS_H
