//
// Created by tom10 on 2020/1/5.
//

#ifndef POLY_SPLIT_UNPOLYFIER_H
#define POLY_SPLIT_UNPOLYFIER_H


#include <cstdint>
#include <vector>
using namespace std;
class unpolyfier {
public:
    static uint32_t merge(vector<uint64_t> points, vector<uint64_t> xs);
};

// for unit testing only
uint64_t power(uint64_t x, uint64_t y);
uint64_t mdiv(uint64_t a, uint64_t b);
uint64_t mul(uint64_t a, uint64_t b);

#endif //POLY_SPLIT_UNPOLYFIER_H
