/*
============================================================================
Filename    : algorithm.c
Author      : Ahmed Elalamy
SCIPER      : 324610

============================================================================
*/
#include <math.h>

#define INPUT(I,J) input[(I)*length+(J)]
#define OUTPUT(I,J) output[(I)*length+(J)]

void simulate(double *input, double *output, int threads, int length, int iterations)
{
    double *temp;
    omp_set_num_threads(threads);
    // Parallelize this!!
    int n, i, j, tid;
    double tmp_in[threads][length*length];
    double tmp_out[threads][length*length];
#pragma omp parallel private(n,i,j, tid) shared(tmp_in, tmp_out)
    {
        tid = omp_get_thread_num();
        init(tmp_in[tid], length);
        init(tmp_out[tid], length);
        for (n = 0; n < iterations/threads; n++) {
            for (i = 1; i < length - 1; i++) {
                for (j = 1; j < length - 1; j++) {
                    if (((i == length / 2 - 1) || (i == length / 2))
                        && ((j == length / 2 - 1) || (j == length / 2)))
                        continue;

                    OUTPUT(i, j) = (INPUT(i - 1, j - 1) + INPUT(i - 1, j) + INPUT(i - 1, j + 1) +
                                    INPUT(i, j - 1) + INPUT(i, j) + INPUT(i, j + 1) +
                                    INPUT(i + 1, j - 1) + INPUT(i + 1, j) + INPUT(i + 1, j + 1)) / 9;
                }
            }

            temp = input;
            input = output;
            output = temp;
        }
    }
}
