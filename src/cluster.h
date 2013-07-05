/*
 * cluster.h
 *
 *  Created on: Apr 9, 2013
 *      Author: kristin
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include <unordered_set>
#include <list>
#include <fstream>
#include "inmatrix.h"
using namespace std;

struct Vertex {
	uint id;
	Vertex* ancestor;
	Vertex* left;
	Vertex* right;
	bool isActive;

	Vertex(uint newId) {
		id = newId;
		ancestor = this;
		left = right = 0;
		isActive = true;
	}

	void updateAncestor(Vertex* ancestor);
};

//----------------------------------------------------------------------------------------------------
struct Cluster {

protected:
	vector<Vertex *> vertices;
	uint numLeaves;

private:
	ofstream phytree;
public:
	Cluster(uint n, char * treeName);

	virtual ~Cluster();

	Vertex * getVertex(uint id) {
		return vertices[id];
	}

	// overriden methods
	virtual Vertex* createVertex(uint id) = 0;

	virtual void clusterMatrix(InMatrix* mat) = 0;

	// non-overriden methods
	void createLeaves();

	void updateGraph(uint id1, uint id2, float dist);

	void printGraph();
};

#endif /* CLUSTER_H_ */
