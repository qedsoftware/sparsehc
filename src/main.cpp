/*
 * main.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: kristin
 */
#include "common.h"
#include "inmatrix.h"
#include "averagecluster.h"
#include "completecluster.h"
#include "singlecluster.h"

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
bool verbose = false;

int main(int argc, char **argv) {

	float threshold = 1.0f;
	int linkage = 0;
	uint numPoints = 10;
	char matFileName[BUF_SIZE];
	char treeFileName[BUF_SIZE];

	Timer timer;
	timer.tic();

	printf("\n============================== SparseHC ==============================\n");

	printUsage();

	fprintf(stderr, "\n==== Extract options ====\n");

	if (!extractOptions("--matrix=%s", matFileName, argc, argv))
		EXIT_MSG("The path to input matrix file is required!");
	fprintf(stderr, "Input matrix: %s\n", matFileName);

	if (!extractOptions("--tree=%s", treeFileName, argc, argv))
		EXIT_MSG("The path to output tree file is required!");
	fprintf(stderr, "Output tree: %s\n", treeFileName);

	if (!extractOptions("--size=%u", &numPoints, argc, argv))
		EXIT_MSG("The number of data points is required!");
	fprintf(stderr, "Number of data points: %u\n", numPoints);

	if (!extractOptions("--threshold=%f", &threshold, argc, argv))
		EXIT_MSG("The distance threshold of the input matrix is required!");
	fprintf(stderr, "Distance threshold: %f\n", threshold);

	char buf[BUF_SIZE];
	if (extractOptions("--linkage=%s", buf, argc, argv)) {
		if (strcmp(buf, "average") == 0) {
			linkage = 0;
			fprintf(stderr, "Linkage: average\n");
		} else if (strcmp(buf, "complete") == 0) {
			linkage = 1;
			fprintf(stderr, "Linkage: complete\n");
		} else if (strcmp(buf, "single") == 0) {
			linkage = 2;
			fprintf(stderr, "Linkage: single\n");
		} else
			EXIT_MSG(
					"Invalid linkage option! SparseHC only supports average-linkage, complete-linkage and single-linkage.");
	}
	initProfiler(1, "cluster");

	fprintf(stderr, "\n==== Read distance matrix from file ====\n");
	InMatrix * inMat;
	inMat = new InMatrix(matFileName, numPoints, threshold);
	inMat->stats();

	fprintf(stderr, "\n==== Perform clustering ====\n");
	Cluster * cluster = NULL;

	if (linkage == 0)
		cluster = new AverageCluster(numPoints, treeFileName);
	else if (linkage == 1)
		cluster = new CompleteCluster(numPoints, treeFileName);
	else if (linkage == 2)
		cluster = new SingleCluster(numPoints, treeFileName);

	cluster->createLeaves();
	PROFILE("cluster", cluster->clusterMatrix(inMat));

	delete inMat;
	delete cluster;

	printProfilerStats(); // print profile statistics before exit
	timer.toc();
	printf("Total time taken: = %.4lf seconds\n", timer.getLapse());
	return EXIT_SUCCESS;
}

