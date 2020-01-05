#include <iostream>
#include <cstring>

using namespace std;

#include "fileSplitter.h"

void usage(){
    string usage = "Usage: \n"
                   "  split <nCopies> <nRequired>";
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

usage();
return 1;

}

