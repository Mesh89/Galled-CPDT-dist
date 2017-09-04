/*
 * structures.h
 *
 *  Created on: Jul 19, 2017
 *      Author: ramesh
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <algorithm>

typedef unsigned long long ull;

struct node_t {
	int id;
	std::vector<node_t*> parents;
	std::vector<node_t*> children;
	int taxa = -1;

	bool hybrid() {
		return parents.size() > 1;
	}
	bool leaf() {
		return taxa >= 0;
	}

	void add_child(node_t* child) {
		children.push_back(child);
		child->parents.push_back(this);
	}

	void remove_child(node_t* child) {
		children.erase(std::remove(children.begin(), children.end(), child));
		child->parents.erase(std::remove(child->parents.begin(), child->parents.end(), this));
	}
};

struct network_t {
	std::vector<node_t*> nodes;
	//std::map<int, tree_node*> hybrid_nodes;
	int leaves = 0;

	network_t() {}

	network_t(network_t* n) {
		leaves = n->leaves;
		for (node_t* node : n->nodes) {
			add_node(node->taxa);
		}

		for (size_t i = 0; i < n->nodes.size(); i++) {
			for (node_t* parent : n->nodes[i]->parents) {
				nodes[i]->parents.push_back(nodes[parent->id]);
			}
			for (node_t* child : n->nodes[i]->children) {
				nodes[i]->children.push_back(nodes[child->id]);
			}
		}
	}

	node_t* add_node() {
		node_t* node = new node_t;
		node->id = nodes.size();
		nodes.push_back(node);
		return node;
	}
	node_t* add_node(int taxa) {
		node_t* node = add_node();
		node->taxa = taxa;
		leaves++;
		return node;
	}
};


ull make_count(node_t* node, ull* count) {
	if (node->leaf()) {
		count[node->id] = 1;
	} else {
		for (node_t* child : node->children) {
			count[node->id] += make_count(child, count);
		}
	}
	return count[node->id];
}

ull* make_count(network_t* n) {
	ull* count = new ull[n->nodes.size()];
	std::fill(count, count+n->nodes.size(), 0);
	make_count(n->nodes[0], count);
	return count;
}


// assumes galled tree, i.e. max 2 parents and 1 hybrid node per gall
node_t* find_split(node_t* hybrid) {
	if (!hybrid->hybrid()) return NULL;

	node_t* lp = hybrid->parents[0];
	node_t* rp = hybrid->parents[1];
	while (lp != rp) {
		if (lp->id > rp->id) {
			assert(lp->parents.size() == 1);
			lp = lp->parents[0];
		}
		else {
			assert(rp->parents.size() == 1);
			rp = rp->parents[0];
		}
	}
	return lp;
}


std::string to_newick(node_t* curr, bool* h_printed);

std::string to_newick_supp(node_t* curr, bool* h_printed) {
	if (curr->taxa >= 0) {
		return std::to_string(curr->taxa+1);
	} else if (curr->children.size() == 1) {
		return to_newick(curr->children[0], h_printed);
	} else if (curr->children.empty()) {
		assert(false);
	}

	std::string str = "(" + to_newick(curr->children[0], h_printed);
	for (size_t i = 1; i < curr->children.size(); i++) {
		node_t* child = curr->children[i];
		str += "," + to_newick(child, h_printed);
	}
	return str + ")";
}

std::string to_newick(node_t* curr, bool* h_printed) {
	if (curr->parents.size() > 1) {
		if (h_printed[curr->id]) {
			return "#" + std::to_string(curr->id);
		} else {
			h_printed[curr->id] = true;
			return to_newick_supp(curr, h_printed) + "#" + std::to_string(curr->id);
		}
	} else {
		return to_newick_supp(curr, h_printed);
	}
}

std::string to_newick(network_t* gt) {
	int n_nodes = gt->nodes.size();
	bool* h_printed = new bool[n_nodes];
	std::fill(h_printed, h_printed+n_nodes, false);

	// cut off loose branches
	ull* count = make_count(gt);
	for (int i = 0; i < n_nodes; i++) {
		if (count[i] == 0 && !gt->nodes[i]->parents.empty()) {
			node_t* loose_branch = gt->nodes[i];
			loose_branch->parents[0]->remove_child(loose_branch);
		}
	}
	return to_newick(gt->nodes[0], h_printed) + ";";
}


void print(network_t* n) {
	for (node_t* node : n->nodes) {
		if (node == NULL) {
			std::cout << "NULL" << std::endl;
			continue;
		}
		std::cout << node->id << " children=( ";
		for (node_t* child : node->children) {
			std::cout << child->id << " ";
		}
		std::cout << ") ";
		std::cout << node->id << " parents=( ";
		for (node_t* child : node->parents) {
			std::cout << child->id << " ";
		}
		std::cout << ") ";
		if (node->taxa >= 0) {
			std::cout << node->taxa;
		}
		std::cout << std::endl;
	}
}


#endif /* NETWORK_H_ */
