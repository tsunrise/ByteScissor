#include <iostream>
#include <cstring>

using namespace std;

#include "fileSplitter.h"
#include "polyfier.h"

void usage(){
    string usage = "Usage: \n"
                   "  split <path> <nCopies> <nRequired> \n"
                   "  split-n <number> <nCopies> <nRequired>";
    cout << usage << endl;
}

int main(int argc, char** argv) {

if (argc < 2) {
    usage();
    return 1;
}

if (strcmp(argv[1], "split") == 0) {
    if (argc != 5) {
        usage();
        return 1;
    }

    fileSplitter fs(argv[2], atoi(argv[3]), atoi(argv[4]));
    fs.run();
    return 0;
}

if (strcmp(argv[1], "split-n") == 0) {
    if (argc != 5) {
        usage();
        return 1;
    }
    uint8_t nCopies = atoi(argv[3]);
    polyfier pf(stoul(argv[2]), nCopies, atoi(argv[4]));
    for (uint8_t i = 0; i < nCopies; i++) {
        cout << pf.getBuffer()[i] << ' ';
    }
    cout << endl;
    return 0;
}

usage();
return 1;

}

