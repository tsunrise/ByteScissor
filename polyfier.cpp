//
// Created by tom10 on 2020/1/3.
//

#include <random>
#include "polyfier.h"
#include "randomMaker.h"

polyfier::polyfier(uint8_t nCopies, uint8_t nRequired) {
    this->ready = false;
    this->buffer = new uint64_t[nCopies];
    this->nCopies = nCopies;
    this->nRequired = nRequired;
    std::random_device rd;
}

polyfier::polyfier(uint32_t msg, uint8_t nCopies, uint8_t nRequired) : polyfier::polyfier(nCopies, nRequired) {
    this->plain = msg;
    this->encrypt();
}

polyfier::~polyfier() {
    delete [] this->buffer;
}

void polyfier::set_msg(uint32_t msg) {
    this->plain = msg;
    encrypt();
}

/**
 * Generate a polynomial where poly[i] is the coefficient of x^i.
 * @param degree
 * @return
 */
std::vector<uint32_t> random_polynomial_at_zero_point(uint32_t degree, uint32_t zero_y) {
    std::vector<u_int32_t> poly;
    poly.push_back(0);
    std::random_device rd;
    std::mt19937_64 eng(rd());
    randomMaker rm (0xffffffff, eng);
    for (uint32_t i = 1; i < degree + 1; i++) {
        poly.push_back(rm.generateUint32());
    }
    poly[0] = zero_y;
    return poly;
}

/**
 * evaluate the polynomial at a point in gf(LARGE_PRIME)
 * @param polynomial
 * @param at
 * @return
 */
uint64_t poly_evaluate(std::vector<uint32_t> polynomial, uint32_t at) {
    uint64_t ans = 0;
    for (auto ptr = polynomial.rbegin(); ptr != polynomial.rend(); ++ptr) {
        ans *= at;
        ans += *ptr;
        ans %= randomMaker::LARGE_PRIME;
    }

    return ans;
}


void polyfier::encrypt() {
    uint32_t degree = this->nRequired - 1;
    auto poly = random_polynomial_at_zero_point(degree, this->plain);
    for (uint32_t i = 0; i < nCopies; i++) {
        this->buffer[i] = poly_evaluate(poly, i + 1); // give ith copy p(i)
    }
    this->ready = true;
}

/**
 * @return an array of length nCopies
 */
uint64_t *polyfier::getBuffer() const {
    return buffer;
}
