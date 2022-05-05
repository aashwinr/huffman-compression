#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include "../include/HCTree.hpp"

const string usage = "USAGE: ./compress <original_file> <compressed_file>";

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

vector<int> get_counts(const string& filename) {
    FancyInputStream ifstream(filename.c_str());
    vector<int> counts(256, 0);
    while(true) {
        unsigned char byte = ifstream.read_byte();
        if(ifstream.good()) {
            counts[byte]++;
        } else {
            break;
        }
    }
    return counts;
}

int main(int argc, char** argv) {
    verify_args(argc, argv);
    string file_name = argv[1];

    HCTree ht;
    vector<int> counts = get_counts(file_name);
    ht.build(counts);
    FancyInputStream fin(argv[1]);
    FancyOutputStream fout(argv[2]);
    auto tree = ht.get_internal_priority_queue();

    if(fin.filesize() <= 0)
        return 0;

    ht.write_header(fout);

    while(fin.good()){
        unsigned char inp_byte = fin.read_byte();
        if(fin.good())
            ht.encode(inp_byte, fout);
    }

}
