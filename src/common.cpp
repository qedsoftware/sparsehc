/*
 * common.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: kristin
 */
#include <stdarg.h>
#include "common.h"

Profiler gProfiler;

void Timer::tic() {
	gettimeofday(&start_, NULL);
}

void Timer::toc() {
	gettimeofday(&end_, NULL);
	lapse_ = end_.tv_sec - start_.tv_sec + 1e-6 * (end_.tv_usec - start_.tv_usec);
}

//----------------------------------------------------------------------------------------------------

void initProfiler(int num_tags, ...) {
	gProfiler.n = num_tags;
	int i;
	va_list tags;
	va_start(tags, num_tags);
	for (i = 0; i < num_tags; i++) {
		const char *tag = va_arg(tags, char*);
		strcpy(gProfiler.tags[i], tag);
		gProfiler.count[i] = 0;
		gProfiler.total_time[i] = 0;
	}
	va_end(tags);
}

void addToProfiler(const char* tag, double lapse) {
	int i;
	for (i = 0; i < gProfiler.n; i++) {
		if (strcmp(gProfiler.tags[i], tag) == 0) {
			gProfiler.count[i]++;
			gProfiler.total_time[i] += lapse;
			return;
		}
	}
}

void printProfilerStats() {
	int i;
	fprintf(stderr, "\n\n==== Profiler statistics ====\n");
	for (i = 0; i < gProfiler.n; i++) {
		fprintf(stderr, "tag %s:\n", gProfiler.tags[i]);
		fprintf(stderr, "\tcount = %d\n", gProfiler.count[i]);
		fprintf(stderr, "\ttotal = %.4lf seconds\n", gProfiler.total_time[i]);
		if (gProfiler.count[i] > 0)
			fprintf(stderr, "\taverage = %.4lf seconds\n",
					gProfiler.total_time[i] / gProfiler.count[i]);
	}
	fprintf(stderr, "\n");
}

//----------------------------------------------------------------------------------------------------

void printUsage() {
	cerr << "Commands: " << endl;
	cerr << "\tSparseHC" << endl;
	cerr << "Options: " << endl;
	cerr << "\t--matrix=%s\tThe path to the input matrix file." << endl;
	cerr << "\t--tree=%s\tThe path to the output tree file." << endl;
	cerr << "\t--size=%u\tNumber of data points" << endl;
	cerr << "\t--threshold=%f\tMaximum value in the input distance matrix." << endl;
	cerr << "\t--linkage=%s\tLinkage type: average, complete, or single. " << endl;
}

int extractOptions(const char* format, void* value, int argc, char** argv) {
	int i;
	for (i = 0; i < argc; i++)
		if (sscanf(argv[i], format, value) > 0)
			return 1;
	return 0;
}


