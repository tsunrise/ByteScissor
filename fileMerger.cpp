//
// Created by tom10 on 2020/1/12.
//

#include "fileMerger.h"
#include "unpolyfier.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#define CHUNK_SIZE 8388608

void mergeFiles(vector<string> paths, string outputFilePath) {
    // initialize input stream
    uint32_t nCopies = paths.size();
    ifstream inputs[nCopies];
    for (uint32_t i = 0; i < nCopies; ++i) {
        inputs[i].open(paths[i], ios::binary);
        if (!inputs[i].is_open()) {
            cout << "Unable to open input file + " << paths[i] << endl;
            throw exception();
        }
    }

    // initialize output stream
    ofstream output;
    output.open(outputFilePath, ios::binary | ios::trunc);

    // read id
    uint8_t id[nCopies];
    for (uint32_t i = 0; i < nCopies; ++i) {
        inputs[i].read(reinterpret_cast<char *>(id + i), sizeof(uint8_t));
        if (inputs[i].gcount() != sizeof(uint8_t)) {
            cout << "file format error: unable to read id -> " << paths[i] << endl;
            throw exception();
        }
    }
    vector<uint64_t> id_vector(nCopies);
    for (uint32_t i = 0; i < nCopies; ++i) {
        id_vector[i] = id[i];
    }

    // read size
    uint64_t size;
    inputs[0].read(reinterpret_cast<char *>(&size), sizeof(uint64_t));
    uint64_t _check;
    for (uint32_t i = 1; i < nCopies; ++i) {
        inputs[i].read(reinterpret_cast<char *>(&_check), sizeof(uint64_t));
        if (_check != size) {
            cout << "file format error: fragment size mismatch";
            throw exception();
        }
    }

    // read and write
    vector<vector<uint64_t>> buffer(32, vector<uint64_t>(nCopies));
    uint64_t totalBytesWritten = 0;
    while (!inputs[0].eof()) {
        uint32_t contentBytesRead;
        for (uint32_t j = 0; j < nCopies; ++j) {
            // read head
            uint32_t most_significant_bits;
            inputs[j].read(reinterpret_cast<char *>(&most_significant_bits), sizeof(uint32_t));

            // read content
            uint32_t temp[32];
            inputs[j].read(reinterpret_cast<char *>(temp), sizeof(uint32_t) * 32);
            if (j == 0) {
                contentBytesRead = inputs[0].gcount();
            }

            // process buffer
            for (uint32_t i = 0; i < contentBytesRead / sizeof(uint32_t); ++i) {
                buffer[i][j] = ((((uint64_t) most_significant_bits & (1u << i)) >> i) << 32u) | (uint64_t) temp[i];
            }
        }

        // main calculation and write to file

        uint32_t plain_buffer[32];
        uint32_t nWritten = 0;
        for (uint32_t i = 0; i < contentBytesRead / sizeof(uint32_t); ++i) {
            plain_buffer[i] = unpolyfier::merge(buffer[i],id_vector);
            nWritten += 1;
        }
        if (totalBytesWritten + 32 * sizeof(uint32_t) > size) {
            output.write(reinterpret_cast<const char *>(plain_buffer), size - totalBytesWritten);
        } else{
            output.write(reinterpret_cast<const char *>(plain_buffer), nWritten * sizeof(uint32_t));
            totalBytesWritten += nWritten * sizeof(uint32_t);
        }

    }
}