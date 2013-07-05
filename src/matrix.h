/*
 * matrix.h
 *
 *  Created on: Apr 9, 2013
 *      Author: kristin
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

#define EPSILON 1.0e-6

struct Element {
	uint row;
	uint col;
	float value;

	Element() {
		row = 0;
		col = 0;
		value = 0;
	}

	Element(uint aRow, uint aCol, float aValue) {
		row = aRow;
		col = aCol;
		value = aValue;
	}

	void update(uint aRow, uint aCol, float aValue) {
		row = aRow;
		col = aCol;
		value = aValue;
	}

	bool operator <(const Element &e) const {
		if (fabsf(value - e.value) < EPSILON) {
			if (row == e.row)
				return col < e.col;
			else
				return row < e.row;
		} else
			return value < e.value;
	}

	static size_t size() {
		return sizeof(uint) * 2 + sizeof(float);
	}

	void print() {
		fprintf(stderr, "%u\t%u\t%f\n", row, col, value);
	}
};

//----------------------------------------------------------------------------------------------------

struct Matrix {


	vector<Element*> elements;

	size_t numElements;
	float threshold;
	uint numPoints;

	Matrix(uint numPoints, float threshold);

	Matrix();

	virtual ~Matrix() {
		for (uint i = 0; i < elements.size(); ++i)
			delete elements[i];
	}

	void clear() {
		for (uint i = 0; i < elements.size(); ++i)
			delete elements[i];
		elements.clear();
	}

	Element* push(uint row, uint col, float dist) {
		Element* e = new Element(row, col, dist);
		elements.push_back(e);
		return e;
	}

	double sparsity() {
		return (double) numElements / (numPoints * (numPoints - 1) / 2);
	}

	size_t sizeInBytes() {
		return elements.size() * Element::size();
	}

	void stats();
};

#endif /* MATRIX_H_ */
