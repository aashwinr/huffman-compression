#ifndef HELPER
#define HELPER
#include <fstream>
#include <iostream>
using namespace std;

void error(const char* message);
class FancyInputStream {
    private:
        const char* FILENAME; // input file's name
        ifstream input_file;  // input stream from which to read
        unsigned char buffer; // bitwise buffer (holds 8 bits = 1 byte)
        int buffer_index;     // current index of bitwise buffer
    public:
        FancyInputStream(const char* filename);
        bool good() const;
        int filesize() const;
        void reset();
        int read_int();
        int read_half_int();
        int read_byte();
        int read_bit();
};

class FancyOutputStream {
    private:
        ofstream output_file; // output stream to which to write
        unsigned char buffer; // bitwise buffer (holds 8 bits = 1 byte)
        int buffer_index;     // current index of bitwise buffer

    public:
        FancyOutputStream(const char* filename);
        ~FancyOutputStream();
        bool good() const;
        void write_int(int const & num);
        void write_byte(unsigned char const & byte);
        void write_bit(int bit);
        void flush_bitwise();
        void flush();
};

class HCNode {
    public:
        int count;            // count of this node
        unsigned char symbol; // symbol of this node
        string prefix;
        bool is_leaf;
        HCNode* c0;           // pointer to '0' child
        HCNode* c1;           // pointer to '1' child
        HCNode* p;            // pointer to parent
        HCNode(int count, unsigned char symbol);
        bool operator<(const HCNode& other);
};

class HCNodePtrComp {
    public:
        bool operator()(HCNode*& lhs, HCNode*& rhs) const;
};
#endif // HELPER
