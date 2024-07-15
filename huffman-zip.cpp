#include "huffman-tree.cpp"
#include "constants.h"

// given a compressed binary string, convert it to the original string using the huffman tree
string inflate(const string &compressed, const tree *huffman_tree)
{
        std::string res;
        std::string::const_iterator it = compressed.begin();
        const tree *node = huffman_tree;
        while (it != compressed.end()) {
                if (*it == '0') {
                        node = node->left;
                        res += node->value;
                        node = huffman_tree;
                } else {
                        node = node->right;
                }
        }
        return res;
}

// given a string, compress it using the huffman tree and return the resulting binary string
string deflate(const string &buf, tree* &huffman_tree)
{
        std::string res;
        for (std::string::const_iterator it = buf.begin(); it != buf.end(); it++) {
                const tree *node = huffman_tree;
                while (node->left) {
                        if (*it == *(node->left->value.begin())) {
                                node = node->left;
                                res += "0";
                        } else {
                                node = node->right;
                                res += "1";
                        }
                }
        }
        return res;
}