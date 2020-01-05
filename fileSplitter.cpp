//
// Created by tom10 on 2020/1/5.
//

#include <iostream>
#include "fileSplitter.h"
#include "polyfier.h"

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

    // initialize polyfier
    auto pf = polyfier(this->nCopies, this->nRequired);
    uint64_t nRead = 0u;
    while (!this->input.eof()) {
        uint32_t msg;
        this->input.read(reinterpret_cast<char *>(&msg), sizeof(msg));
        uint32_t lastRead = this->input.gcount();
        nRead += lastRead;
        // last bit alignment
        if (this->input.eof()) {
            msg = msg & ((1u << (lastRead) * 8)) - 1;
        }

        pf.set_msg(msg);
        uint64_t* buffer = pf.getBuffer();
        for (uint8_t i = 0; i < this->nCopies; i++) {
            this->outputs[i]->write(reinterpret_cast<const char *>(buffer + i), sizeof(uint64_t));
        }
    }

    for (uint8_t i = 0; i < nCopies; i++) {
        this->outputs[i]->seekp(1);
        this->outputs[i]->write(reinterpret_cast<const char *>(&nRead), sizeof(uint64_t));
    }

}