//
// Created by tom10 on 2020/1/5.
//

#include "unpolyfier.h"
#include "randomMaker.h"
#define P randomMaker::LARGE_PRIME

uint64_t modInverse(uint64_t a);
// modular operation
uint64_t sub(uint64_t a, uint64_t b);
uint64_t add(uint64_t a, uint64_t b);
uint64_t mul(uint64_t a, uint64_t b);
uint64_t mdiv(uint64_t a, uint64_t b);
// points[i] is the value at p(i+1)
uint32_t unpolyfier::merge(vector<uint64_t> points, vector<uint64_t> xs) {
    uint64_t n = points.size();
    uint64_t result = 0;

    // y1, y2, ...
    for (uint64_t i = 0; i < n; i++) {
        uint64_t term = points[i];
        uint64_t xi = xs[i];
        for (uint64_t j = 0; j < n; j++) {
            if (j != i) {
                uint64_t xj = xs[j];
                term = mul(term, sub(0, xj)); // term = (x - x1)(x - x2)...(x - x_i-1)(x - x_i+1)...)
            }
        }
        uint64_t divider = 1;
        for (uint64_t j = 0; j < n; j++) {
            if (j != i) {
                uint64_t xj = xs[j];
                divider = mul(divider, sub(xi, xj)); // term = (xi - x1)(xi - x2)...(xi - x_i-1)(xi - x_i+1)...)
            }
        }
        term = mdiv(term, divider);
        result = add(result, term);
    }
    // poly at x = 0
    return result;
}



// Function to find modulo inverse of a (P should be prime, inv(a) = a ^ (P - 2))
uint64_t power(uint64_t x, uint64_t y) {
    if (y == 0) {
        return 1;
    }

    if (y == 1) {
        return x % P;
    }
    // repeated squaring
    uint64_t t = power(x, y / 2);
    t = mul(t, t);

    if (y % 2 == 0) {
        return t;
    }
    return mul(x, t);
}

uint64_t gcd(uint64_t a, uint64_t b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}
uint64_t modInverse(uint64_t a) {
    uint64_t g = gcd(a, P);
    if (g != 1) {
        throw exception();
    }
    return power(a, P - 2);
};

uint64_t sub(uint64_t a, uint64_t b){
    if (a >= b) {
        return (a - b) % P;
    }
    return P - (b - a) % P;
}
uint64_t add(uint64_t a, uint64_t b) {
    return ((a % P) + (b % P)) % P;
}

uint64_t mul(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    // check overflow
    uint64_t product = a * b;
    if (product / b == a) {
        // correct
        return product % P;
    }
    uint64_t lo, hi;
    if (a > b) {
        lo = b;
        hi = a;
    }else {
        lo = a;
        hi = b;
    }

    if (hi % 2 == 0) {
        product =  (mul(lo, hi / 2) * 2) % P;
    }else{
    product = (mul(lo, hi / 2) * 2 + lo) % P;
    }
    return product;
};

uint64_t mdiv(uint64_t a, uint64_t b) {
    if (b == 0) {
        printf("division by zero\n");
        throw exception();
    }
    return mul(a, modInverse(b));
}