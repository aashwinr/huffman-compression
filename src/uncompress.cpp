#include <iostream>
#include <vector>
#include <stack>
#include "../include/Helper.hpp"
#include "../include/HCTree.hpp"

using namespace std;

const string usage = "USAGE: ./uncompress <original_file> <compressed_file>";

void verify_args(int argc, char** argv) {
    if(argc != 3) {
        cerr << "ERROR: Incorrect parameters\n";
        cerr << usage << '\n';
        exit(1);
    }
    FILE* infile = fopen(argv[1], "r");
    if(!infile) {
        cerr << "ERROR: File not found: " << argv[1] << "\n";
        exit(1);
    }
    fclose(infile);
}

int main(int argc, char** argv) {
    verify_args(argc, argv);
    FancyInputStream ifile(argv[1]);
    FancyOutputStream ofile(argv[2]);
    if(ifile.filesize() <= 0)
        return 0;
    HCTree ht;
    ht.build(ht.read_header(ifile));
    ht.decode(ifile, ofile);
}
