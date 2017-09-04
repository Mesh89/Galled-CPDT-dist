#ifndef NEX_PARSE_H_
#define NEX_PARSE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>

#include "network.h"


std::string strip_cr(std::string str) {
	str.erase(remove(str.begin(), str.end(), '\015'), str.end());
	return str;
}

bool isdigit(char c) {
	return c >= '0' && c <= '9';
}

int parse_int(const char*& str) {
	int i = 0;
	while (isdigit(*str)) {
		i = i*10 + (*str-'0');
		str++;
	}
	return i;
}


node_t* parse_tree_support(const char*& str, network_t* n, std::map<int, node_t*>& hybrid_nodes) {

	assert(*str == '(');
	str++;


	node_t* curr_node = n->add_node();

	assert(*str == '(' || isdigit(*str) || *str == '#');
	while (true) {
		node_t* subtree;
		if (*str == '(') {
			subtree = parse_tree_support(str, n, hybrid_nodes);
		} else if (*str == '#') {
			str++;
			int h_id = parse_int(str);
			subtree = hybrid_nodes[h_id];
		} else {
			subtree = n->add_node(parse_int(str)-1);
		}
		curr_node->add_child(subtree);

		if (*str == '#') {
			str++;
			int h_id = parse_int(str);
			hybrid_nodes[h_id] = subtree;
		}

		assert(*str == ',' || *str == ')');
		if (*str == ',') {
			str++;
		} else if (*str == ')') {
			str++;
			break;
		}
	}

	return curr_node;
}


// this (actually the methods above) parses a string representation of a binary tree
// and returns the tree nodes structure (stored in a vector)
network_t* parse_tree(std::string s) {
	const char* cstr = s.c_str();
	std::map<int, node_t*> hybrid_nodes;
	network_t* n = new network_t;
	parse_tree_support(cstr, n, hybrid_nodes);
	return n;
}


network_t* parse_nex(std::string filename) {

	std::ifstream fin(filename.c_str());

	// parse tree
    std::string line;
	getline(fin, line, ';');
	line = strip_cr(line);

	std::vector<node_t*> nodes;
	std::map<int, node_t*> hybrid_nodes;
	network_t* n = parse_tree(line);
	return n;
}



#endif /* NEX_PARSE_H_ */
