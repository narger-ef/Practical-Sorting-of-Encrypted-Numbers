//
// Created by Lorenzo on 14/04/24.
//

#ifndef SORTING_UTILS_H
#define SORTING_UTILS_H

#include <iostream>
#include <algorithm> // for shuffle
#include <numeric>   // for iota

using namespace std;

vector<double> generate_random_vector(int num_values) {
    //Generates a vector of num_values elements uniformely sampled from (0, 1)

    vector<double> vec(num_values);

    iota(vec.begin(), vec.end(), 0.5);

    for (double& value : vec) {
        value /= num_values;
    }

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

static inline vector<double> parse_input_vector(const std::string& input) {
    std::vector<double> result;
    std::istringstream iss(input);
    char discard;
    double value;

    // Discard the opening bracket
    iss >> discard;

    // Parse the doubles
    while (iss >> value) {
        result.push_back(value);

        // Discard the comma or closing bracket
        iss >> std::ws >> discard;
    }

    if (floor(log2(result.size())) != ceil(log2(result.size()))) {
        cerr << "The number of values must be a power of two!" << endl;
        return {};
    }

    return result;
}

static inline double infinity_norm(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double max_diff = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        double diff = abs(vec1[i] - vec2[i]);
        if (diff > max_diff) {
            max_diff = diff;
        }
    }

    return max_diff;
}

static inline double precision_bits(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    return -log2(infinity_norm(vec1, vec2));
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

static inline vector<string> tokenizer(string s, char del)
{
    stringstream ss(s);
    string word;

    vector<string> tokens;

    while (!ss.eof()) {
        getline(ss, word, del);
        tokens.push_back(word);
    }

    return tokens;
}

static inline vector<double> get_codomain(int degree) {
    std::ifstream in("../src/codomains.csv");

    std::string s;

    for(int i = 0; i < degree; ++i)
        std::getline(in, s);

    std::getline(in,s);

    vector<string> tokens = tokenizer(s, ',');

    return {stod(tokens[1]), stod(tokens[2]), stod(tokens[3])};
}


#endif //SORTING_UTILS_H
