/*
 * averagecluster.cpp
 *
 *  Created on: Apr 12, 2013
 *      Author: kristin
 */

#include "averagecluster.h"
#include "common.h"

bool AverageVertex::isConnected(Vertex* v) {
	return outEdges.find(v) != outEdges.end();
}

// idX > idY
float AverageVertex::computeDist(AverageVertex* v, float max, float tempMax) {

	float dist = 1.0f, weight;
	uint maxWidth;
	uint missing, width;

	if (isConnected(v)) {
		maxWidth = numChildren * v->numChildren;
		weight = outEdges.at(v).weight;
		width = outEdges.at(v).width;
		missing = maxWidth - width;

		if (missing == 0) {
			dist = (weight + max * missing) / maxWidth;
			if (dist < exactDist) {
				exactDist = dist;
				candidate = v;
			}
		} else {
			dist = (weight + tempMax * missing) / maxWidth;
			if (dist < inexactDist)
				inexactDist = dist;
		}
	}

	return dist;
}

void AverageVertex::updateMin(float max, float tempMax) {

	// only update an active vertex
	if (!isActive)
		return;

	AverageVertex *n;
	AverageEdgeMapIt it;

	exactDist = 1.0f;
	inexactDist = 1.0f;

	if (!outEdges.empty()) {
		for (it = outEdges.begin(); it != outEdges.end(); ++it) {
			n = reinterpret_cast<AverageVertex *>(it->first);

			if (id > n->id)
				computeDist(n, max, tempMax);
			else
				n->computeDist(this, max, tempMax);
		}
	}
}

void AverageVertex::printEdges() {

	if (outEdges.empty())
		return;

	AverageEdgeMapIt it;
	fprintf(stderr, "Vertex %u has %zu outcoming edges:", id + 1, outEdges.size());
	for (it = outEdges.begin(); it != outEdges.end(); ++it)
		fprintf(stderr, "(%u, %f, %u) ", it->first->id + 1, it->second.weight, it->second.width);
	fprintf(stderr, "\n");
}

void AverageVertex::printCandidate() {
	if (candidate)
		fprintf(stderr, "Candidate: %u %f %f\n", candidate->id + 1, exactDist, inexactDist);
}

//----------------------------------------------------------------------------------------------------

uint AverageCluster::getNumEdges() {
	AverageVertex *v;
	uint sum = 0;
	for (uint i = 0; i < vertices.size(); ++i) {
		v = reinterpret_cast<AverageVertex *>(vertices[i]);
		if (v->isActive)
			sum += v->outEdges.size();
	}
	return sum;
}

uint AverageCluster::getNumActiveVertices() {

	uint count = 0;
	for (uint i = 0; i < vertices.size(); ++i) {
		if (vertices[i]->isActive)
			++count;
	}
	return count;
}

void AverageCluster::printAllEdges() {
	AverageVertex *v;
	for (uint i = 0; i < vertices.size(); ++i) {
		v = reinterpret_cast<AverageVertex *>(vertices[i]);
		if (v->isActive) {
			v->printEdges();
			v->printCandidate();
		}
	}
}

Vertex* AverageCluster::createVertex(uint id) {
	return new AverageVertex(id);
}

void AverageCluster::updateAllMin(float max) {

	for (uint i = 0; i < vertices.size(); ++i)
		reinterpret_cast<AverageVertex *>(vertices[i])->updateMin(max, tempMax);

}

// merge two clusters
void AverageCluster::merge(AverageVertex * v1, AverageVertex * v2, AverageVertex *v, float max) {
	// set child vertices of the new vertex
	v->left = v1;
	v->right = v2;

	// update the ancestors
	v1->updateAncestor(v);
	v2->updateAncestor(v);

	// set numChildren of the new vertex
	v->numChildren = v1->numChildren + v2->numChildren;

	// clear the connection between v1 and v2
	v1->outEdges.erase(v2);

	AverageEdgeMapIt it;
	// pass the outEdges of v1 to the parentNode
	v->outEdges = v1->outEdges;

	// for each item in the outEdges of v2
	for (it = v2->outEdges.begin(); it != v2->outEdges.end(); ++it)
		v->outEdges[it->first].add(it->second);

	// cleaning the outEdges of v1 and v2
	v1->outEdges.clear();
	v2->outEdges.clear();

	v1->isActive = false;
	v2->isActive = false;

	AverageVertex *tempV;

	// for vertices with larger ID than v2 that hold the links to v2
	for (uint i = v2->id + 1; i < v1->id; ++i) {
		tempV = reinterpret_cast<AverageVertex *>(vertices[i]);

		if (tempV->isActive && tempV->isConnected(v2)) {
			v->outEdges[tempV].add(tempV->outEdges.at(v2));
			tempV->outEdges.erase(v2);
		}
	}

	// for vertices with larger ID than v1 that hold the links to v1 or v2
	for (uint i = v1->id + 1; i < v->id; ++i) {
		tempV = reinterpret_cast<AverageVertex *>(vertices[i]);

		if (tempV->isActive && tempV->isConnected(v1)) {
			v->outEdges[tempV].add(tempV->outEdges.at(v1));
			tempV->outEdges.erase(v1);
		}

		if (tempV->isActive && tempV->isConnected(v2)) {
			v->outEdges[tempV].add(tempV->outEdges.at(v2));
			tempV->outEdges.erase(v2);
		}
	}

	// Update the distance from the newNode to its outEdges
	for (it = v->outEdges.begin(); it != v->outEdges.end(); ++it) {

		tempV = reinterpret_cast<AverageVertex *>(it->first);

		v->computeDist(tempV, max, tempMax);

		if (tempV->candidate == v1 || tempV->candidate == v2)
			tempV->updateMin(max, tempMax);
	}

	totalEdges = getNumEdges();
}

// absorb a TreeNode
void AverageCluster::absorb(Element* e) {
	AverageVertex *v1, *v2;

	v1 = reinterpret_cast<AverageVertex*>(vertices[e->row]->ancestor);
	v2 = reinterpret_cast<AverageVertex*>(vertices[e->col]->ancestor);

	if (v1 == v2)
		return;

	uint weight = 0;
	// Links are stored in the node with the LARGER id
	if (v1->id > v2->id)
		weight = v1->outEdges[v2].update(e->value);
	else
		weight = v2->outEdges[v1].update(e->value);

	totalEdges += (weight == 1);
}

// absorb a TreeNode
void AverageCluster::absorb(uint row, uint col, float value) {
	AverageVertex *v1, *v2;

	v1 = reinterpret_cast<AverageVertex*>(vertices[row]->ancestor);
	v2 = reinterpret_cast<AverageVertex*>(vertices[col]->ancestor);

	if (v1 == v2)
		return;

	uint weight = 0;
	// Links are stored in the node with the LARGER id
	if (v1->id > v2->id)
		weight = v1->outEdges[v2].update(value);
	else
		weight = v2->outEdges[v1].update(value);

	totalEdges += (weight == 1);
}

bool AverageCluster::getCandidate(float &minInexact, float &minExact, AverageVertex* &vertex) {
	minInexact = minExact = 1.0f;
	AverageVertex *v;
	for (uint i = 0; i < vertices.size(); ++i) {
		v = reinterpret_cast<AverageVertex *>(vertices[i]);
		if (v->isActive) {
			if (v->inexactDist < minInexact)
				minInexact = v->inexactDist;
			if (v->exactDist < minExact) {
				vertex = v;
				minExact = vertex->exactDist;
			}
		}
	}

	return (minExact < 1.0f) && (minExact < minInexact || fabsf(minExact - minInexact) < EPSILON);
}

void AverageCluster::clusterMatrix(InMatrix * mat) {

	uint maxEdges = mat->numPoints; // maxEdges = num of outEdges can be stored in RAM

	uint newId = numLeaves;

	uint row, col;
	float value;

	bool done = false;

	while (!done) {

		// if after merging the number of edges is still more than allowed
		if (totalEdges > maxEdges)
			maxEdges *= 1.1f;

		// read a chunk of the matrix
		while (totalEdges <= maxEdges && mat->getNext(row, col, value) && value < mat->threshold)
			absorb(row, col, value);

		tempMax = value;
		if (totalEdges <= maxEdges)
			done = true;

		// update tempMax and min values
		updateAllMin(mat->threshold);

#if (DEBUG==1)
		fprintf(stderr, "tempMax = %f\n", tempMax);
#endif

		// cluster the current chunk
		AverageVertex * v1 = NULL, *v2 = NULL, *v;
		float minInexact, minExact;

		while (getCandidate(minInexact, minExact, v1)) {
			v = reinterpret_cast<AverageVertex *>(createVertex(newId));
			vertices.push_back(v);
			++newId;

			v2 = v1->candidate;
			merge(v1, v2, v, mat->threshold);

			// for visualization purpose
			updateGraph(v1->id, v2->id, minExact);
		}

#if (DEBUG==1)
		if (minExact < 1.0f)
		fprintf(stderr, "Cannot continue: %f\t%f\t%u\n", minExact, minInexact, newId);
		else
		fprintf(stderr, "Cannot find any more complete edges!\n");
#endif
	}

	fprintf(stderr, "Finished! number of edges = %u. linear ratio: %.2f. quadratic ratio: %zu\n", maxEdges,
			(double) maxEdges / mat->numPoints, mat->numElements / maxEdges);
}

