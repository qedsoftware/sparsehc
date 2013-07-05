/*
 * cluster.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: kristin
 */

#include "cluster.h"
#include "common.h"
#include <boost/graph/graphviz.hpp>

void Vertex::updateAncestor(Vertex* ancestor) {
	this->ancestor = ancestor;
	if (this->left)
		this->left->updateAncestor(ancestor);
	if (this->right)
		this->right->updateAncestor(ancestor);
}

//----------------------------------------------------------------------------------------------------

Cluster::Cluster(uint n, char * treeName) :
		numLeaves(n) {
	// add the leaves into the final graph
	phytree.open(treeName, ofstream::binary);
	if (!phytree)
		EXIT_MSG(treeName);
	phytree.precision(5);
}

void Cluster::createLeaves() {

	for (uint i = 0; i < numLeaves; ++i)
		vertices.push_back(this->createVertex(i));
}

Cluster::~Cluster() {

	for (uint i = 0; i < vertices.size(); ++i)
		delete vertices[i];
	phytree.close();

}

void Cluster::updateGraph(uint id1, uint id2, float dist) {

	if (id1 < id2)
		phytree << id1 + 1 << " " << id2 + 1 << " " << dist << endl;
	else
		phytree << id2 + 1 << " " << id1 + 1 << " " << dist << endl;
}

