/*
 * averagecluster.h
 *
 *  Created on: Apr 12, 2013
 *      Author: kristin
 */

#ifndef AVERAGECLUSTER_H_
#define AVERAGECLUSTER_H_

#include <unordered_map>
#include "cluster.h"
using namespace std;

struct AverageEdge {
	uint width;
	float weight;

	uint update(float dist) {
		weight += dist;
		++width;
		return width;
	}

	uint add(const AverageEdge& l) {
		weight += l.weight;
		width += l.width;
		return width;
	}
};

typedef unordered_map<Vertex*, AverageEdge> AverageEdgeMap;
typedef AverageEdgeMap::iterator AverageEdgeMapIt;

//----------------------------------------------------------------------------------------------------

struct AverageVertex: public Vertex {

	AverageEdgeMap outEdges;
	float inexactDist; // min inexact dist
	float exactDist; // max exact dist
	AverageVertex* candidate;
	uint numChildren;

	AverageVertex(uint newId) :
			Vertex(newId) {
		inexactDist = 1.0f;
		exactDist = 1.0f;
		candidate = 0;
		numChildren = 1;
	}

	bool isConnected(Vertex* v);

	float computeDist(AverageVertex* v, float max, float tempMax);

	void updateMin(float max, float tempMax);

	void printEdges();

	void printCandidate();
};

//----------------------------------------------------------------------------------------------------

struct AverageCluster: public Cluster {

	float tempMax;
	uint totalEdges;

	AverageCluster(uint n, char * treeName) :
			Cluster(n, treeName) {
		totalEdges = 0;
		tempMax = 0;
	}

	// overrider super methods
	Vertex* createVertex(uint id);

	void clusterMatrix(InMatrix * mat);

	// current class methods
	void merge(AverageVertex * v1, AverageVertex * v2, AverageVertex* v, float max);

	void absorb(Element* e);
	void absorb(uint row, uint col, float value);

	bool getCandidate(float &minInexact, float &minExact, AverageVertex* &n);

	void updateAllMin(float max);

	uint getNumEdges();

	uint getNumActiveVertices();

	void printAllEdges();
};

#endif /* AVERAGECLUSTER_H_ */
