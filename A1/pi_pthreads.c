/*
============================================================================
Filename    : pi.c
Author      : Your names goes here
SCIPER		: Your SCIPER numbers
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utility.h"

typedef struct threads_op
{
    int samples;
    int thread_id;
    int result;
} threads_op;

double calculate_pi(int num_threads, int samples);
void countInCircle(threads_op* args);



int main (int argc, const char *argv[]) {

    int num_threads, num_samples;
    double pi;

    if (argc != 3) {
		printf("Invalid input! Usage: ./pi <num_threads> <num_samples> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
	}

    set_clock();
    pi = calculate_pi (num_threads, num_samples);

    printf("- Using %d threads: pi = %.15g computed in %.4gs.\n", num_threads, pi, elapsed_time());

    return 0;
}


double calculate_pi (int num_threads, int samples) {
    double pi;
    pthread_t* threads = calloc(num_threads, sizeof(pthread_t));
    threads_op* args = calloc(num_threads, sizeof(threads_op));

    for(int i = 0; i < num_threads; i++) {
        args[i].samples = samples/num_threads;
        args[i].thread_id = i;
        pthread_create( &threads[i], NULL, countInCircle, &args[i]);
    }


    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int in_circle = 0;

    for(int i = 0; i < num_threads; i++) {
        in_circle += args[i].result;
    }

    pi = 4.0 * in_circle / samples;

    return pi;
}

void countInCircle(threads_op* args) {
    int in_circle = 0;
    rand_gen rdm = init_rand_pthreads(args->thread_id);

    for (size_t i = 0; i < args->samples; i++) {
        double x = next_rand(rdm);
        double y = next_rand(rdm);
        if ((x*x) + (y*y) <= 1){
            in_circle += 1;
        }
    }

    free_rand(rdm);
    args->result = in_circle;
}