/*
 * outmatrix.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: kristin
 */

#include <algorithm>
#include <map>
#include "outmatrix.h"
#include "distance.h"

bool compareElementPointers(Element* e1, Element* e2) {
	return (*e1 < *e2);
}

OutMatrix::OutMatrix(PointVector* v, float threshold) :
		Matrix(v->size(), threshold) {

	numFiles = 0;
	vectors = v;
	sprintf(fullMatrixName, "%s/full_matrix_%u.dat", STORE_PATH, numPoints);
	sprintf(sparseMatrixName, "%s/sparse_matrix_%u.dat", STORE_PATH,
			numPoints);

	ofs.open(fullMatrixName, ofstream::binary);
	if (!ofs)
		EXIT_MSG(fullMatrixName);

}

void OutMatrix::compute() {

	float dist;
	uint size = 0;

	for (uint i = 0; i < numPoints; ++i) {
		for (uint j = i + 1; j < numPoints; ++j) {
			dist = computeEuclidDist(vectors->get(i), vectors->get(j));

			ofs << dist << " ";

			if (dist < threshold || fabsf(dist - threshold) < EPSILON) {
				this->push(i, j, dist);
				++size;

				if (size == 1048576) {
					writeToFile();
					size = 0;
				}
			}
		}
	}

	ofs.close();

	if (elements.size() > 0)
		writeToFile();

	mergeToFile(sparseMatrixName);
}

void OutMatrix::sortElements() {
// sort elements by distance
	sort(elements.begin(), elements.end(), compareElementPointers);
}

void OutMatrix::mergeToFile(const char* filename) {

	ofstream outFile;
	outFile.open(filename, ofstream::binary);
	if (!outFile)
		EXIT_MSG(filename);

	uint row, col, fileId;
	float value;

	map<Element, uint> sortedElements;
	map<Element, uint>::iterator it;

	vector<FILE *> files;
	FILE * file;
	char buf[BUF_SIZE];

	for (uint i = 0; i < numFiles; ++i) {
		sprintf(buf, "%s/%u", TEMP_PATH, i);
		file = fopen(buf, "rb");

		if (!file)
			EXIT_MSG(buf);

		files.push_back(file);

		if (fscanf(file, "%u %u %f\n", &row, &col, &value) != EOF)
			sortedElements.insert(
					pair<Element, uint>(Element(row, col, value), i));
		else
			fclose(file);
	}

	while (!sortedElements.empty()) {
		// get the first element in the queue and write to output file
		it = sortedElements.begin();
		outFile << it->first.row << " " << it->first.col << " "
				<< it->first.value << endl;

		fileId = it->second;

		// delete the element from the queue
		sortedElements.erase(it);

		// insert to the queue the next item from the same file
		if (fscanf(files[fileId], "%u %u %f\n", &row, &col, &value) != EOF)
			sortedElements.insert(
					pair<Element, uint>(Element(row, col, value), fileId));
		else {
			fclose(files[fileId]);
			sprintf(buf, "%s/%u", TEMP_PATH, fileId);
			fprintf(stderr, "Delete file: %s\n", buf);
			remove(buf); // delete the temp file when done
		}
	}

	outFile.close();
}

// print sparse matrix
void OutMatrix::writeToFile() {

	sortElements();
	char buf[BUF_SIZE];
	sprintf(buf, "%s/%u", TEMP_PATH, numFiles);

	ofstream file;
	file.open(buf, ofstream::binary);
	if (!file)
		EXIT_MSG(buf);

	for (uint i = 0; i < elements.size(); ++i)
		file << elements[i]->row << " " << elements[i]->col << " "
				<< elements[i]->value << endl;

	file.close();

	++numFiles;

	numElements += elements.size();
	clear();
}
