//
// Created by Lorenzo on 14/04/24.
//

#include "FHEController.h"

/**
 * Generate the Crypto Context for the RNS-CKKS cryptosystem.
 *
 * @param num_slots The number of slots in the ciphertext.
 * @param levels_required The required circuit depth
 * @param toy_parameters Choose whether to use toy parameters (true)
 * or 128-bit security parameters (false)
 * @return the total depth of the circuit
 */
int FHEController::generate_context(int num_slots, int levels_required, bool toy_parameters) {
    CCParams<CryptoContextCKKSRNS> parameters;

    parameters.SetSecretKeyDist(SPARSE_TERNARY);
    vector<uint32_t> level_budget;

    level_budget = {3, 3};

    if (toy_parameters) {
        parameters.SetSecurityLevel(lbcrypto::HEStd_NotSet);
        parameters.SetRingDim(1 << 14);
    } else {
        parameters.SetSecurityLevel(lbcrypto::HEStd_128_classic);
        parameters.SetRingDim(1 << 16);
    }

    parameters.SetNumLargeDigits(5);
    parameters.SetBatchSize(num_slots);

    ScalingTechnique rescaleTech = FLEXIBLEAUTO;

    int dcrtBits = 50;
    int firstMod = 54;

    parameters.SetScalingModSize(dcrtBits);
    parameters.SetScalingTechnique(rescaleTech);
    parameters.SetFirstModSize(firstMod);

    int levelsUsedBeforeBootstrap = levels_required + 1;

    int circuit_depth = levelsUsedBeforeBootstrap + FHECKKSRNS::GetBootstrapDepth(level_budget, SPARSE_TERNARY);

    parameters.SetMultiplicativeDepth(circuit_depth);

    context = GenCryptoContext(parameters);
    context->Enable(PKE);
    context->Enable(KEYSWITCH);
    context->Enable(LEVELEDSHE);
    context->Enable(ADVANCEDSHE);
    context->Enable(FHE);

    key_pair = context->KeyGen();

    context->EvalMultKeyGen(key_pair.secretKey);

    context->EvalBootstrapSetup(level_budget, {0, 0}, num_slots);
    context->EvalBootstrapKeyGen(key_pair.secretKey, num_slots);

    return circuit_depth;
}

/**
 * Generate rotation keys for performing rotations on ciphertexts. In particular,
 * the required rotation keys are for the indexes [+-1, +-2, +-4, +-8, ..., n_slots)
 *
 * @param num_slots The number of slots in the ciphertext.
 */
void FHEController::generate_rotation_keys(int num_slots) {
    vector<int> rotations;

    for (int i = 0; i < log2(num_slots); i++) {
        rotations.push_back(pow(2, i));
        rotations.push_back(-pow(2, i));
    }

    context->EvalRotateKeyGen(key_pair.secretKey, rotations);
}

/**
 * Encode a vector of doubles into a RLWE polynomial using the CKKS scheme.
 *
 * @param vec The vector of doubles to be encoded.
 * @param level The level at which the vector is encoded.
 * @param num_slots The number of plaintext slots.
 * @return The encoded plaintext.
 */
Ptxt FHEController::encode(const vector<double> &vec, int level, int num_slots) {
    Ptxt p = context->MakeCKKSPackedPlaintext(vec, 1, level, nullptr, num_slots);
    p->SetLength(num_slots);

    return p;
}

/**
 * Encode a real value into a RLWE polynomial using the CKKS scheme.
 *
 * @param value The real value to be encoded
 * @param level The level at which the vector is encoded.
 * @param num_slots The number of plaintext slots.
 * @return The encoded plaintext.
 */
Ptxt FHEController::encode(double value, int level, int num_slots) {
    vector<double> repeated_value;
    for (int i = 0; i < num_slots; i++) repeated_value.push_back(value);

    return encode(repeated_value, level, num_slots);
}

/**
 * Encrypt a vector of doubles
 *
 * @param vec The vector of doubles to be encrypted.
 * @param level The encryption level.
 * @param num_slots The number of plaintext slots.
 * @return The encrypted ciphertext.
 */
Ctxt FHEController::encrypt(const vector<double> &vec, int level, int num_slots) {
    Ptxt p = encode(vec, level, num_slots);

    return context->Encrypt(p, key_pair.publicKey);
}

/**
 * Decodes a plaintext into a vector of double values.
 *
 * @param c The ciphertext to be decoded.
 * @return The decoded plaintext.
 */
vector<double> FHEController::decode(const Ptxt& p) {
    return p->GetRealPackedValue();
}

/**
 * Decrypt a ciphertext into a plaintext.
 *
 * @param c The ciphertext to be decrypted.
 * @return The decrypted plaintext.
 */
Ptxt FHEController::decrypt(const Ctxt &c) {
    Ptxt p;
    context->Decrypt(key_pair.secretKey, c, &p);

    return p;
}

/**
 * Perform a slot-wise addition operation on two ciphertexts
 *
 * @param a The first ciphertext.
 * @param b The second ciphertext.
 * @return The result of addition operation on the ciphertexts.
 */
Ctxt FHEController::add(const Ctxt &a, const Ctxt &b) {
    return context->EvalAdd(a, b);
}

/**
 * Perform a slot-wise addition operation on a set of ciphertexts
 *
 * @param v A vector of ciphertexts
 * @return The result of addition operation on the ciphertexts.
 */
Ctxt FHEController::add_tree(vector<Ctxt> v) {
    return context->EvalAddMany(v);
}

/**
 * Perform a slot-wise subtraction operation on two ciphertexts
 *
 * @param a The first ciphertext.
 * @param b The second ciphertext.
 * @return The result of subtraction operation on the ciphertexts.
 */
Ctxt FHEController::sub(const Ctxt &a, const Ctxt &b) {
    return context->EvalSub(a, b);
}

/**
 * Perform a slot-wise multiplication operation between a ciphertext
 * and a plaintext
 *
 * @param c A ciphertext.
 * @param p A plaintext
 * @return The result of multiplication operation between the inputs
 */
Ctxt FHEController::mult(const Ctxt &c, const Ptxt& p) {
    return context->EvalMult(c, p);
}

/**
 * Perform a multiplication operation between all the slots of a ciphertext
 * and a real value
 *
 * @param c A ciphertext.
 * @param v A real value
 * @return The result of multiplication operation between the inputs
 */
Ctxt FHEController::mult(const Ctxt &c, double v) {
    return context->EvalMult(c, encode(v, c->GetLevel(), c->GetSlots()));
}

/**
 * Rotate the elements in a ciphertext by a specified number of positions.
 * Positive indexes rotate the elements to the left, negative indexes
 * to the right
 *
 * @param c The ciphertext to be rotated.
 * @param index The rotation index.
 * @return The rotated ciphertext.
 */
Ctxt FHEController::rot(const Ctxt& c, int index) {
    return context->EvalRotate(c, index);
}

/**
 * Refresh the level of a ciphertext by performing a bootstrapping operation.
 *
 * @param c The ciphertext to be bootstrapped.
 * @return The bootstrapped ciphertext.
 */
Ctxt FHEController::bootstrap(const Ctxt &c) {
    return context->EvalBootstrap(c);
}

/**
 * Implements the minimum function defined as
 *
 * min(a,b) = a - max(0, a - b)
 *
 * where max(0, a - b) is the so-called ReLU function, approximated using
 * Chebyshev polynomials
 *
 * @param a
 * @param b
 * @param poly_degree
 * @return
 */
Ctxt FHEController::min(const Ctxt &a, const Ctxt &b, int poly_degree) {
    Ctxt relu = context->EvalChebyshevFunction([](double x) -> double { if (x < 0) return 0; else return x; },
                                               sub(a, b),
                                               -1,
                                               1, poly_degree);
    return sub(a, relu);
}

/**
 * Evaluates a layer of a Sorting Network. In particular, it performs the swap
 * operation exploiting the SIMD parallelism in order to evaluate a whole layer.
 * See (TODO add link) for a clear implementation
 *
 * @param in The input vector
 * @param delta The delta value, i.e., the distance between compared elements
 * @param round The current round
 * @param stage The current stage
 * @param poly_degree The degree of the ReLU Chebyshev polynomial
 * @return The vector obtained by applying the swapping opeartions
 */
Ctxt FHEController::swap(const Ctxt &in, int delta, int round, int stage, int poly_degree) {
    Ctxt rot_pos = rot(in, delta);
    Ctxt rot_neg = rot(in, -delta);

    // This performs the evaluation of the min function
    Ctxt m1 = min(in, rot_pos, poly_degree);

    // The other values are obtained in function of m1
    Ctxt m3 = sub(add(in, rot_pos), m1);
    Ctxt m4 = rot(m1, -delta);
    Ctxt m2 = sub(add(in, rot_neg), m4);

    vector<Ptxt> masks = generate_layer_masks(m1->GetLevel(), m1->GetSlots(), round, stage);

    return add_tree({mult(m1, masks[0]), mult(m2, masks[1]), mult(m3, masks[2]), mult(m4, masks[3])});

}

/**
 * Generates a set of four masks to be applied to the four comparison vectors
 *
 * @param encoding_level The level at which the masks must be encoded
 * @param num_slots The number of values of the input vector
 * @param round The current round
 * @param stage The current stage
 * @return Four masks to be applied to the four comparison vectors for extracting the
 * required values
 */
vector<Ptxt> FHEController::generate_layer_masks(int encoding_level, int num_slots, int round, int stage) {
    vector<double> mask_1, mask_2, mask_3, mask_4;

    for (int i = 0; i < num_slots / (pow(2, round + 2)); i++) {

        for (int times = 0; times < pow(2, stage); times++) {
            for (int j = 0; j < pow(2, round); j++) {
                mask_1.push_back(1);
                mask_2.push_back(0);
                mask_3.push_back(0);
                mask_4.push_back(0);
            }

            for (int j = 0; j < pow(2, round); j++) {
                mask_1.push_back(0);
                mask_2.push_back(1);
                mask_3.push_back(0);
                mask_4.push_back(0);
            }
        }

        if ((i + 1) * pow(2, stage + round + 1) >= num_slots) {
            break;
        }

        for (int times = 0; times < pow(2, stage); times++) {
            for (int j = 0; j < pow(2, round); j++) {
                mask_1.push_back(0);
                mask_2.push_back(0);
                mask_3.push_back(1);
                mask_4.push_back(0);
            }

            for (int j = 0; j < pow(2, round); j++) {
                mask_1.push_back(0);
                mask_2.push_back(0);
                mask_3.push_back(0);
                mask_4.push_back(1);
            }
        }

        if ((i + 1) * pow(2, stage + round + 2) >= num_slots) {
            break;
        }

    }

    return {encode(mask_1, encoding_level, num_slots),
            encode(mask_2, encoding_level, num_slots),
            encode(mask_3, encoding_level, num_slots),
            encode(mask_4, encoding_level, num_slots)};
}

/**
 * Print the content of a ciphertext
 *
 * @param c The ciphertext to be printed.
 * @param slots The number of slots in the ciphertext.
 * @param prefix A prefix string to be printed.
 */
void FHEController::print(const Ctxt &c, int slots, string prefix) {
    if (slots == 0) {
        slots = c->GetSlots();
    }

    cout << prefix;

    Ptxt result;
    context->Decrypt(key_pair.secretKey, c, &result);
    result->SetSlots(slots);
    vector<double> v = result->GetRealPackedValue();

    cout << setprecision(4) << fixed;
    cout << "[ ";

    for (int i = 0; i < slots; i += 1) {
        string segno = "";
        if (v[i] > 0) {
            segno = "";
        } else {
            segno = "-";
            v[i] = -v[i];
        }


        if (i == slots - 1) {
            cout << segno << v[i] << " ]";
        } else {
            if (abs(v[i]) <= 0.0001)
                cout << "0.0000" << " ";
            else
                cout << segno << v[i] << " ";
        }
    }

    cout << endl;
}