#include <cstdlib>
#include <iostream>
#include <cassert>
#include <ctime>

#include "nex_parse.h"
#include "cpdt-dist-bin.h"
#include "cpdt-dist.h"

#define NDEBUG

using namespace std;

int main(int argc, char* argv[]) {

	tree* tree1 = parse_nex(argv[1]);
    tree* tree2 = parse_nex(argv[2]);
    
    std::pair<ull,ull> result = cpdt_dist::triplet_distance(tree1, tree2);

    cout << result.first << " " << result.second << endl;
}
