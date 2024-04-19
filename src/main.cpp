#include <iostream>
#include "openfhe.h"
#include "chrono"
#include "../src/FHEController.h"
#include "../src/Utils.h"

using namespace lbcrypto;
using namespace std;
using namespace std::chrono;

void read_arguments(int argc, char *argv[]);

/**
 * Arguments parameters
 */
// Degree of the Chebyshev ReLU approximation
int poly_degree = 245;

// Input
vector<double> input_values = {};
int verbose = 0;
bool toy_parameters = false;
double scaling_factor = 1;
bool error_correction = false;
bool load_context = false;

int main(int argc, char *argv[]) {
    FHEController controller;

    if (argc > 1) read_arguments(argc, argv);

    if (input_values.empty()) {
        cout << "Could not parse input arguments, going with default parameters." << endl;
        input_values = generate_random_vector(8);
        toy_parameters = true;
        verbose = 1;
        //return -1;
    }

    vector<double> codomain = {};
    //Position 0: Minimum
    //Position 1: Maximum
    //Position 2: Range
    if (error_correction) {
        codomain = get_codomain(poly_degree);
    }

    if (verbose >= 1) cout << "Input vector: " << input_values << endl;

    int num_values = input_values.size();

    // Levels required by max(0, x) approximation
    int levels_consumption = poly_evaluation_cost(poly_degree);

    // One more level for the masking operation
    levels_consumption += 1;

    int circuit_depth;

    if (load_context) {
        cout << "loading existing context" << endl;
        circuit_depth = controller.load_context(num_values, levels_consumption);
    } else {
        cout << "generating new context" << endl;
        circuit_depth = controller.generate_context(num_values, levels_consumption, toy_parameters);
    }

    controller.generate_rotation_keys(num_values);

    Ctxt in = controller.encrypt(input_values, circuit_depth - levels_consumption - 3, num_values);

    if (verbose >= 2) {
        cout << "Arguments: poly_degree: " << poly_degree
             << ", toy_parameters: " << toy_parameters
             << ", scaling_factor: " << scaling_factor
             << ", error_correction: " << error_correction
             << ", verbose: " << verbose
             << ", circuit depth: " << circuit_depth << endl;
    }

    if (scaling_factor != 1)
        in = controller.mult(in, 1 / scaling_factor);

    // The number of iteration is (N*(N+1)/2), where N is the logarithm of the number of slots
    int iterations = (log2(num_values) * (log2(num_values) + 1)) / 2;

    int current_iteration = 1;

    auto start_time = steady_clock::now();

    for (int i = 0; i < log2(num_values); i++) {
        for (int j = 0; j < i + 1; j++) {
            int delta = pow(2, i - j);
            int stage = i - j;
            int round = j;

            auto start_time_local = steady_clock::now();

            in = controller.swap(in, delta, stage, round, poly_degree, codomain);

            if (verbose >= 2) print_duration(start_time_local, "Swap");
            start_time_local = steady_clock::now();

            if (current_iteration < iterations)
                in = controller.bootstrap(in);

            if (verbose >= 2) print_duration(start_time_local, "Bootstrapping");

            if (verbose >= 1) cout << "Layer " << current_iteration << " / " << iterations << " done." << endl;
            current_iteration++;

        }
    }

    // This could be avoided by running the last masking phase with scaling_factor, but whatever
    in = controller.mult(in, scaling_factor);

    sort(input_values.begin(), input_values.end());

    cout << setprecision(4) << fixed;
    if (verbose >= 0) cout << "Expected: " << input_values << endl << "Obtained: ";
    if (verbose >= 0) controller.print(in);

    vector<double> fhe_result = controller.decode(controller.decrypt(in));

    if (verbose >= 1) cout << "Infinity norm : " << infinity_norm(input_values, fhe_result) << endl;
    if (verbose >= 0) cout << "Precision bits: " << GREEN_TEXT << precision_bits(input_values, fhe_result) << RESET_COLOR << endl;
    if (verbose >= 0) print_duration(start_time, "The evaluation of the circuit took");

    return 0;
}

void read_arguments(int argc, char *argv[]) {

    if (argc == 1) {
        cerr << "TODO: mettere istruzioni" << endl;
    }

    if (argc > 2 && string(argv[1]) == "--random") {
        int num_values = stoi(string(argv[2]));

        if (floor(log2(num_values)) != ceil(log2(num_values))) {
            cerr << "The number of values must be a power of two" << endl;
        }

        input_values = generate_random_vector(num_values);
    } else if (argc > 2 && string(argv[1]) == "--file") {
        ifstream f(argv[2]); //taking file as inputstream
        string str;
        if (f) {
            ostringstream ss;
            ss << f.rdbuf(); // reading data
            str = ss.str();
        } else {
            cout << "Could not find \"" << string(argv[2]) << "\"" << endl;
        }
        input_values = parse_input_vector("[ " + str + " ]");
    }
    else {
        try {
            input_values = parse_input_vector(argv[1]);
        } catch (...) {
            cerr << "A problem occured in parsing the input vector" << endl;
        }
    }

    for (int i = 2; i < argc; i++) {
        if (string(argv[i]) == "--poly_degree" && i + 1 < argc) {
            poly_degree = stoi(string(argv[i + 1]));
        }

        if (string(argv[i]) == "--verbose" && i + 1 < argc) {
            verbose = stoi(string(argv[i + 1]));
        }

        if (string(argv[i]) == "--toy_parameters") {
            toy_parameters = true;
        }

        if (string(argv[i]) == "--error_correction") {
            error_correction = true;
        }

        if (string(argv[i]) == "--scaling_factor" && i + 1 < argc) {
            scaling_factor = stod(string(argv[i + 1]));
        }

        if (string(argv[i]) == "--load_context") {
            load_context = true;
        }
    }
}