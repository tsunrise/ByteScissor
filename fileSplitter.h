//
// Created by tom10 on 2020/1/5.
//
#include <string>
#include <fstream>
#include <vector>

using namespace std;
#ifndef POLY_SPLIT_FILESPLITTER_H
#define POLY_SPLIT_FILESPLITTER_H


class fileSplitter {
    ifstream input;
    vector<ofstream*> outputs;
    string path;
    uint8_t nCopies;
    uint8_t nRequired;
public:
    fileSplitter(string path, uint8_t nCopies, uint8_t nRequired);
    void run();
    ~fileSplitter();

private:
    fileSplitter() = default;
};


#endif //POLY_SPLIT_FILESPLITTER_H
