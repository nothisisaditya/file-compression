#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include "constants.h"
using std::string, std::bitset;
typedef unsigned long ulg;

struct backref{
    ulg offset, length;
    backref(const ulg &offset, const ulg &length): offset(offset), length(length){}
    operator string() const{
        return encode_backref();
    }
    string encode_backref() const{
        return string(1, -1) + bitset<15>(offset).to_string() + string(1, -1) + bitset<8>(length-3).to_string() + string(1, -1);
    }
};

backref find_longest_match(const string &buf, ulg i, ulg window_size, ulg lookahead_size) {
    ulg max_length = 0;
    ulg max_offset = 0;
    ulg window_start = (i > window_size) ? i - window_size : 0;

    for (ulg j = window_start; j < i; ++j) {
        ulg length = 0;
        while (i + length < buf.size() && 
               length < lookahead_size && 
               buf[j + length] == buf[i + length]) {
            ++length;
        }

        if (length > max_length) {
            max_length = length;
            max_offset = i - j;
        }
    }

    return backref(max_offset, max_length);
}

string deflate_lz77(const string &infile_str) {
    string compressed;
    ulg i = 0;
    while (i < infile_str.size()) {
        backref br = find_longest_match(infile_str, i, WINDOW_SIZE, LOOKAHEAD_SIZE);
        if (br.length >= 3) {
            compressed += br.encode_backref();
            i += br.length;
        } else {
            compressed += infile_str[i];
            ++i;
        }
    }
    return compressed;
}

string inflate_lz77(const string &compressed) {
    string decompressed;
    ulg i = 0;
    while (i < compressed.size()) {
        if (compressed[i] == -1) {
            string offset_bits = compressed.substr(i + 1, 15);
            string length_bits = compressed.substr(i + 17, 8);
            ulg offset = bitset<15>(offset_bits).to_ulong();
            ulg length = bitset<8>(length_bits).to_ulong() + 3;
            
            ulg start = decompressed.size() - offset;
            for (ulg j = 0; j < length; ++j) {
                decompressed += decompressed[start + j];
            }
            i += 26;
        } else {
            decompressed += compressed[i];
            ++i;
        }
    }
    return decompressed;
}
