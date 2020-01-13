#include <iostream>
#include <cstring>

using namespace std;

#include "fileSplitter.h"
#include "polyfier.h"
#include "unpolyfier.h"
#include "fileMerger.h"

void usage(){
    string usage = "Byte Scissor v0.1\n"
                    "Usage: \n"
                   "  split <path> <number of fragments> <number of required fragments to merge> \n"
                   "  split-n <number> <number of fragments> <number of required fragments to merge> \n"
                   "  merge --output <output path> --input <input paths>...\n"
                   "  merge-n <id> <values>... \n"
                   ;
    cout << usage << endl;
}

int parseMerge(int argc, char** argv);

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

if (strcmp(argv[1], "merge-n") == 0){
    if (argc % 2 != 0 || argc == 2) {
        usage();
        return 1;
    }

    vector<uint64_t> xs((argc-2)/2);
    vector<uint64_t> points((argc-2)/2);

    for (int i = 0; i < (argc - 2) / 2; ++i) {
        xs[i] = stoull(argv[i * 2 + 2]);
        points[i] = stoull(argv[i * 2 + 3]);
    }
    uint32_t recovered = unpolyfier::merge(points, xs);
    cout << recovered << endl;
    return 1;
}

if (strcmp(argv[1], "merge") == 0) {
    int ret = parseMerge(argc, argv);
    if (ret != 0) {
        usage();
    }
    return ret;
}

if (strcmp(argv[1], "debug") == 0){
    mergeFiles({"./infoS.txt.1.part", "./infoS.txt.3.part", "./infoS.txt.4.part"}, "output.txt");
    return 0;
}
usage();
return 1;

}

int parseMerge(int argc,char** argv) {
    if (argc < 6) {
        return 1;
    }

    if ((strcmp(argv[2], "--output") != 0) && (strcmp(argv[2], "-o") != 0) ) {
        return 1;
    }

    string outPath(argv[3]);

    if ((strcmp(argv[4], "--input") != 0) && (strcmp(argv[4], "-i") != 0) ) {
        return 1;
    }

    vector<string> inPaths(argc - 5);
    for (int i = 5; i < argc; i++) {
        inPaths[i - 5] = argv[i];
    }

    try{
    mergeFiles(inPaths, outPath);
    } catch (exception &a) {
        cout << "Unable to merge files (exception thrown): " << a.what() << endl;
        return 1;
    }

    return 0;

}

