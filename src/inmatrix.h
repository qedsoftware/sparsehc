/*
 * inmatrix.h
 *
 *  Created on: Apr 25, 2013
 *      Author: kristin
 */

#ifndef INMATRIX_H_
#define INMATRIX_H_

#include "matrix.h"
#include "common.h"
using namespace std;

struct InMatrix: public Matrix {

	uint pos;
	FILE * file;
	bool fullyLoaded;

	InMatrix(const char* filename, uint numPoints, float threshold);

	~InMatrix() {
		pos = 0;
		fclose(file);
	}

	bool loadElements();

	bool isEmpty() {
		return (pos >= elements.size() && (feof(file) || fullyLoaded));
	}

	Element* getNext();

	bool getNext(uint &row, uint &col, float &value);

}
;

#endif /* INMATRIX_H_ */
