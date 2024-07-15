#include "rle.h"
#include <math.h>

// Constructor
rle::rle(int gamma): gamma(gamma){}

// Convert integer to gamma-bit binary string
string rle::f(int i)
{
	std::string asdf = "";
	while (i) {
		asdf.insert(0, std::to_string(i % 2));
		i /= 2;
	}
	while (asdf.length() < gamma) {
		asdf.insert(0, std::to_string(0));
	}
	return asdf;
}

// Convert gamma-bit binary string to integer
int rle::g(string s)
{
	int res = 0;
	for (int i = 0; i < s.length(); i++) {
		res += (s[i] - '0') * pow(2, gamma - i - 1);
	}
	return res;
}

// Compress the input file
string rle::compress(string input_string)
{
	std::string output = "";
	output += input_string[0];
	char prev = input_string[0];
	int j = 1;
	for (int i = 1; i < input_string.length(); i++) {
		if (input_string[i] == prev) {
			j++;
		} else {
			while (j > pow(2, gamma) - 1) {
				output += f(pow(2, gamma) - 1);
				output += f(0);
				j -= pow(2, gamma) - 1;
			}
			output += f(j);
			j = 1;
			prev = input_string[i];
		}
	}
	return output;
}

// Decompress the input file
string rle::decompress(string input_string)
{
	std::string output = "";
	bool prev = input_string[0] - '0';
	for (int i = 1; i < input_string.length();) {
		std::string asdf = "";
		for (int j = 0; j < gamma; j++)
			asdf += input_string[i++];
		for (int j = 0; j < g(asdf); j++)
			output += std::to_string(prev);
		prev = !prev;
	}
	std::cerr << input_string << "\n";
	return output;
	// return input_string;
}
