/*
 * inmatrix.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: kristin
 */

#include "inmatrix.h"

InMatrix::InMatrix(const char* filename, uint numPoints, float threshold) :
		Matrix(numPoints, threshold) {
	file = fopen(filename, "rb");
	if (!file)
		EXIT_MSG("Cannot open input file!");

	// get length of file:
	pos = 0;
	fullyLoaded = false;
}

bool InMatrix::getNext(uint &row, uint &col, float &value) {
	if (fscanf(file, "%u %u %f", &row, &col, &value) == 3)
		return true;
	else
		return false;
}

Element* InMatrix::getNext() {
	if (pos < elements.size())
		return elements[pos++];
	else {
		if (loadElements())
			return elements[pos++];
		else
			return NULL;
	}
}

bool InMatrix::loadElements() {

	this->clear();
	pos = 0;

	uint row = 0, col = 0, size = 0;
	float value = 0.0f;

	while (!feof(file) && !fullyLoaded && size < 4096) {
		if (fscanf(file, "%u %u %f", &row, &col, &value) == 3 && value < threshold) {
			this->push(row, col, value);
			++size;
		} else {
			fprintf(stderr, "Stop at: %u %u %f\n", row, col, value);
			fullyLoaded = true;
		}
	}

	numElements += size;
	if (size > 0)
		return true;
	else
		return false;
}
