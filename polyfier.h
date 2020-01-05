//
// Created by tom10 on 2020/1/3.
//

#ifndef POLY_SPLIT_POLYFIER_H
#define POLY_SPLIT_POLYFIER_H


#include <cstdint>

class polyfier {
    uint32_t plain;
    uint64_t* buffer;
public:
    uint64_t *getBuffer() const;

private:
    bool ready;
    int nCopies;
    int nRequired;

public:
    polyfier(uint32_t msg, uint8_t nCopies, uint8_t nRequired);
    polyfier(uint8_t nCopies, uint8_t nRequired);
    ~polyfier();
    void set_msg(uint32_t msg);

private:
    polyfier() = default;
    void encrypt();
};


uint64_t poly_evaluate(std::vector<uint32_t> polynomial, uint32_t at);
std::vector<uint32_t> random_polynomial_at_zero_point(uint32_t degree, uint32_t zero_y);


#endif //POLY_SPLIT_POLYFIER_H
