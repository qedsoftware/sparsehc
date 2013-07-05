/*
 * main.cpp
 *
 *  Created on: Apr 5, 2013
 *      Author: kristin
 */
#include "common.h"
#include "inmatrix.h"
#include "outmatrix.h"
#include "averagecluster.h"
#include "completecluster.h"
#include "singlecluster.h"

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
bool verbose = false;

int main(int argc, char **argv) {

	uint numThreads = 1;
	float threshold = 1.0f;
	int linkage = 0;
	uint numPoints;
	//printUsage();

	Timer timer;
	timer.tic();

	fprintf(stderr,
			"\n============================== SparseHC ==============================\n");
	fprintf(stderr, "\n==== Extract options ====\n");

	if (!extractOptions("--num-threads=%u", &numThreads, argc, argv))
		numThreads = omp_get_num_procs();
	omp_set_num_threads(numThreads);
	fprintf(stderr, "Number of threads: %u\n", numThreads);

	if (!extractOptions("--threshold=%f", &threshold, argc, argv))
		threshold = 1.0f;
	fprintf(stderr, "Distance threshold: %f\n", threshold);

	char temp[100];
	memset(temp, 0, 100);
	if (extractOptions("--verbose=%s", temp, argc, argv))
		verbose = (strcmp(temp, "true") == 0);

	memset(temp, 0, 100);
	if (extractOptions("--linkage=%s", temp, argc, argv)) {
		if (strcmp(temp, "average") == 0)
			linkage = 0;
		else if (strcmp(temp, "complete") == 0)
			linkage = 1;
		else if (strcmp(temp, "single") == 0)
			linkage = 2;
		else
			EXIT_MSG("Invalid linkage option");
	}

	if (!extractOptions("--size=%u", &numPoints, argc, argv))
		numPoints = 10;
	fprintf(stderr, "Number of points: %u\n", numPoints);

	PointVector* points = NULL;
	char buf[BUF_SIZE];

	memset(temp, 0, 100);

	if (extractOptions("--compute=%s", temp, argc, argv)) {

		initProfiler(3, "point", "compute", "cluster");

		// Compute the distance matrix;
		OutMatrix *outMat;

		memset(temp, 0, 100);

		uint dim;
		if (!extractOptions("--dim=%u", &dim, argc, argv))
			dim = 3;
		fprintf(stderr, "Dimension of each point: %u\n", dim);

		fprintf(stderr, "\n==== Generate input vectors ====\n");

		srand(time(NULL));
		PROFILE("point", points = new PointVector(numPoints, dim));

		points->print();
		sprintf(buf, "%s/points_%u.dat", STORE_PATH, numPoints);
		points->printToFile(buf);

		outMat = new OutMatrix(points, threshold);

		fprintf(stderr, "\n==== Compute distance matrix ====\n");

		PROFILE("compute", outMat->compute());
		outMat->stats();

		delete points;
		delete outMat;
	} else {

		initProfiler(1, "cluster");

		fprintf(stderr, "\n==== Read distance matrix from file ====\n");
		InMatrix * inMat;
		sprintf(buf, "%s/sparse_matrix_%u.dat", STORE_PATH, numPoints);
		inMat = new InMatrix(buf, numPoints, threshold);
		inMat->stats();

		Cluster * cluster = NULL;

		if (linkage == 0) {
			fprintf(stderr, "\n==== Average linkage clustering ====\n");
			sprintf(buf, "%s/sparsehc_average_tree_%d.txt", STORE_PATH,
					numPoints);
			cluster = new AverageCluster(numPoints, buf);
		} else if (linkage == 1) {
			fprintf(stderr, "\n==== Complete linkage clustering ====\n");
			sprintf(buf, "%s/sparsehc_complete_tree_%d.txt", STORE_PATH,
					numPoints);
			cluster = new CompleteCluster(numPoints, buf);
		} else if (linkage == 2) {
			fprintf(stderr, "\n=== Single linkage clustering ====\n");
			sprintf(buf, "%s/sparsehc_single_tree_%d.txt", STORE_PATH, numPoints);
			cluster = new SingleCluster(numPoints, buf);
		} else
			EXIT_MSG("Invalid linkage option");

		printf("%u\t", numPoints);
		cluster->createLeaves();
		PROFILE("cluster", cluster->clusterMatrix(inMat));

		delete inMat;
		delete cluster;

	}

	printProfilerStats(); // print profile statistics before exit
	timer.toc();
	fprintf(stderr, "Total time taken: = %.4lf seconds\n", timer.getLapse());
	printf("%.2lf\t", timer.getLapse());
	return EXIT_SUCCESS;
}

