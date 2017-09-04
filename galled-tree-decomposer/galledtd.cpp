/*
 * main.cpp
 *
 *  Created on: Jul 18, 2017
 *      Author: ramesh
 */

#include "network.h"
#include "nex_parse.h"
#include "utils.h"

#include <set>


network_t* get_Nse(network_t* n) {
	network_t* Nse = new network_t(n);
	for (node_t* node : Nse->nodes) {
		if (node->hybrid()) {
			node->parents[1]->remove_child(node);
		}
	}
	return Nse;
}

network_t* get_Nsw(network_t* n) {
	network_t* Nsw = new network_t(n);
	for (node_t* node : Nsw->nodes) {
		if (node->hybrid()) {
			node->parents[0]->remove_child(node);
		}
	}
	return Nsw;
}

network_t* get_Ns(network_t* n) {
	network_t* Ns = new network_t(n);
	for (node_t* node : Ns->nodes) {
		if (node->hybrid()) {
			node_t* split = find_split(node);

			// remove from left and right parent
			node->parents[1]->remove_child(node);
			node->parents[0]->remove_child(node);

			assert(node->parents.empty());
			split->add_child(node);
		}
	}
	return Ns;
}

network_t* get_Ndelta(network_t* n, bool get_NL, bool dd) {
	network_t* Ndelta = new network_t(n);
	int n_nodes = Ndelta->nodes.size();
	for (int i = 0; i < n_nodes; i++) {
		node_t* node = Ndelta->nodes[i];
		if (node == NULL || !node->hybrid()) continue;
		node_t* split = find_split(node);

		// collect left side trees
		std::vector<node_t*> side_trees;
		node_t* p = node->parents[!get_NL], *prev = node;
		while (p != split) {
			p->remove_child(prev);
			for (node_t* child : p->children) {
				side_trees.push_back(child);
			}

			prev = p;
			p = p->parents[0];
		}
		Ndelta->nodes[prev->id] = prev;
		for (node_t* st : side_trees) {
			st->parents[0]->remove_child(st);
		}

		// disconnect non-delta path
		node->parents[0]->remove_child(node);

		// node in-between split and hybrid node on delta path
		prev->add_child(node);

		if (dd) {
			// attach side trees to intermediate node
			node_t* v = Ndelta->add_node();
			prev->add_child(v);
			for (node_t* st : side_trees) {
				v->add_child(st);
			}
		} else {
			// attach side trees directly
			for (node_t* st : side_trees) {
				prev->add_child(st);
			}
		}
	}
	return Ndelta;
}

//network_t* get_Ndeltadelta(network_t* n, bool get_NLL) {
//	network_t* Ndeltadelta = new network_t(n);
//	for (node_t* node : Ndeltadelta->nodes) {
//		if (node == NULL || !node->hybrid()) continue;
//		node_t* split = find_split(node);
//
//		// collect left side trees
//		std::vector<node_t*> side_trees;
//		node_t* p = node->parents[!get_NLL], *prev = node;
//		while (p != split) {
//			for (node_t* child : p->children) {
//				if (child != prev) {
//					side_trees.push_back(child);
//					child->parents.clear();
//				}
//			}
//			Ndeltadelta->nodes[p->id] = NULL;
//			prev = p;
//			p = p->parents[0];
//		}
//		Ndeltadelta->nodes[prev->id] = prev;
//
//		// disconnect delta path
//		node->parents[0]->remove_child(node);
//		node->parents[1]->remove_child(node);
//		prev->children.clear();
//		if (!side_trees.empty()) {
//			node_t* new_node = Ndeltadelta->add_node();
//			for (node_t* st : side_trees) {
//				new_node->add_child(st);
//			}
//			prev->add_child(new_node);
//		}
//		prev->add_child(node);
//	}
//
//	return Ndeltadelta;
//}

node_t* make_caterpillar(network_t* n, std::vector<node_t*> nodes) {
	node_t* curr_cat_root = NULL;
	for (node_t* child : nodes) {
		if (curr_cat_root == NULL) {
			curr_cat_root = child;
		} else {
			node_t* new_cat_root = n->add_node();
			new_cat_root->add_child(curr_cat_root);
			new_cat_root->add_child(child);
			curr_cat_root = new_cat_root;
		}
	}
	return curr_cat_root;
}

network_t* get_NBC(network_t* n, bool getNB) {
	network_t* nbc = new network_t(n);
	int n_nodes = nbc->nodes.size();
	bool* split_nodes = new bool[n_nodes];
	std::fill(split_nodes, split_nodes+n_nodes, false);
	for (int i = 0; i < n_nodes; i++) {
		node_t* node = nbc->nodes[i];
		if (!node->hybrid()) continue;
		node_t* split = find_split(node);
		node_t* lp = node->parents[0];
		node_t* rp = node->parents[1];
		while (lp->parents[0] != split) lp = lp->parents[0];
		while (rp->parents[0] != split) rp = rp->parents[0];

		// make caterpillar out of non-gall children of split node
		std::vector<node_t*> split_children;
		for (node_t* child : split->children) {
			if (child != lp && child != rp) {
				split_children.push_back(child);
			}
		}
		for (node_t* child : split_children) {
			split->remove_child(child);
		}
		split->remove_child(lp);
		split->remove_child(rp);

		node_t* cat = make_caterpillar(nbc, split_children);
		if (cat != NULL) {
			split->add_child(cat);
		}

		node_t* u = nbc->add_node();
		u->add_child(lp);
		u->add_child(rp);
		split->add_child(u);

		node->parents[1]->remove_child(node);
		node->parents[0]->remove_child(node);
		if (getNB) {
			split->add_child(node);
		} else {
			u->add_child(node);
		}
		split_nodes[split->id] = true;
	}

	// make caterpillar out of non-split non-binary nodes
	for (int i = 0; i < n_nodes; i++) {
		node_t* node = nbc->nodes[i];
		if (!split_nodes[i] && node->children.size() > 2) {
			std::vector<node_t*> children;
			for (node_t* child : node->children) {
				children.push_back(child);
			}
			for (node_t* child : children) {
				node->remove_child(child);
			}

			node->add_child(make_caterpillar(nbc, children));
		}
	}

	return nbc;
}


// counts
// a) leaves in subtrees "sprouting" from a gall
// b) number of pairs of leaves in a same subtree
std::pair<ull,ull> count_galled_path(node_t* start, node_t* hybrid, node_t* split, ull* count) {
	ull onpath = 0, same = 0;
	node_t* prev = hybrid;
	while (start != split) {
		for (node_t* child : start->children) {
			if (child != prev) {
				onpath += count[child->id];
				same += comb2(count[child->id]);
			}
		}
		prev = start;
		start = start->parents[0];
	}
	return std::make_pair(onpath, same);
}


ull count_triplets(network_t* n) {
	ull tot = comb3(n->leaves);

	network_t* ns = get_Ns(n);
	ull* count = make_count(ns);
	delete ns;

	for (node_t* node : n->nodes) {
		if (!node->hybrid()) continue;
		node_t* split = find_split(node);
		ull hybrid_count = count[node->id];

		node_t* lp = node->parents[0],* rp = node->parents[1];
		std::pair<ull,ull> lp_counts = count_galled_path(lp, node, split, count);
		std::pair<ull,ull> rp_counts = count_galled_path(rp, node, split, count);

		// type-A
		ull typeA = hybrid_count * (comb2(lp_counts.first) - lp_counts.second);
		typeA += hybrid_count * (comb2(rp_counts.first) - rp_counts.second);

		// get attached to split node but not in gall
		ull fromsplit = 0;
		while (lp->parents[0] != split) lp = lp->parents[0];
		while (rp->parents[0] != split) rp = rp->parents[0];
		for (node_t* child : split->children) {
			if (child != lp && child != rp) fromsplit += count[child->id];
		}

		// type-B
		ull typeB = hybrid_count * fromsplit * (lp_counts.first + rp_counts.first);

		// type-C
		ull typeC = hybrid_count * lp_counts.first * rp_counts.first;

		tot += typeA + typeB + typeC;
	}

	delete[] count;

	return tot;
}


// if one path of the gall is empty, it can cause problems: fix by inserting fake node in it
void fix_empty_gall_paths(network_t* n) {
	int n_nodes = n->nodes.size();
	for (int i = 0; i < n_nodes; i++) {
		node_t* node = n->nodes[i];
		if (!node->hybrid()) continue;
		node_t* split = find_split(node);
		if (node->parents[0] == split) {
			node_t* v = n->add_node();
			replace_first(split->children, node, v);
			v->children.push_back(node);
			v->parents.push_back(split);
			replace_first(node->parents, split, v);
		}
		if (node->parents[1] == split) {
			node_t* v = n->add_node();
			replace_first(split->children, node, v);
			v->children.push_back(node);
			v->parents.push_back(split);
			replace_first(node->parents, split, v);
		}
	}
}


int main(int argc, char* argv[]) {

	network_t* n1 = parse_nex(argv[1]);
	network_t* n2 = parse_nex(argv[2]);
	std::string prefix = argv[3];

	fix_empty_gall_paths(n1);
	fix_empty_gall_paths(n2);

	ull n1_trips = count_triplets(n1);
	ull n2_trips = count_triplets(n2);

	std::ofstream fout;

	fout.open(prefix + "N1_Nse");
	fout << to_newick(get_Nse(n1)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_Nsw");
	fout << to_newick(get_Nsw(n1)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_Ns");
	fout << to_newick(get_Ns(n1)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_NL");
	fout << to_newick(get_Ndelta(n1, true, false)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_NLL");
	fout << to_newick(get_Ndelta(n1, true, true)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_NR");
	fout << to_newick(get_Ndelta(n1, false, false)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_NRR");
	fout << to_newick(get_Ndelta(n1, false, true)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_NB");
	fout << to_newick(get_NBC(n1, true)) << std::endl;
	fout.close();

	fout.open(prefix + "N1_NC");
	fout << to_newick(get_NBC(n1, false)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_Nse");
	fout << to_newick(get_Nse(n2)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_Nsw");
	fout << to_newick(get_Nsw(n2)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_Ns");
	fout << to_newick(get_Ns(n2)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_NL");
	fout << to_newick(get_Ndelta(n2, true, false)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_NLL");
	fout << to_newick(get_Ndelta(n2, true, true)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_NR");
	fout << to_newick(get_Ndelta(n2, false, false)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_NRR");
	fout << to_newick(get_Ndelta(n2, false, true)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_NB");
	fout << to_newick(get_NBC(n2, true)) << std::endl;
	fout.close();

	fout.open(prefix + "N2_NC");
	fout << to_newick(get_NBC(n2, false)) << std::endl;
	fout.close();

	std::cout << n1_trips << " " << n2_trips << std::endl;

}
