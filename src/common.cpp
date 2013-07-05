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
	lapse_ = end_.tv_sec - start_.tv_sec
			+ 1e-6 * (end_.tv_usec - start_.tv_usec);
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
	cout << "Commands: " << endl;
	cout << "\tSparseHC" << endl;
	cout << "Options: " << endl;
	cout
			<< "\t--num-threads=%u\tNumber of parallel threads, auto-detect if not specified"
			<< endl;
	cout
			<< "\t--threshold=%f\tMaximum value in the sparse distance matrix. [default=1.0f]"
			<< endl;
	cout
			<< "\t--linkage=%s\tLinkage for hierarchical clustering (average, complete, or single). [default=average]"
			<< endl;
	cout
			<< "\t--read-file=%s\tInput file in fasta or fastq format, switch to generate mode if not specified"
			<< endl;
	cout << "\t--k=%u\tk value for computing k-mer distance. [default=6]"
			<< endl;
	cout << "\t--size=%u\tNumber of generated vectors. [default=10]" << endl;
	cout << "\t--dim=%u\tDimension of each vector. [default=3]" << endl;
	cout << "\t--verbose=%s\tPrint intermediate results. [default=false]"
			<< endl;
}

int extractOptions(const char* format, void* value, int argc, char** argv) {
	int i;
	for (i = 0; i < argc; i++)
		if (sscanf(argv[i], format, value) > 0)
			return 1;
	return 0;
}

string sizeOnDisk(size_t sizeInBytes) {
	char buf[BUF_SIZE];
	float size = (float) sizeInBytes / 1024;
	if (size < 1024) {
		sprintf(buf, "%f KB", size);
		return string(buf);
	}

	size /= 1024;
	if (size < 1024) {
		sprintf(buf, "%f MB", size);
		return string(buf);
	}

	size /= 1024;
	fprintf(stderr, buf, "%f GB", size);
	return string(buf);
}

