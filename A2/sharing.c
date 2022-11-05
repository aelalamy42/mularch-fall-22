/*
============================================================================
Filename    : sharing.c
Author      : Ugo Balducci
SCIPER		: 325035
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

int perform_bucket_computation(int, int, int);

int main (int argc, const char *argv[]) {
    int num_threads, num_samples, num_buckets;

    if (argc != 4) {
		printf("Invalid input! Usage: ./sharing <num_threads> <num_samples> <num_buckets> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
        num_buckets = atoi(argv[3]);
	}
    
    set_clock();
    perform_buckets_computation(num_threads, num_samples, num_buckets);

    printf("Using %d threads: %d operations completed in %.4gs.\n", num_threads, num_samples, elapsed_time());
    return 0;
}

int perform_buckets_computation(int num_threads, int num_samples, int num_buckets) {
    volatile int *histogram = (int*) calloc(num_buckets, sizeof(int));
    int** tmp_hist = calloc(num_threads, sizeof(histogram));
    int val, tid;

    omp_set_num_threads(num_threads);

    #pragma omp parallel private(val, tid) shared(tmp_hist)
    {
        tid = omp_get_thread_num();
        rand_gen generator = init_rand();
        tmp_hist[tid] = (int*)calloc(num_buckets, sizeof(int));

        #pragma omp for nowait
        for(int i = 0; i < num_samples; i++){
            val = next_rand(generator) * num_buckets;
            tmp_hist[tid][val]++;
        }

        free_rand(generator);
    }

    #pragma omp parallel for shared (tmp_hist, histogram) private(num_buckets)
    for(int i = 0; i < num_buckets; i++) {
        int tid;
        for (tid = 0; tid < omp_get_num_threads(); tid++) {
            histogram[i] += tmp_hist[tid][i];
        }
    }

    return 0;
}
