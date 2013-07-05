/*
 * common.h
 *
 *  Created on: Apr 7, 2013
 *      Author: kristin
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <omp.h>
#include <memory>
using namespace std;

#define EXIT_ERR(msg, error) { \
	fprintf(stderr, "Error %d in %s (Line %d): %s\n", error, __func__, __LINE__, (msg)); \
	exit(error); \
}

#define EXIT_MSG(msg) { \
	fprintf(stderr, "Error in %s (Line %d): %s\n", __func__, __LINE__, (msg)); \
	exit(EXIT_FAILURE); \
}

#define cudaCheck(status)   if ((status) != cudaSuccess)           { fprintf(stderr, "CUDA error: %s\n ", cudaGetErrorString(status)); EXITERR("CUDA error: ", (status)); }

#define WARN(msg) { \
	fprintf(stderr, "Warning in %s (Line %d): %s\n", __func__, __LINE__, (msg)); \
}

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define DEBUG 0  // if 2: include reads, tuples and vectors
#define TEMP_PATH "/media/kristin/DATA/data"
#define STORE_PATH "/media/kristin/Movies/data" // slow


#define BUF_SIZE 4096
//----------------------------------------------------------------------------------------------------

#include <sys/time.h>
class Timer {
private:
	struct timeval start_, end_;
	double lapse_;

public:
	void tic(); // set start
	void toc(); // set end and compute lapse
	double getLapse() {
		return lapse_;
	}
};

struct Profiler {
	int n;
	int count[64];
	double total_time[64];
	char tags[64][256];
};

void initProfiler(int num_tags, ...);
void addToProfiler(const char* tag, double lapse);
void printProfilerStats();

#define PROFILE(tag, code_block) { \
  Timer timer_; \
  timer_.tic(); \
  code_block; \
  timer_.toc(); \
  addToProfiler(tag, timer_.getLapse()); \
}

//----------------------------------------------------------------------------------------------------

void printUsage();

int extractOptions(const char *format, void *value, int argc, char **argv);

string sizeOnDisk(size_t sizeInBytes);

#endif /* COMMON_H_ */
