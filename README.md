# Galled-CPDT-dist

This software computes the Rooted Triplet Distance between two galled trees. It implements the algorithm described in [1].

## Getting started

In order to compile and run the software you will need a C++11-compliant compiler and a Bash shell.

1) Open a shell and navigate it to the main folder of the project
2) Compile the galled-tree-decomposer/galledtd.cpp into an executable called GalledTD and place it in the main folder
3) Compile modified-cpdt-dist/cpdt-dist.cpp into an executable called trip-dist and place it in the main folder

## Usage

After compiling the two executables, galled-td.sh is ready to compute the distance between the galled tree in the Extended Newick format [2]. In order to use it run the following command:

./galled-td.sh tree1 tree2 prefix

tree1 and tree2 must each contain a single tree in the Extended Newick format. Prefix is a folder where temporary files will be placed. Please make sure it is accessible and writable.

The distance will be output as a single number to the shell. 


[1] Jesper Jansson, Ramesh Rajaby, Wing-Kin Sung. An Efficient Algorithm for the Rooted Triplet Distance Between Galled Trees. International Conference on Algorithms for Computational Biology 2017
[2] Gabriel Cardona, Francesc Rosselló, Gabriel Valiente. Extended Newick: it is time for a standard representation of phylogenetic networks. BMC Bioinformatics 2008

