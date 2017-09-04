#include <iostream>
#include <random>
#include <set>
#include <cassert>
#include <unistd.h>

#include "nex_parse.h"

using namespace std;

pair<node_t*, node_t*> make_caterpillar(network_t* n, std::vector<node_t*> nodes) {
	node_t* curr_cat_root = NULL,* lowest_int_node = NULL;
	for (node_t* child : nodes) {
		node_t* new_cat_root = n->add_node();
		if (curr_cat_root != NULL) {
			new_cat_root->add_child(curr_cat_root);
		} else {
			lowest_int_node = new_cat_root;
		}
		new_cat_root->add_child(child);
		curr_cat_root = new_cat_root;
	}
	if (lowest_int_node == NULL) curr_cat_root = lowest_int_node = n->add_node();
	return make_pair(curr_cat_root, lowest_int_node);
}

void check(network_t* n) {
	for (node_t* node : n->nodes) {
		if (!node->leaf() && node->children.empty()) {
			sleep(1000000);
			assert(false);
		}
	}
}

int main() {

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> ddis(0.0, 1.0);
	uniform_real_distribution<> pdis(0.0, 0.4);
	uniform_real_distribution<> qdis(0.0, 1.0);
	
	string tree_str;
	cin >> tree_str;
	network_t* tree = parse_tree(tree_str);
	uniform_int_distribution<> idis(0, tree->nodes.size()-1);

	set<int> in_gall;

	int galls = log2(tree->leaves), tries = 100000; // sometimes, especially on smaller trees, it can be (nearly) impossible to make another gall; 
//if we fail for "tries" times, we stop trying
//	double p = 0.2, q = 0.5;
	while (galls > 0 && tries > 0) {
		tries--;
		int r = idis(gen);
		node_t* h_node = tree->nodes[r];
		if (h_node->taxa >= 0) continue;

		if (h_node->parents.empty() || h_node->parents[0]->parents.empty()) continue;
		node_t* s_node = h_node->parents[0]->parents[0];
		double p = pdis(gen);
		while (s_node != NULL) {
			s_node = s_node->parents.empty() ? NULL : s_node->parents[0];
			if (ddis(gen) < p) break;
		}
		if (s_node == NULL) continue;

		node_t* curr = h_node;
		while (curr != s_node) {
			if (in_gall.count(curr->id) > 0) {
				s_node = NULL;
				break;
			}
			curr = curr->parents[0];
		}
		if (s_node != NULL && in_gall.count(s_node->id) > 0) s_node = NULL;

		if (s_node != NULL) {
			curr = h_node->parents[0];
			node_t* prev = h_node;
			std::vector<node_t*> side_trees;
			double q = qdis(gen);
			while (curr != s_node) {
				for (node_t* child : curr->children) {	
					if (child != prev && ddis(gen) < q) {
						side_trees.push_back(child);
					}
				}
				prev = curr;
				curr = curr->parents[0];
			}
			for (node_t* st : side_trees) {
				st->parents[0]->remove_child(st);
			}

			pair<node_t*, node_t*> cat = make_caterpillar(tree, side_trees);
			s_node->add_child(cat.first);
			cat.second->add_child(h_node);

			in_gall.insert(h_node->id);
			node_t* lp = h_node->parents[0];
			node_t* rp = h_node->parents[1];
			while (lp != s_node) {
				in_gall.insert(lp->id);
				lp = lp->parents[0];
			}
			while (rp != s_node) {
				in_gall.insert(rp->id);
				rp = rp->parents[0];
			}
			in_gall.insert(s_node->id);

			galls--;
		}
	}

	cout << to_newick(tree) << endl;
}

