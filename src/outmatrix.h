/*
 * outmatrix.h
 *
 *  Created on: Apr 25, 2013
 *      Author: kristin
 */

#ifndef OUTMATRIX_H_
#define OUTMATRIX_H_

#include <fstream>
#include <cstdlib>
#include "matrix.h"
#include "point.h"
#include "common.h"
using namespace std;

class OutMatrix: public Matrix {

private:
	PointVector* vectors;

	uint numFiles;
	char fullMatrixName[BUF_SIZE];
	char sparseMatrixName[BUF_SIZE];


	ofstream ofs;

	void sortElements();

	void writeToFile();

	void mergeToFile(const char* filename);

public:
	void compute();

	void cuCompute();

	OutMatrix() :
			Matrix() {
		numFiles = 0;
	}

	OutMatrix(PointVector* v, float threshold);

	~OutMatrix() {
		fprintf(stderr, "Number of files: %u\n", numFiles);
	}
};

#endif /* OUTMATRIX_H_ */
