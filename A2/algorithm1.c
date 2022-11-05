/*
============================================================================
Filename    : algorithm.c
Author      : Your names go here
SCIPER      : Your SCIPER numbers

============================================================================
*/
#include <math.h>

#define INPUT(I,J) input[(I)*length+(J)]
#define OUTPUT(I,J) output[(I)*length+(J)]

void simulate(double *input, double *output, int threads, int length, int iterations)
{
    double *temp;
    int n;

    omp_set_num_threads(threads);

    // Parallelize this!!
    #pragma omp parallel private(n)
    {
    for(n=0; n < iterations; n++)
    {
        #pragma omp for collapse(2)
        for(int i=1; i<length-1; i++)
        {
            for(int j=1; j<length-1; j+=8)
            {
                for(int k = j; k < j + 8; k++) {
                    if ( ((i == length/2-1) || (i== length/2))
                        && ((k == length/2-1) || (k == length/2)) )
                        continue;

                    OUTPUT(i,k) = (INPUT(i-1,k-1) + INPUT(i-1,k) + INPUT(i-1,k+1) +
                                   INPUT(i,k-1)   + INPUT(i,k)   + INPUT(i,k+1)   +
                                   INPUT(i+1,k-1) + INPUT(i+1,k) + INPUT(i+1,k+1) )/9;
                }
            }
        }

        temp = input;
        input = output;
        output = temp;
    }
    }
}
