#include "../include/HCTree.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

void delete_node(HCNode* root) {
    if(root == nullptr)
        return;
    delete_node(root -> c0);
    delete_node(root -> c1);
    delete root;
}

HCTree::~HCTree() {
    delete_node(this -> root);
}

void add_prefixes(HCNode* root) {
    if(root == nullptr)
        return;
    if(root -> p == nullptr) {
        root -> prefix = "";
    } else {
        if(root -> p -> c0 == root) {
            root -> prefix = root -> p -> prefix + '0';
        } else {
            root -> prefix = root -> p -> prefix + '1';
        }
    }
    add_prefixes(root -> c0);
    add_prefixes(root -> c1);
}

void HCTree::build(const vector<int>& freqs) {

    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    int ctr = -1;

    for(const auto& freq: freqs) {
        ctr++;
        HCNode* newnode = new HCNode(freq, (unsigned char)(ctr));
        newnode -> is_leaf = true;
        if(freq <= 0) {
            delete newnode;
            continue;
        }
        leaves[ctr] = newnode;
        pq.push(newnode);
        this -> m_nodes.push(newnode);
    }
    
    while(pq.size() > 1) {
        HCNode* right = pq.top(); pq.pop();
        HCNode* left = pq.top(); pq.pop();
        HCNode* newnode = new HCNode(right->count + left -> count, '\0');
        newnode -> c1 = right;
        newnode -> c0 = left;
        right -> p = left -> p = newnode;
        pq.push(newnode);
    }

    if(pq.size() > 0) {
        this -> root = pq.top();
        this -> root -> p = nullptr;
        add_prefixes(this -> root);
        pq.pop();
    }

}

bool get_nth_bit_int(int c, unsigned char n) {
    return (c >> (n)) & 1;
}

bool get_nth_bit_byte(unsigned char c, unsigned char n) {
    return (c >> (n)) & 1;
}

void HCTree::encode(unsigned char symbol, FancyOutputStream &out) const {    
    string symbol_prefix = this -> leaves[(int)symbol] -> prefix;
    for(const auto& i: symbol_prefix) {
        switch (i) {
            case '0':
                out.write_bit(0);
                break;
            case '1':
                out.write_bit(1);
                break;
            default:
                cerr << "Invalid Prefix Character detected" << endl;
                break;
        }
    }
}

vector<int> HCTree::read_header(FancyInputStream &infile) {
    vector<int> freqs(256,0);
    int num_three_sized_headers = infile.read_int();
    unsigned char symbol;
    int freq = 0;
    while(num_three_sized_headers--) {
        symbol = infile.read_byte();
        freq = (((freq | infile.read_byte() ) << 8) | infile.read_byte());
        freqs[symbol] = freq;
        freq = 0;
    }
    return freqs;
}

void HCTree::decode(FancyInputStream &inf, FancyOutputStream &of) const {

    inf.reset();
    int header_size = inf.read_int()*3;
    while(header_size--) inf.read_byte();

    HCNode* temp = this -> root;
    while(inf.good()) {
        int inpbit = inf.read_bit();
        if(!inf.good()) {
            return;
        }
        if(inpbit) {
            temp = temp -> c1;
            if(temp -> is_leaf) {
                of.write_byte(temp -> symbol);
                temp = this -> root;
            }
        } else {
            temp = temp -> c0;
            if(temp -> is_leaf) {
                of.write_byte(temp -> symbol);
                temp = this -> root;
            }
        }
    }
    
}

int get_valid_char_count(HCTree* tree) {
    int ret = 0;
    for(const auto& i: tree -> leaves) {
        ret += (i != nullptr);
    }
    return ret;
}

vector<unsigned char> get_two_ending_bytes(unsigned int num) {
    vector<unsigned char> two_ending_bytes(2,0);
    unsigned char mask = 255;
    two_ending_bytes[0] = (((unsigned char)(num >> 8)) & mask);
    two_ending_bytes[1] = (((unsigned char)(num)) & mask);
    return two_ending_bytes;
}

void HCTree::write_header(FancyOutputStream &of) {
    of.write_int(get_valid_char_count(this));
    for(const auto& i: this -> leaves) {
        if(i != nullptr) {
            of.write_byte(i -> symbol);
            for(const auto &j: get_two_ending_bytes(i -> count)) {
                if(i -> count > 0) {
                    of.write_byte(j);
                }
            }
        }
    }
}
