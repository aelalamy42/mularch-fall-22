/*
============================================================================
Filename    : pi.c
Author      : Ugo Balducci & Ahmed Elalamy
SCIPER		: 325035 & 324610
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

double calculate_pi (int num_threads, int samples);

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

    /* Your code goes here */
    omp_set_num_threads(num_threads);
    int in_circle = 0;
    #pragma omp parallel reduction(+:in_circle)
    {
        rand_gen rdm = init_rand();
    for (size_t i = 0; i < samples / num_threads; i++) {
        double x = next_rand(rdm);
        double y = next_rand(rdm);
        if ((x * x) + (y * y) <= 1) {
            in_circle += 1;
        }
    }
    }
    pi = 4.0 * in_circle / samples;
    return pi;

}
