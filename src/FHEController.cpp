//
// Created by Lorenzo on 14/04/24.
//

//TODO Tutto da commentare bene con ChatGPT

#include "FHEController.h"

int FHEController::generate_context(int num_slots, int levels_required) {
    CCParams<CryptoContextCKKSRNS> parameters;

    parameters.SetSecretKeyDist(SPARSE_TERNARY);

    //parameters.SetSecurityLevel(lbcrypto::HEStd_128_classic);
    //parameters.SetRingDim(1 << 16);

    parameters.SetSecurityLevel(lbcrypto::HEStd_NotSet);
    parameters.SetRingDim(1 << 14);

    parameters.SetNumLargeDigits(5); //d_{num} Se lo riduci, aumenti il logQP, se lo aumenti, aumenti memori
    parameters.SetBatchSize(num_slots);

    level_budget = {3, 3};

    ScalingTechnique rescaleTech = FLEXIBLEAUTO;

    int dcrtBits = 50;
    int firstMod = 51;

    parameters.SetScalingModSize(dcrtBits);
    parameters.SetScalingTechnique(rescaleTech);
    parameters.SetFirstModSize(firstMod);

    uint32_t levelsUsedBeforeBootstrap = levels_required + 1;

    int circuit_depth = levelsUsedBeforeBootstrap + FHECKKSRNS::GetBootstrapDepth(level_budget, SPARSE_TERNARY);

    cout << endl << "Ciphertexts depth: " << circuit_depth << ", available multiplications: "
         << levelsUsedBeforeBootstrap - 2 << endl;

    parameters.SetMultiplicativeDepth(circuit_depth);

    context = GenCryptoContext(parameters);

    cout << "Context built, generating keys..." << endl;

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

void FHEController::generate_rotation_keys(int num_slots) {
    vector<int> rotations;

    for (int i = 0; i < log2(num_slots); i++) {
        rotations.push_back(pow(2, i));
        rotations.push_back(-pow(2, i));
    }

    //Rimuovo gli eventuali duplicati
    sort(rotations.begin(), rotations.end());
    rotations.erase(unique(rotations.begin(), rotations.end()), rotations.end());

    cout << "Rotation keys for: " << rotations << endl;

    context->EvalRotateKeyGen(key_pair.secretKey, rotations);
}

Ptxt FHEController::encode(const vector<double> &vec, int level, int plaintext_num_slots) {
    Ptxt p = context->MakeCKKSPackedPlaintext(vec, 1, level, nullptr, plaintext_num_slots);
    p->SetLength(plaintext_num_slots);
    return p;
}

Ptxt FHEController::encode(double val, int level, int plaintext_num_slots) {
    vector<double> vec;
    for (int i = 0; i < plaintext_num_slots; i++) {
        vec.push_back(val);
    }

    Ptxt p = context->MakeCKKSPackedPlaintext(vec, 1, level, nullptr, plaintext_num_slots);
    p->SetLength(plaintext_num_slots);
    return p;
}

Ctxt FHEController::encrypt(const vector<double> &vec, int level, int plaintext_num_slots) {
    Ptxt p = encode(vec, level, plaintext_num_slots);

    return context->Encrypt(p, key_pair.publicKey);
}

Ctxt FHEController::encrypt_ptxt(const Ptxt& p) {
    return context->Encrypt(p, key_pair.publicKey);
}

Ptxt FHEController::decrypt(const Ctxt &c) {
    Ptxt p;
    context->Decrypt(key_pair.secretKey, c, &p);
    return p;
}

Ctxt FHEController::add(const Ctxt &c1, const Ctxt &c2) {
    return context->EvalAdd(c1, c2);
}

Ctxt FHEController::add_tree(vector<Ctxt> v) {
    return context->EvalAddMany(v);
}

Ctxt FHEController::sub(const Ctxt &c1, const Ctxt &c2) {
    return context->EvalSub(c1, c2);
}

Ctxt FHEController::mult(const Ctxt &c1, double d) {
    Ptxt p = encode(d, c1->GetLevel(), c1->GetSlots());
    return context->EvalMult(c1, p);
}

Ctxt FHEController::mult(const Ctxt &c, const Ptxt& p) {
    return context->EvalMult(c, p);
}

Ctxt FHEController::rot(const Ctxt& c, int index) {
    return context->EvalRotate(c, index);
}

Ctxt FHEController::bootstrap(const Ctxt &c) {
    return context->EvalBootstrap(c);
}

Ctxt FHEController::relu(const Ctxt &c, int poly_degree) {
    return context->EvalChebyshevFunction([](double x) -> double { if (x < 0) return 0; else return x; }, c,
                                              -1,
                                              1, poly_degree);
}

Ctxt FHEController::min(const Ctxt &a, const Ctxt &b, int poly_degree) {
    return sub(a, relu(sub(a, b), poly_degree));
}

Ctxt FHEController::swap(const Ctxt &a, int delta, int round, int stage, int poly_degree, int num_slots) {
    Ctxt rot_pos = rot(a, delta);
    Ctxt rot_neg = rot(a, -delta);

    Ctxt f = min(a, rot_pos, poly_degree);

    Ctxt m1 = f;
    Ctxt m2 = sub(add(a, rot_neg), rot(f, -delta));
    Ctxt m3 = sub(add(a, rot_pos), f);
    Ctxt m4 = rot(f, -delta);

    vector<Ptxt> masks = generate_layer_masks(m1->GetLevel(), m1->GetSlots(), round, stage);

    return add_tree({mult(m1, masks[0]), mult(m2, masks[1]), mult(m3, masks[2]), mult(m4, masks[3])});

}

vector<Ptxt> FHEController::generate_layer_masks(int encoding_level, int length, int round, int stage) {
    vector<double> mask_1, mask_2, mask_3, mask_4;

    for (int i = 0; i < length / (pow(2, round + 2)); i++) {

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

        if ((i + 1) * pow(2, stage + round + 1) >= length) {
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

        if ((i + 1) * pow(2, stage + round + 2) >= length) {
            break;
        }

    }

    return {encode(mask_1, encoding_level, length),
            encode(mask_2, encoding_level, length),
            encode(mask_3, encoding_level, length),
            encode(mask_4, encoding_level, length)};
}

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