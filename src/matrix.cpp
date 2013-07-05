/*
 * matrix.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: kristin
 */

#include <algorithm>
#include "matrix.h"

bool isAboveThreshold(Element* e, float threshold) {
	return (e->value > threshold);
}

//----------------------------------------------------------------------------------------------------

Matrix::Matrix() {
	threshold = 0.0f;
	numPoints = 0;
	numElements = 0;
}

Matrix::Matrix(uint n, float t) {
	threshold = t;
	numPoints = n;
	numElements = 0;
}

void Matrix::stats() {

	fprintf(stderr, "Number of elements: %zu\n", numElements);
	fprintf(stderr, "Threshold: %f\n", threshold);
	fprintf(stderr, "Number of points: %u\n", numPoints);
	fprintf(stderr, "Sparsity: %.3f\n", sparsity());
}

