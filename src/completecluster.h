/*
 * maxcluster.h
 *
 *  Created on: Apr 12, 2013
 *      Author: kristin
 */

#ifndef MAXCLUSTER_H_
#define MAXCLUSTER_H_

#include <unordered_map>
#include "cluster.h"
using namespace std;

typedef unordered_map<Vertex *, uint> CompleteEdgeMap;
typedef CompleteEdgeMap::iterator CompleteEdgeMapIt;

//----------------------------------------------------------------------------------------------------

struct CompleteVertex: public Vertex {

	CompleteEdgeMap outEdges;
	uint numChildren;

	CompleteVertex(uint newId) :
			Vertex(newId) {
		numChildren = 1;
	}

	bool isConnected(Vertex* v);

	uint getNumEdges() {
		return outEdges.size();
	}

	void printOutEdges();
};

//----------------------------------------------------------------------------------------------------

struct CompleteCluster: public Cluster {

	uint totalEdges;

	CompleteCluster(uint matDim, char * treeName) :
			Cluster(matDim, treeName) {
		totalEdges = 0;
	}

	// overrider super methods
	Vertex* createVertex(uint id);

	void clusterMatrix(InMatrix * mat);

	// current class methods
	void merge(CompleteVertex * v1, CompleteVertex * v2, CompleteVertex *v);

	void writeAllEdges(FILE* file);

	uint getNumEdges();

	void printAllEdges();

};

#endif /* MAXCLUSTER_H_ */
