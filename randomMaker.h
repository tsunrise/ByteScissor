//
// Created by tom10 on 2020/1/3.
//

#ifndef POLY_SPLIT_RANDOMMAKER_H
#define POLY_SPLIT_RANDOMMAKER_H

#include <cstdint>
#include <random>

class randomMaker {
    uint64_t min;
    uint64_t max;
    std::mt19937_64 rd;
    std::uniform_int_distribution<uint64_t> dist64;
    std::uniform_int_distribution<uint32_t> dist32;
public:
    randomMaker(uint64_t min, uint64_t max, std::mt19937_64 rd);
    randomMaker(uint64_t max, std::mt19937_64 rd);

    explicit randomMaker(std::mt19937_64 rd);

    uint32_t generateUint32();
    uint64_t generatePrime();

    static bool isPrime(uint64_t n);

    static const uint64_t LARGE_PRIME = 0xfb23fba6560244edL;

private:randomMaker() = default;
};


#endif //POLY_SPLIT_RANDOMMAKER_H
