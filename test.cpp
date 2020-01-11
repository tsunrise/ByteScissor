//
// Created by tom10 on 2020/1/11.
//

#include "unpolyfier.h"
#include "randomMaker.h"
#include "polyfier.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <assert.h>

using namespace std;
int main(int argc, char** argv) {
    random_device rd;
    mt19937_64 eng(rd());
    randomMaker rm(1, 0xffffffffffffffff,eng);
    cout << "Checking Arithmetic correctness" << endl;
#pragma omp parallel for default(none) shared(rm)
    for (int i = 0; i < 500000; i++) {
        uint64_t a = rm.generateUint32();
        uint64_t b = rm.generateUint32();
        uint64_t adb = mdiv(a, b);
        if ((mul(b, adb) % randomMaker::LARGE_PRIME != a) || (mul(adb, b) % randomMaker::LARGE_PRIME != a)) {
            printf("Failed test: a = %lu, b = %lu, a/b = %lu \n", a, b, adb);
            exit(1);
        }
    }

//    cout << "Sanity Check: Polynomial >>> ";
//    vector<uint64_t> points{4066695527, 160641785, 2448808308, 451415728, 5037137841};
//    cout << "Answer: " << unpolyfier::merge(points, {2, 4,6,9, 15}) << endl;
//
    cout << "Checking Polynomial Merging" << endl;
#pragma omp parallel for default(none) shared(rm, rd, cout)
    for (int i = 0; i < 1000000; i++) {
        uint32_t msg = rm.generateUint32();
        uint32_t nCopies = 10 + (rm.generateUint32() % 5);
        uint32_t nRequired = (rm.generateUint32()) % nCopies + 1;
        polyfier pf(msg, nCopies, nRequired);
        auto buffer = pf.getBuffer();
        vector<uint32_t> indices(nCopies);
        iota(indices.begin(), indices.end(), 0);
        shuffle(indices.begin(), indices.end(), rd);
        uint32_t nInputPoints = nRequired + rm.generateUint32() % (nCopies - nRequired + 1);
        vector<uint64_t> inputPoints(nInputPoints);
        vector<uint64_t> inputXs(nInputPoints);
        for (int j = 0; j < nInputPoints; j++) {
            inputPoints[j] = buffer[indices[j]];
            inputXs[j] = indices[j] + 1;
        }
        uint32_t recovered = unpolyfier::merge(inputPoints, inputXs);
        if(recovered != msg){
            printf("Test Failed when generating message %u\n", msg);
            exit(1);
        }
    }
    cout << "\n\no(*°▽°*)o All tests passed!" << endl;
}