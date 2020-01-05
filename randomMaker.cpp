//
// Created by tom10 on 2020/1/3.
//

#include "randomMaker.h"
#include <omp.h>

randomMaker::randomMaker(uint64_t min, uint64_t max, std::mt19937_64 rd) {
    this->min = min;
    this->max = max;
    this->rd = rd;
}

randomMaker::randomMaker(uint64_t max, std::mt19937_64 rd): randomMaker(max / 10, max, rd){};

// create a 64-bit prime
randomMaker::randomMaker(std::mt19937_64 rd): randomMaker(0x8000000000000000, 0xffffffffffffffff, rd){};

bool randomMaker::isPrime(uint64_t n) {
    if (n <= 3) return (n > 1);

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

uint64_t randomMaker::generatePrime() {
    bool success = false;
    uint64_t rdP;
    while (!success) {
        {
            // any random number
            uint64_t rdN = this->dist64(this->rd) % (max - min) + min;
            if (isPrime(rdN)) {
                rdP = rdN;
                success = true;
            }
    }
    }

    return rdP;
}

uint32_t randomMaker::generateUint32() {
    return this->dist32(this->rd) % (max - min) + min;
}


