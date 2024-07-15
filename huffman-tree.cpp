#ifndef HUFFMAN
#define HUFFMAN

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <typeinfo>
// #include <algorithm> // not present in original file
using std::cout, std::endl, std::string;
using std::map, std::set, std::pair, std::make_pair;

// A node of the huffman tree. Already written, observe what is going on.
class tree{
public:
	string value = "┐"; // ┐/0/1/character. for now we just use just ┐.
	tree *left = nullptr, *right = nullptr;

	tree() {}
	// tree(char value): value(1, value) {}
	tree(char value): value(1, value) {}
	tree(tree *left): left(left) {}
	tree(tree *left, tree *right): left(left), right(right) {}
	
	static tree *merge_trees(tree *left, tree *right){
		return new tree(left, right);
	}

	void print(const string &prefix="", bool isLeftChild=false){
		cout << prefix << (prefix==""?"└──":(isLeftChild? "├─0":"└─1"));
		// cout << "value:" << value << "|" << endl;
		if (isLeaf())
			cout << "─[" << value << "]" << endl;
		else
			cout << value << endl;
		if (left)
			left->print(prefix + (isLeftChild?"│  ":"   "), true);
		if (right)
			right->print(prefix + (isLeftChild?"│  ":"   "), false);
	}
	bool isLeaf() const
	{
		return !(left && right);
	}
	void get_codes(map<char, string> &map_in_progress,
		       const string &prefix="") const
	{
		if (left) {
			if (left->isLeaf())
				map_in_progress[left->value[0]] = prefix + "0";
			else
				left->get_codes(map_in_progress, prefix + "0");
		}
		if (right) {
			if (right->isLeaf())
				map_in_progress[right->value[0]] = prefix + "1";
			else
				right->get_codes(map_in_progress, prefix + "1");
		}
	}
	~tree()
	{
		delete left; delete right;
	}
};

struct CompareTrees {
	bool operator()(const std::pair<double, tree> a, const std::pair<double, tree> b) const
	{
		return a.first < b.first;
	}
};

// This returns the huffman tree using passed map of character to frequency
tree *huffman(const map<char, double> &freq_map)
{
	std::set<std::pair<double, tree*>, CompareTrees> trees;
	for (const std::pair<const char, double> &entry : freq_map) {
		tree *x = new tree(entry.first);
		trees.insert(std::make_pair(entry.second, x));
	}
	while (trees.size() > 1) {
		std::set<std::pair<double, tree*>, CompareTrees>::iterator a = trees.begin();
		trees.erase(trees.begin());
		std::set<std::pair<double, tree*>, CompareTrees>::iterator b = trees.begin();
		trees.erase(trees.begin());

		trees.insert(std::make_pair(a->first + b->first, tree::merge_trees(a->second, b->second)));
	}
	return trees.begin()->second;
}

// Compute the frequencies of characters in the string. For part 3, make sure to
// skip over backreferences and also set highest priority to the special
// character.
map<char, double> get_freqs(const string &buf)
{
	std::map<char, double> mp;
	for (std::string::const_iterator it = buf.begin(); it != buf.end(); it++) {
		mp[*it]++;
	}
	return mp;
}

#endif
