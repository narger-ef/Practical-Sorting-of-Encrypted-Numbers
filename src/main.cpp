#include <iostream>
#include "openfhe.h"
#include "chrono"
#include "FHEController.h"
#include "Utils.h"


using namespace lbcrypto;
using namespace std;
using namespace std::chrono;


int main() {
    FHEController controller;

    int num_values = 1024;
    int relu_degree = 247;

    // Levels required by ReLU approximation
    int levels_consumption = poly_evaluation_cost(relu_degree);

    // One more level for the masking operation
    levels_consumption += 1;

    int circuit_depth = controller.generate_context(num_values, levels_consumption);

    controller.generate_rotation_keys(num_values);

    vector<double> vec = generate_random_vector(num_values);

    Ctxt in = controller.encrypt(vec, circuit_depth - levels_consumption - 1, num_values);

    cout << endl << "Input vector: " << vec << endl;

    // The number of iteration is (N*(N+1)/2), where N is the logarithm of the number of slots
    int iterations = (log2(num_values) * (log2(num_values) + 1)) / 2;

    int current_iteration = 1;

    auto start_time = steady_clock::now();

    for (int i = 0; i < log2(num_values); i++) {
        for (int j = 0; j < i + 1; j++) {
            int delta = pow(2, i - j);
            int stage = i - j;
            int round = j;

            //cout << "Level before: " << in->GetLevel() << endl;
            auto start_time_local = steady_clock::now();
            in = controller.swap(in, delta, stage, round, relu_degree, num_values);
            print_duration(start_time_local, "Swap");
            start_time_local = steady_clock::now();
            //cout << "Level after: " << in->GetLevel() << endl;
            auto test = controller.decrypt(in);
            in = controller.bootstrap(in);
            print_duration(start_time_local, "Bootstrapping");
            //cout << "Level fresh: " << in->GetLevel() << endl;

            cout << current_iteration << " / " << iterations << endl;
            current_iteration++;
        }
    }

    sort(vec.begin(), vec.end());

    print_duration(start_time, "The evaluation of the circuit took");

    cout << setprecision(4) << fixed;
    cout << "Expected: " << vec << endl << "Obtained: ";
    controller.print(in);

    return 0;
}

