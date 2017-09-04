/*
 * utils.h
 *
 *  Created on: Jul 20, 2017
 *      Author: ramesh
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>


typedef unsigned long long ull;

inline ull comb2(ull n) {
	return n*(n-1)/2;
}

inline ull comb3(ull n) {
	ull n1 = n-1, n2 = n-2;
	if (n%2 == 0) {
		n /= 2;
	} else {
		n1 /= 2;
	}
	if (n%3 == 0) {
		n /= 3;
	} else if (n1%3 == 0) {
		n1 /= 3;
	} else {
		n2 /= 3;
	}
	return n*n1*n2;
}

void replace_first(std::vector<node_t*>& v, node_t* old_node, node_t* new_node) {
	for (size_t i = 0; i < v.size(); i++) {
		if (v[i] == old_node) {
			v[i] = new_node;
			return;
		}
	}
}


#endif /* UTILS_H_ */
