//
// Created by Lorenzo on 14/04/24.
//

#ifndef ORDERING_FHECONTROLLER_H
#define ORDERING_FHECONTROLLER_H

#include "openfhe.h"
#include "ciphertext-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include <thread>

using namespace lbcrypto;
using namespace std;
using namespace std::chrono;


using Ptxt = Plaintext;
using Ctxt = Ciphertext<DCRTPoly>;

class FHEController {
    CryptoContext<DCRTPoly> context;

public:
    FHEController() {}


    /*
     * Cryptocontext
     */
    int generate_context(int num_slots, int levels_required);
    void generate_rotation_keys(int num_slots);

    /*
     * CKKS Encoding/Encryption
     */
    Ptxt encode(const vector<double>& vec, int level, int plaintext_num_slots);
    Ptxt encode(double val, int level, int plaintext_num_slots);
    Ctxt encrypt(const vector<double>& vec, int level = 0, int plaintext_num_slots = 0);
    Ctxt encrypt_ptxt(const Ptxt& p);
    Ptxt decrypt(const Ctxt& c);


    /*
     * Homomorphic operations
     */
    Ctxt add(const Ctxt& c1, const Ctxt& c2);
    Ctxt add_tree(vector<Ctxt> v);
    Ctxt sub(const Ctxt& c1, const Ctxt& c2);
    Ctxt mult(const Ctxt& c, double d);
    Ctxt mult(const Ctxt& c, const Ptxt& p);
    Ctxt rot(const Ctxt& c, int index);
    Ctxt bootstrap(const Ctxt& c);
    Ctxt relu(const Ctxt& c, int degree);

    Ctxt min(const Ctxt& a, const Ctxt& b, int degree);
    Ctxt swap(const Ctxt &a, int delta, int round, int stage, int poly_degree, int num_slots);

    vector<Ptxt> generate_layer_masks(int encoding_level, int length, int round, int stage);

    /*
     * I/O
     */
    void print(const Ctxt& c, int slots = 0, string prefix = "");


private:
    KeyPair<DCRTPoly> key_pair;
    vector<uint32_t> level_budget = {4, 4};


};


#endif //ORDERING_FHECONTROLLER_H
