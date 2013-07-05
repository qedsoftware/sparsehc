/*
 * vector.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: kristin
 */

#include "point.h"
#include "distance.h"
#include "common.h"

uint Point::dim = 0;

Point::Point(uint d) {

	dim = d;
	values = (float*) malloc(dim * sizeof(float));
	for (uint i = 0; i < dim; ++i)
		values[i] = (float) rand() / (float) RAND_MAX;
}

//----------------------------------------------------------------------------------------------------

void PointVector::print() {
	if (DEBUG == 2) {
		fprintf(stderr, "Dimenstion = %u\n", Point::dim);
		for (uint i = 0; i < points.size(); ++i) {
			fprintf(stderr, "Read: %u. Values: ", i);
			for (uint j = 0; j < Point::dim; ++j)
				fprintf(stderr, "%.3f ", points[i]->values[j]);
			fprintf(stderr, "\n");
		}
	}
}

void PointVector::printToFile(const char* name) {

	FILE * file = fopen(name, "wb");
	if (!file) {
		WARN(name);
		return;
	}

	for (uint i = 0; i < points.size(); ++i) {
		for (uint j = 0; j < Point::dim; ++j)
			fprintf(file, "%f ", points[i]->values[j]);
		fprintf(file, "\n");
	}
	fclose(file);

}

