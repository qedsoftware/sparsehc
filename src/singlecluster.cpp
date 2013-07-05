/*
 * mincluster.cpp
 *
 *  Created on: Apr 12, 2013
 *      Author: kristin
 */

#include "singlecluster.h"
#include "common.h"

Vertex* SingleCluster::createVertex(uint id) {
	return new Vertex(id);
}

//----------------------------------------------------------------------------------------------------

void SingleCluster::clusterMatrix(InMatrix * mat) {

	uint newId = numLeaves, maxVertices = numLeaves * 2 - 1;
	uint row, col;
	float value;
	Vertex *v1, *v2, *v;

	while (mat->getNext(row, col, value) && value < mat->threshold && newId < maxVertices) {

		// get the ancestor of two leaves extracted from the input matrix
		v1 = vertices[row]->ancestor;
		v2 = vertices[col]->ancestor;

		if (v1 == v2)
			continue;

		v = createVertex(newId);
		vertices.push_back(v);
		++newId;

		// update the left and right attribute
		v->left = v1;
		v->right = v2;

		// update the ancestor atrribute
		v1->updateAncestor(v);
		v2->updateAncestor(v);

		// for visualization purpose
		updateGraph(v1->id, v2->id, value);
	}

	fprintf(stderr, "Finished! index = %u\n", mat->pos);
}
