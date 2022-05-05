#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include "Helper.hpp"
using namespace std;

class HCTree {
    private:
        priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> m_nodes;
    public:
        HCNode* root;
        vector<HCNode*> leaves;
        HCTree() : root(nullptr) {
            leaves = vector<HCNode*>(256, nullptr);
        }
        ~HCTree();
        void build(const vector<int>& freqs);
        void encode(unsigned char symbol, FancyOutputStream & out) const;
        void decode(FancyInputStream & in, FancyOutputStream &of) const;
        void write_header(FancyOutputStream &of);
        vector<int> read_header(FancyInputStream &infile);
        priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> get_internal_priority_queue() { return this -> m_nodes; };
        vector<HCNode*> get_leaves() { return this -> leaves; }
};
#endif // HCTREE_HPP
