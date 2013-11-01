/*
 * maxcluster.cpp
 *
 *  Created on: Apr 12, 2013
 *      Author: kristin
 */

#include "completecluster.h"
#include "common.h"
#include <limits.h>

bool CompleteVertex::isConnected(Vertex* v) {
	return outEdges.find(v) != outEdges.end();
}

void CompleteVertex::printOutEdges() {
	if (outEdges.empty())
		return;

	CompleteEdgeMapIt it;
	fprintf(stderr, "Vertex %u has %zu edges:", id, outEdges.size());
	for (it = outEdges.begin(); it != outEdges.end(); ++it)
		fprintf(stderr, "(%u, %u) ", it->first->id, it->second);
	fprintf(stderr, "\n");
}

//----------------------------------------------------------------------------------------------------

uint CompleteCluster::getNumEdges() {
	CompleteVertex *n;
	uint sum = 0;
	for (uint i = 0; i < vertices.size(); ++i) {
		n = reinterpret_cast<CompleteVertex *>(vertices[i]);
		if (n->isActive)
			sum += n->outEdges.size();
	}

	return sum;
}

Vertex* CompleteCluster::createVertex(uint id) {
	return new CompleteVertex(id);
}

// merge two clusters, v1->id > v2->id
void CompleteCluster::merge(CompleteVertex * v1, CompleteVertex * v2, CompleteVertex *v) {

	// set child vertices for the parent TreeNode
	v->left = v1;
	v->right = v2;

	// set parent TreeNode for the child vertices
	v1->updateAncestor(v);
	v2->updateAncestor(v);

	// set dist between two child vertices
	v->numChildren = v1->numChildren + v2->numChildren;

	// clear the connection between v1 and v2
	v1->outEdges.erase(v2);

	CompleteEdgeMapIt it;
	// pass the outEdges from v1 to the parentNode
	v->outEdges = v1->outEdges;

	// for each item in the outEdges of v2
	for (it = v2->outEdges.begin(); it != v2->outEdges.end(); ++it)
		v->outEdges[it->first] += it->second;

	v1->outEdges.clear();
	v2->outEdges.clear();

	v1->isActive = false;
	v2->isActive = false;

	CompleteVertex *tempV;

	// for vertices with larger ID than v2 that hold the outEdges to v2
	for (uint i = v2->id + 1; i < v1->id; ++i) {
		tempV = reinterpret_cast<CompleteVertex *>(vertices[i]);

		if (tempV->isActive && tempV->isConnected(v2)) {
			v->outEdges[tempV] += tempV->outEdges.at(v2);
			tempV->outEdges.erase(v2);
		}
	}

	// for vertices with larger ID than v1 that hold the links to v1 or v2
	for (uint i = v1->id + 1; i < v->id; ++i) {
		tempV = reinterpret_cast<CompleteVertex *>(vertices[i]);

		if (tempV->isActive && tempV->isConnected(v1)) {
			v->outEdges[tempV] += tempV->outEdges.at(v1);
			tempV->outEdges.erase(v1);
		}

		if (tempV->isActive && tempV->isConnected(v2)) {
			v->outEdges[tempV] += tempV->outEdges.at(v2);
			tempV->outEdges.erase(v2);
		}
	}

	totalEdges = getNumEdges();
}

void CompleteCluster::printAllEdges() {
	CompleteVertex *n;
	for (uint i = 0; i < vertices.size(); ++i) {
		n = reinterpret_cast<CompleteVertex *>(vertices[i]);
		if (n->isActive)
			n->printOutEdges();
	}
}

void CompleteCluster::clusterMatrix(InMatrix * mat) {

	uint maxEdges = mat->numPoints; // maxNumEdges = num of links can be stored in RAM
	uint newId = numLeaves;

	CompleteVertex *v1, *v2, *tempV, *v;
	uint row, col;
	float value;

	while (mat->getNext(row, col, value) && value < mat->threshold) {

		v1 = reinterpret_cast<CompleteVertex*>(vertices[row]->ancestor);
		v2 = reinterpret_cast<CompleteVertex*>(vertices[col]->ancestor);

		if (v1 == v2)
			return;

		if (v1->id < v2->id) {
			tempV = v1;
			v1 = v2;
			v2 = tempV;
		}

		// v1 is always larger than v2
		// Links are stored in v1
		uint weight = ++v1->outEdges[v2];

		totalEdges += (weight == 1);

		if (weight == v1->numChildren * v2->numChildren) {

			v = reinterpret_cast<CompleteVertex *>(createVertex(newId));
			vertices.push_back(v);
			merge(v1, v2, v);
			++newId;

			// for visualization purpose
			updateGraph(v1->id, v2->id, value);
		}

		if (totalEdges > maxEdges)
			maxEdges *= 1.1f;
	}

	fprintf(stderr, "Finished! number of edges = %u. linear ratio: %.2f. quadratic ratio: %zu\n", maxEdges,
			(double) maxEdges / mat->numPoints, mat->numElements / maxEdges);
}

