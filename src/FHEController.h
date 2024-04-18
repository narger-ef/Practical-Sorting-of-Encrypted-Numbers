#ifndef SORTING_FHECONTROLLER_H
#define SORTING_FHECONTROLLER_H

#include "openfhe.h"
#include "ciphertext-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"

using namespace lbcrypto;
using namespace std;
using namespace std::chrono;

// Define aliases for clarity
using Ptxt = Plaintext;
using Ctxt = Ciphertext<DCRTPoly>;

class FHEController {
    CryptoContext<DCRTPoly> context; // Crypto context for the FHE system

public:
    FHEController() {}


    // Generate the cryptocontext with a specified number of slots and levels
    int generate_context(int num_slots, int levels_required, bool toy_parameters);

    // Generate rotation keys for performing rotations
    void generate_rotation_keys(int num_slots);

    /**
      * Basic RNS-CKKS operations
      */

    // Encode a vector of doubles into a plaintext
    Ptxt encode(const vector<double>& vec, int level, int num_slots);

    // Encodes a value in a plaintext (it will be repeated)
    Ptxt encode(double value, int level, int num_slots);

    // Encrypt a vector of doubles
    Ctxt encrypt(const vector<double>& vec, int level = 0, int plaintext_num_slots = 0);

    //Decodes a ciphertexxt
    vector<double> decode(const Ptxt& p);

    // Decrypt a ciphertext
    Ptxt decrypt(const Ctxt& c);



    /**
     * Basic Arithmetic Operations
     */

    // Add two ciphertexts
    Ctxt add(const Ctxt& c1, const Ctxt& c2);

    // Add multiple ciphertexts using a tree structure
    Ctxt add_tree(vector<Ctxt> v);

    // Subtract one ciphertext from another
    Ctxt sub(const Ctxt& c1, const Ctxt& c2);

    // Subtract a ciphertext by a plaintext
    Ctxt sub(const Ctxt& c, const Ptxt& p);

    // Multiply a ciphertext by a plaintext
    Ctxt mult(const Ctxt& c, const Ptxt& p);

    // Multiply a ciphertext by a scalar
    Ctxt mult(const Ctxt& c, double d);

    // Rotate a ciphertext by a specified index
    Ctxt rot(const Ctxt& c, int index);

    // Perform bootstrapping operation on a ciphertext
    Ctxt bootstrap(const Ctxt& c);

    /**
      * Sorting Network operations
      */

    // Find the minimum of two ciphertexts
    Ctxt min(const Ctxt& a, const Ctxt& b, int degree);

    // Perform a swap operation in a sorting network
    Ctxt swap(const Ctxt &a, int delta, int round, int stage, int poly_degree);

    // Perform a swap operation in a sorting network with errors correction
    Ctxt swap(const Ctxt &a, int delta, int round, int stage, int poly_degree, vector<double> codomain);

    // Generate masks for a sorting network layer
    vector<Ptxt> generate_layer_masks(int encoding_level, int length, int round, int stage, double mask_value = 1.0);


    /**
      * Utilities
      */

    // Print the values of slots in a ciphertext
    void print(const Ctxt& c, int slots = 0, string prefix = "");

private:
    KeyPair<DCRTPoly> key_pair; // Key pair for the FHE system
};

#endif //SORTING_FHECONTROLLER_H
