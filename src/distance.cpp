/*
 * distance.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: kristin
 */

#include "common.h"
#include "distance.h"
#include <math.h>

float computeEuclidDist(Point* p1, Point* p2) {

	float diff, sum, dist;
	sum = 0;
	for (uint i = 0; i < Point::dim; ++i) {
		diff = p1->values[i] - p2->values[i];
		sum += diff * diff;

	}
	dist = sqrt(sum / Point::dim);

	return dist;
}
