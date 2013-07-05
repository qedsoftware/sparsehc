/*
 * point.h
 *
 *  Created on: Apr 8, 2013
 *      Author: kristin
 */

#ifndef POINT_H_
#define POINT_H_

#include <vector>
#include <set>
#include <cstdlib>
using namespace std;

struct Point {

	static uint dim;
	float * values;

	Point(uint d);

	~Point() {
		if (values)
			free(values);
		values = NULL;
	}

	static size_t sizeInBytes() {
		return dim * sizeof(float);
	}
};

//----------------------------------------------------------------------------------------------------

class PointVector
{
private:
	vector<Point*> points;
	uint dim_;
public:

	PointVector(uint size, uint dim) :
			dim_(dim) {
		for (uint i = 0; i < size; ++i)
			this->push(dim);
	}

	~PointVector() {
		uint sz = points.size();
		for (uint i = 0; i < sz; ++i)
			delete points[i];
	}

	Point* push(uint d) {
		Point* v = new Point(d);
		points.push_back(v);
		return v;
	}

	Point* get(int i) {
		return points[i];
	}

	uint size() {
		return points.size();
	}

	size_t sizeInBytes() {
		return points.size() * Point::sizeInBytes();
	}

	void print();

	void printToFile(const char* name);
};

#endif /* VECTOR_H_ */
