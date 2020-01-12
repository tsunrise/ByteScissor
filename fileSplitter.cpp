//
// Created by tom10 on 2020/1/5.
//

#include <iostream>
#include "fileSplitter.h"
#include "polyfier.h"

#define CHUNK_SIZE 16777216

fileSplitter::fileSplitter(string path, uint8_t nCopies, uint8_t nRequired) {
    if (nCopies < nRequired) {
        cout << "Error: Number of generated blocks must be greater of equal to number of required blocks. ";
        throw exception();
    }
    this->nCopies = nCopies;
    this->nRequired = nRequired;
    this->path = path;
    this->input.open(path, ios::binary);
    if (!this->input.is_open()) {
        cout << "Unable to open input file." << endl;
        throw exception();
    }

}

fileSplitter::~fileSplitter() {
    this->input.close();
    for (auto output : this->outputs) {
        output->close();
        delete output;
    }
}

void fileSplitter::run() {
    for (uint8_t i = 0; i < this->nCopies; i++) {
        // populate output stream
        auto out = new ofstream(this->path + "." + to_string(i + 1) + ".part", ios::binary | ios::trunc);
        if (!(*out).is_open()) {
            cout << "Unable to create block files.";
            throw exception();
        }
        this->outputs.push_back(out);
    }

    // write sanity header (block id (uint8) and size in bytes(uint64))
    uint64_t ZERO64 = 0;
    for (uint8_t i = 1; i < nCopies + 1; i++) {
        this->outputs[i - 1]->write(reinterpret_cast<const char *>(&i), sizeof(uint8_t));
        this->outputs[i - 1]->write(reinterpret_cast<const char *>(&ZERO64), sizeof(uint64_t));
    }
    // read data and write block

    //read chunks
    auto* msg = new uint32_t[CHUNK_SIZE];
    uint64_t totalBytesRead = 0;
    while (!this->input.eof()) {
        this->input.read(reinterpret_cast<char *>(msg), sizeof(uint32_t) * CHUNK_SIZE);
        // ceiling division of gcount/4
        uint32_t lastRead = this->input.gcount();
        uint32_t nRead = (lastRead + sizeof(uint32_t) - 1) / sizeof(uint32_t);
        totalBytesRead += lastRead;
        if (this->input.eof()) {
            // last bit alignment
            msg[nRead - 1] = msg[nRead - 1] & (((1u << (lastRead % sizeof(uint32_t)) * 8)) - 1);
        }

        // decode and write
        // write one group: one uint32_t indicating first bit, and 32 uint32_t indicating the remaining bit
        for (uint32_t p = 0; p < nRead; p += 32) {
            // most_significant_bits
            uint32_t most_significant_bits[this->nCopies]; // little endian

            // input_buffer
            uint32_t input_buffer [this->nCopies][32];
            for (uint32_t i = 0; i < this->nCopies; i++) {
                most_significant_bits[i] = 0;
            }
            polyfier pf(this->nCopies, this->nRequired);
            uint32_t nWritten = 0;
            for (uint32_t i = p; i < p + 32 && i < nRead; i++) {
                pf.set_msg(msg[i]);
                uint64_t* buffer = pf.getBuffer();
                for (uint32_t j = 0; j < this->nCopies; j++) {
                    most_significant_bits[j] = most_significant_bits[j] |
                            ((1u << (i - p)) & ((buffer[j] & 0x100000000u) ? 0xffffffffffffffff : 0)); // set ith bit of first bits
                    input_buffer[j][i-p] = buffer[j];
                }
                nWritten += 1;
            }

            // write to file
            for (uint32_t j = 0; j < this->nCopies; j++) {
                this->outputs[j]->write(reinterpret_cast<const char *>(most_significant_bits + j), sizeof(uint32_t));
                this->outputs[j]->write(reinterpret_cast<const char *>(input_buffer[j]), sizeof(uint32_t) * nWritten);
            }

        }
    }
//    while (!this->input.eof()) {
//        uint32_t msg;
//        this->input.read(reinterpret_cast<char *>(&msg), sizeof(msg));
//        uint32_t lastRead = this->input.gcount();
//        nRead += lastRead;
//        // last bit alignment
//        if (this->input.eof()) {
//            msg = msg & ((1u << (lastRead) * 8)) - 1;
//        }
//
//        pf.set_msg(msg);
//        uint64_t* buffer = pf.getBuffer();
//        for (uint8_t i = 0; i < this->nCopies; i++) {
//            this->outputs[i]->write(reinterpret_cast<const char *>(buffer + i), sizeof(uint64_t));
//        }
//    }

    for (uint32_t i = 0; i < nCopies; i++) {
        this->outputs[i]->seekp(1);
        this->outputs[i]->write(reinterpret_cast<const char *>(&totalBytesRead), sizeof(uint64_t));
    }

}